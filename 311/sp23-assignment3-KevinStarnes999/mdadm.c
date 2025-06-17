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

  
  int diskNum(uint32_t addr){
    return addr / JBOD_DISK_SIZE;
  }

  int blockNum(uint32_t addr){
    return addr % JBOD_DISK_SIZE / JBOD_BLOCK_SIZE;    // functions for variables
  }
  int offNum(uint32_t addr){
    return addr % JBOD_BLOCK_SIZE;
  }
		   
  int disk = diskNum(addr);
  int block = blockNum(addr);
  int offset = offNum(addr);
  uint8_t temp[JBOD_BLOCK_SIZE];

  uint32_t operation = (op_help(JBOD_SEEK_TO_DISK,disk,0)); //updating disk
  jbod_operation(operation, NULL);
	
  operation = (op_help(JBOD_SEEK_TO_BLOCK,0,block)); //updating block
  jbod_operation(operation, NULL);
  if(0 < len && buf == NULL){  //read parameters
    return -1;
}
 
  if((is_mounted == 1) && (len <= 1024) && (addr + len <= 1048576)){  //read parameters
    if(offset + len >= JBOD_BLOCK_SIZE){ 
      int bits = 0; // keeps track of the bits read

    while(len > bits) {
      int holder = len - bits; 
      int holder2 = JBOD_BLOCK_SIZE - offset;
     
      operation = (op_help(JBOD_READ_BLOCK,disk,block)); //reads block
      jbod_operation(operation, temp);

      if(( 1 <= offset)) { // if the offset and bits are bigger than the block
	memcpy(buf, temp + offset, holder2);
	bits = bits + (holder2);
      }
      else if(JBOD_BLOCK_SIZE < (offset + holder))  { // if the bits are than the block
	memcpy(buf+bits, temp, JBOD_BLOCK_SIZE);
	bits = bits + JBOD_BLOCK_SIZE;
      }
      else{ // if the bits  and offset are less than the blcok
	memcpy(buf + bits, temp + offset, holder);
	bits = bits + holder;
      }
      operation = (op_help(JBOD_SEEK_TO_BLOCK,0,block)); //updating block
      jbod_operation(operation, NULL);
       
      operation = (op_help(JBOD_READ_BLOCK,disk,block)); //reading block
      jbod_operation(operation, temp);

      block++; 
      offset = 0;

      if(block == JBOD_BLOCK_SIZE){ //checks if it hit the max block
	disk++; //moves to next disk if it did
	block = 0; //resets the block num to 0;
	
	operation = (op_help(JBOD_SEEK_TO_DISK,disk,0)); //updating disk
	jbod_operation(operation, NULL);
	
	operation = (op_help(JBOD_SEEK_TO_BLOCK,0,block)); //updating block
	jbod_operation(operation, NULL);

      }
    }
  }
  else{
    operation = (op_help(JBOD_READ_BLOCK,disk,block)); //reading block
    jbod_operation(operation, temp);

    memcpy(buf, temp + offset, len);
    
    operation = (op_help(JBOD_SEEK_TO_BLOCK,0,block));// updating block
    jbod_operation(operation, NULL);
       
    operation = (op_help(JBOD_READ_BLOCK,disk,block)); //reading block
    jbod_operation(operation, temp);
  }
  
  return len;
}
  else{
    return -1;
  }
}





int mdadm_write(uint32_t addr, uint32_t len, const uint8_t *buf) {
 
 
  if(1 <= len && buf == NULL){
    return -1;
  }

   int diskNum(uint32_t addr){
    return addr / JBOD_DISK_SIZE;
  }

  int blockNum(uint32_t addr){
    return addr % JBOD_DISK_SIZE / JBOD_BLOCK_SIZE; //functions for variables
  }
  int offNum(uint32_t addr){
    return addr % JBOD_BLOCK_SIZE;
  }
		   
  int disk = diskNum(addr);
  int block = blockNum(addr);
  int offset = offNum(addr);
  uint8_t temp[JBOD_BLOCK_SIZE];


  uint32_t operation = (op_help(JBOD_SEEK_TO_DISK,disk,0));  //updating disk
	jbod_operation(operation, NULL);
	
	operation = (op_help(JBOD_SEEK_TO_BLOCK,0,block)); //updating block
	jbod_operation(operation, NULL);
	if((is_mounted == 1) && (len <= 1024) && (addr + len <= 1048576)){ //write parameters

	if( JBOD_BLOCK_SIZE <= offset + len) {
	  int bits = 0;  //tracking bits

	  while(len > bits){ 
	    int holder = len - bits;
	    int holder2 = JBOD_BLOCK_SIZE - offset;
	    operation = (op_help(JBOD_READ_BLOCK,disk,block)); //reads block
	    jbod_operation(operation, temp);
	  
	    if(1 <= offset) { //if the offset and bits are bigger than the block
	    memcpy(temp + offset, buf, holder2);
	    bits = bits + (holder2);
	  }
	
	    else if (JBOD_BLOCK_SIZE <= (holder + offset)) {  //if the bits are bigger than the block
	    memcpy(temp + offset, buf + bits , JBOD_BLOCK_SIZE);
	    bits = bits + JBOD_BLOCK_SIZE;
	  }
	    else{ //if the bits and offset are smaller than the block
	    memcpy(temp + offset, buf + bits, holder);
	    bits = bits + (holder);
	  }
	
	    operation = (op_help(JBOD_SEEK_TO_BLOCK,0,block)); //updating block
	  jbod_operation(operation, NULL);

	  operation = (op_help(JBOD_WRITE_BLOCK,disk,block)); //writing block
	  jbod_operation(operation, temp);
	  offset = 0;
	  block++; //moves until last block

	  if(block == JBOD_BLOCK_SIZE) { //if thet last block in disk
	    disk++; //move to next disk
	    block = 0; //make block num 0

	    operation = (op_help(JBOD_SEEK_TO_DISK,disk,0)); //updating disk
	    jbod_operation(operation, NULL);
	
	    operation = (op_help(JBOD_SEEK_TO_BLOCK,0,block)); //updating block
	    jbod_operation(operation, NULL);

	  }
	}
}
	
	else {
	  operation = (op_help(JBOD_READ_BLOCK,disk,block)); //reading block
	  jbod_operation(operation, temp);

	  memcpy(temp + offset, buf, len);

	  operation = (op_help(JBOD_SEEK_TO_BLOCK,0,block)); //updating block
	  jbod_operation(operation, NULL);

	  operation = (op_help(JBOD_WRITE_BLOCK,disk,block)); //writing block
	  jbod_operation(operation, temp);
	}

     
  return len;
	}
  else{
    return -1;
  }
}
