/*
 * mm.c
 *
 * Name: [Kevin Starnes]
 *
 * 
   MallocLAB design: This design consists of four main function of implementation along with many helper functions. The four         functions are, mm_init, malloc, free, and realloc. The function mm_init initializes the heap so our other functions can work.     Next function is malloc. The purpose of malloc is to allocate a given space with the size the user requests. It does some         checks to see if the value the user put it valid. If there is a valid value, the design will use the first fit placement policy.  This policy says, if I want a size of 8 bytes, it will look throught the sizes of the free blocks and check if these values       will fit    my reuqest space. If it fit does, then that free block will be used. The third function is free. The purpose of       free is to free   an allocated space. In the previous example, I used a size of 8 bytes. I am finished with this space, so        instead of it just sitting there being useless space, we can get rid of it to reuse that space. So this block of 8 bytes will     be freed, and then additionally, it will search for other free blocks and combine the new free block with other free blocks.      The final function is realloc. The purose of realloc is to reallocate an already allocated space. If checks if the size the       user wants is valid. If the value they enter is 0 bytes, this is eqivualent to the functoin free. If the size they entire is      NULL. It returns the space space the already allocated. If the size is neither of those, it will copy the oringinal data to the   new allocate space along with extra or less space, depending on the size of the space. Lets go back to my 8 bytes example. I      want to reallocate that 8 bytes of space to 10 bytes. I will have those 8 original bytes of memory plus 2 new bytes of memory.    If i want to resize it to 5 bytes, then the new allocated will be 5 bytes, but only 5 bytes of the orinal data. The last 3 will   bacially be deleted. 
 
 *
 */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#include "mm.h"
#include "memlib.h"

/*
 * If you want to enable your debugging output and heap checker code,
 * uncomment the following line. Be sure not to have debugging enabled
 * in your final submission.
 */
#define DEBUG

#ifdef DEBUG
// When debugging is enabled, the underlying functions get called
#define dbg_printf(...) printf(__VA_ARGS__)
#define dbg_assert(...) assert(__VA_ARGS__)
#else
// When debugging is disabled, no code gets generated
#define dbg_printf(...)
#define dbg_assert(...)
#endif // DEBUG

// do not change the following!
#ifdef DRIVER
// create aliases for driver tests
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#define memset mm_memset
#define memcpy mm_memcpy
#endif // DRIVER

#define ALIGNMENT 16
////////////////////////////////////////
/////////////HELPER_FUNCTIONS////////////
/////////////////////////////////////////


//Citing from the text book
//changing the macros into functions

uint64_t MAX(uint64_t x, uint64_t y){        // takes two values. if x is larger returns x, if y is larger return y
  return ((x) > (y) ? (x) : (y));
}

uint64_t LOW(uint64_t x, uint64_t y){        // takes two values. if x is smaller returns x, if y is smaller it returns y.
  return ((x) < (y) ? (x) : (y));
}

// int CHUNKSIZE(){
//return (1<<12);
//}

 uint64_t PACK(size_t size, uint64_t alloc){      
  return ((size) | (alloc));

}
uint64_t GET(void* ptr){             // gets the pointer
  return (*(uint64_t *)(ptr));
}

void PUT(void* ptr,uint64_t val)   // sets the pointer to a value
{
   (*(uint64_t *)(ptr) = (val));
}


size_t GET_SIZE(void* ptr){   // gets the last 4 digits of the pointer address
  return (GET(ptr) & ~0xF);
}

size_t GET_ALLOC(void* ptr){   // gets the allocation 
  return (GET(ptr) & 0x1);
}

void* HDRP(void* ptr){           // returns the pointer to the header in heap
  return ((char *)(ptr) - 8);
}

 void* FTRP(void* ptr){
   return ((char *)(ptr) + GET_SIZE(HDRP(ptr)) - 16);     // returns pointer to the footer in the heap
}

	  
 void* NEXT_BLKP(void* ptr){
   return ((char *)(ptr) + GET_SIZE(((char *)(ptr) - 8)));     //returns the pointer of the next block
}

 void* PREV_BLKP(void* ptr){
   return ((char *)(ptr) - GET_SIZE(((char *)(ptr) - 16)));   //returns the pointer of the previous block
}


/////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
/////////////END OF MACRO -> FUNCTIONS//////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
/////////Other helper functions from text book//////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////


static char *heap_listp;

static void *coalesce(void *ptr){                              // this function will find free blocks and combine them together in                                                                  to 1 big free block
   size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
   size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
   size_t size = GET_SIZE(HDRP(ptr));

   if(prev_alloc && next_alloc) {
     return ptr;
   }
   else if(prev_alloc && !next_alloc){
     size+= GET_SIZE(HDRP(NEXT_BLKP(ptr)));
     PUT(HDRP(ptr), PACK(size, 0));
     PUT(FTRP(ptr), PACK(size,0));
   }
   else if (!prev_alloc && next_alloc){
     size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
     PUT(FTRP(ptr), PACK(size,0));
     PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
     ptr = PREV_BLKP(ptr);
   }
   else
     {
       size += GET_SIZE(HDRP(PREV_BLKP(ptr))) + GET_SIZE(FTRP(NEXT_BLKP(ptr)));
       PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
       PUT(FTRP(NEXT_BLKP(ptr)), PACK(size, 0));
       ptr = PREV_BLKP(ptr);
     }
   return ptr;
     }
 

static void* extend_heap(size_t words){   //if the heap is too small, then this function will extend the heap by a certain amount                                               given
  char* ptr;
  size_t size;
  
  size = (words % 2) ? (words+1) * 8 : words * 8;
  if((long)(ptr = mm_sbrk(size)) == -1){
     return NULL;
    }
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1));

    return coalesce(ptr);
}

