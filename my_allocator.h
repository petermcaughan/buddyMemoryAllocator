/* 
    File: my_allocator.h

    Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

*/

#ifndef _my_allocator_h_                   // include file only once
#define _my_allocator_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <vector>
#include <iostream>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/
typedef struct h{
	struct h *next;
	struct h *prev;
	unsigned int size;
	bool isFree;
	unsigned int marker;
}Header;


typedef struct flist{
	 Header *first;
	 Header *last;
	 int seg_size;
}Flist;


typedef void * Addr; 

Addr memstart;

std::vector<Flist> flist;

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* MODULE   MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

unsigned int init_allocator(unsigned int _basic_block_size, 
			    unsigned int _length); 
/* This function initializes the memory allocator and makes a portion of 
   ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
   its minimal unit of allocation. The function returns the amount of 
   memory made available to the allocator. ./a.If an error occurred, 
   it returns 0. 
*/ 

void release_allocator(); 
/* This function returns any allocated memory to the operating system. 
   After this function is called, any allocation fails.
*/ 

Addr my_malloc(unsigned int _length); 
/* Allocate _length number of bytes of free memory and returns the 
   address of the allocated portion. Returns 0 when out of memory. */ 

int my_free(Addr _a); 
/* Frees the section of physical memory previously allocated 
   using ’my_malloc’. Returns 0 if everything ok. */ 

int index_search(std::vector<Flist> flist, Header* head);

Header* allocator_coalesce(Header* h);
#endif 
