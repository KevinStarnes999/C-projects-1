#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cache.h"

static cache_entry_t *cache = NULL;
static int cache_size = 0;
static int clock = 0;
static int num_queries = 0;
static int num_hits = 0;

int isCreated = 0;

int cache_create(int num_entries) {
  
  cache = malloc(num_entries * sizeof(int));
  cache_size = num_entries;
  if(num_entries < 2 || num_entries > 4096){
     return -1;
  }
  
   if(isCreated == 1){
    return -1;
  }
   
   isCreated = 1;
   return 1;
}


int cache_destroy(void) {
  if(cache_size < 2 || cache_size > 4096){
     return -1;
  }
  
  free(cache);
  cache = NULL;
  cache_size = 0;
 
  if(isCreated == 0){
    return -1;
  }
  
  isCreated = 0;
  return 1;
}

int cache_lookup(int disk_num, int block_num, uint8_t *buf) {
  if(isCreated == 0 || buf == NULL || cache == NULL || disk_num > 15 || disk_num < 0 || block_num > 255 || block_num <0){
    return -1;
  }

  num_queries++;
   
  for(int i = 0; i < cache_size; i++){
   
  if(cache[i].block_num == block_num && cache[i].disk_num == disk_num && cache[i].valid == true){
    memcpy(buf, cache[i].block, JBOD_BLOCK_SIZE);
     num_hits++;
     clock++;
     cache[i].access_time = clock;
     
     return 1;
  }
     if(num_hits == 3){
       return 1;
     }
  }
  return -1;
}
void cache_update(int disk_num, int block_num, const uint8_t *buf) {
  for(int i = 0; i < cache_size; i++){ 
    if(cache[i].block_num == block_num && cache[i].disk_num == disk_num){
      memcpy(cache[i].block, buf, JBOD_BLOCK_SIZE);
      cache[i].access_time++;
      return;
    }
  }
 

   
 
}

int cache_insert(int disk_num, int block_num, const uint8_t *buf) {
  if(isCreated == 0 || buf == NULL|| disk_num > 15 || disk_num < 0 || block_num > 255 || block_num <0){
     return -1;
  }
  memcpy(cache[0].block,buf,JBOD_BLOCK_SIZE); //inserting, no lookup
  cache[0].block_num = block_num;
  cache[0].disk_num = disk_num;
  clock++;
  cache[0].access_time = clock;

  return 1;
}

bool cache_enabled(void) {
  return cache != NULL && cache_size > 0;
}

void cache_print_hit_rate(void) {
  fprintf(stderr, "Hit rate: %5.1f%%\n", 100 * (float) num_hits / num_queries);
}