static void place(void *ptr, size_t asize){   //this will place the allocation 
  size_t csize = GET_SIZE(HDRP(ptr));

  if((csize - asize) >= (2*16)) {
    PUT(HDRP(ptr), PACK(asize, 1));
    PUT(FTRP(ptr), PACK(asize, 1));
    ptr = NEXT_BLKP(ptr);
    PUT(HDRP(ptr), PACK(csize-asize, 0));
    PUT(FTRP(ptr), PACK(csize-asize, 0));
  }
  else{
    PUT(HDRP(ptr), PACK(csize, 1));
    PUT(FTRP(ptr), PACK(csize, 1));
  }
}

static void *find_fit(size_t asize){                // this is the first fit search. it will look for free blocks that is bigger o                                                       or equal to the requested size. 
   //first fit search//
   void *ptr;
   for(ptr = heap_listp; GET_SIZE(HDRP(ptr)) > 0; ptr = NEXT_BLKP(ptr)) {
     if(!GET_ALLOC(HDRP(ptr)) && (asize <= GET_SIZE(HDRP(ptr)))) {
       return ptr;
     }
   }
   return NULL;
 }
 
/////////////////////////////////////////////////
/////End of helper function from text book///////
/////////////////////////////////////////////////


// rounds up to the nearest multiple of ALIGNMENT
static size_t align(size_t x)
{
    return ALIGNMENT * ((x+ALIGNMENT-1)/ALIGNMENT);
}


/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
///////Citing from text book. Using mm_init page 883///////
//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

/*
 * mm_init: returns false on error, true on success.
 */


bool mm_init(void)   // this initalizes the heap. 
{
  if((heap_listp = mm_sbrk(4*8)) == (void *)-1){
     return false;
    }
    
    PUT(heap_listp,0);
    PUT(heap_listp + (1*8), PACK(16, 1));
    PUT(heap_listp + (2*8), PACK(16, 1));
    PUT(heap_listp + (3*8), PACK(0,1));
    heap_listp += (2*8);

    if(extend_heap((1<<12)/ 8) == NULL)
       return false;
    
    return true;
}



///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/////Citing from text book. Using mm_malloc page 886///////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

/*
 * malloc
 */
void* malloc(size_t size)  // this will allocate a requested size. if the size it 0, there is no point of allocating it. It will                                 just return null. 
{
  size_t asize;
   size_t extendsize;
   char *ptr;
  if(size == 0)
    return NULL;

  if(size <= 16){
    asize = 2*16;
  }
  else
    {
      asize = 16 * ((size + (16) + (16-1)) / 16);   //
		 }
  if((ptr = find_fit(asize)) != NULL) {
	place(ptr,asize);
	return ptr;
      }

  extendsize = MAX(asize, (1<<12)); //this will return  highest vallue betwen asize and chunksize. 
  if((ptr = extend_heap(extendsize/8)) == NULL) //which ever one of the two values will be returned, and then divided by 8 so it can match our alignment. 
       return NULL;
  place(ptr,asize);   // finally it will place the allocation with the value of the pointer. 
    return ptr;
}



///////////////////////////////////////////////////
///Citing from text book. Using mm_free page 885///
///////////////////////////////////////////////////


/*
 * free
 */
void free(void* ptr)
{
  size_t size = GET_SIZE(HDRP(ptr));
  PUT(HDRP(ptr), PACK(size, 0));        /// will free the entire block with the given pointer. after it frees, it will then combin                                             e it with other free blocks for effeiency. 
  PUT(FTRP(ptr), PACK(size, 0));
  coalesce(ptr);
}

/* * realloc
 */
void* realloc(void* oldptr, size_t size)      // this function resizes the allocation of a certain pointer with the size the user                                                   wants
{
  
  if(oldptr == NULL){                           // if ptr is null
     return malloc(size);
   }

  
  if(size == 0){                             // if size is 0
    free(oldptr);
    return NULL;
  }
  void* temp = malloc(size);                           // to no lose important data, i will create a new allocated spot called                                                          temp
  void* ptr = malloc(size);                             // this will be the final pointer of the new allocation
  if(oldptr != NULL && size != 0){
    size_t size2 = LOW(GET_SIZE(HDRP(oldptr)), size);  // to find what need to be copied over, it finds the smallest value of the                                                          old pointer and the new size.
    mm_memcpy(temp, oldptr, size2);                     // copy the original data into the temp
    free(oldptr);                                       // free the old ptr block
    mm_memcpy(ptr,temp, size2);                          // copy over the temp to the final pointer
    free(temp);                                          // free the temp data
 //combine the two free blocks with other free blocks. 
    return ptr;
    coalesce(oldptr);
    coalesce(temp);
  }
  return ptr;
}
/*
 * calloc
 * This function is not tested by mdriver, and has been implemented for you.
 */
void* calloc(size_t nmemb, size_t size)
{
    void* ptr;
    size *= nmemb;
    ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

/*
 * Returns whether the pointer is in the heap.
 * May be useful for debugging.
 */
static bool in_heap(const void* p)
{
    return p <= mm_heap_hi() && p >= mm_heap_lo();
}

/*
 * Returns whether the pointer is aligned.
 * May be useful for debugging.
 */
static bool aligned(const void* p)
{
    size_t ip = (size_t) p;
    return align(ip) == ip;
}

/*
 * mm_checkheap
 * You call the function via mm_checkheap(__LINE__)
 * The line number can be used to print the line number of the calling
 * function where there was an invalid heap.
 */
bool mm_checkheap(int line_number)
{
#ifdef DEBUG
  mm_checkheap(5);
    // Write code to check heap invariants here
    // IMPLEMENT THIS
#endif // DEBUG
    return true;
}
