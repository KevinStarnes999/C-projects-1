#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "mdadm.h"
#include "jbod.h"

uint32_t op_help(uint32_t command, uint32_t disk_num, uint32_t block_num){
  uint32_t op = command << 26 | disk_num << 22 | block_num;
  return op;
}

int is_mounted = 0;

int mdadm_mount(void) {
  if(is_mounted == 1){
  return -1;
  }
  uint32_t operation =  op_help(JBOD_MOUNT, 0,0);
  int y = jbod_operation(operation, NULL);
  if( y ==0){
    is_mounted = 1;
    return 1;
  }
  else
    {
      return -1;
    }

}

  

int mdadm_unmount(void) {
  if(!is_mounted){
  return -1;
  }
  uint32_t operation =  op_help(JBOD_UNMOUNT,0,0);
  int y = jbod_operation(operation, NULL);
  if( y == 0)
    {
    is_mounted = 0;
    return 1;
    }
  else
    {
      return -1;
    }
}
int mdadm_read(uint32_t addr, uint32_t len, uint8_t *buf) {
  if(1024 < len){
    return -1;
  }
  if(is_mounted == 0){
    return -1;
  }
  if(1048576 < addr + len)
    {
      return -1;
    }
  if(1 <= len && buf == NULL){
    return -1;
  }
        int diskNum = addr/(JBOD_BLOCK_SIZE * JBOD_NUM_BLOCKS_PER_DISK);
  	int blockNum = (addr % JBOD_DISK_SIZE) / JBOD_BLOCK_SIZE;
	int read = addr; 
	int offset = read % 256;
	uint8_t tempbuffer[256];  
	int current_offset = 0;
 
  while(read < addr + len){
	uint32_t operation = (op_help(JBOD_SEEK_TO_DISK,diskNum,blockNum));
	jbod_operation(operation, NULL);
	
	operation = (op_help(JBOD_SEEK_TO_BLOCK,diskNum,blockNum));
	jbod_operation(operation, NULL);
	
	operation = (op_help(JBOD_READ_BLOCK,diskNum,blockNum));
	jbod_operation(operation, tempbuffer);

	
	if(256 > len && offset == 0)
	  {
	    memcpy(buf, tempbuffer, len);
	    read = read + len;
	  }
	else
	  {
	    memcpy(buf, tempbuffer, 256);
	    current_offset = current_offset + 256;
	    read = read + 256;
	  }

  }
  return len;
  }
