/* 
    File: my_allocator.c

    Author: <your name>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include "my_allocator.h"
#include <cmath>
#include <cstdint>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */

bool power_of_two(int n)
{
  if (n == 0)
    return 0;
  while (n != 1)
  {
    if (n%2 != 0)
      return 0;
    n = n/2;
  }
  return 1;
}

extern int index_search(std::vector<Flist> flist, Header* head){
	if (head->size == 0) return 0;
	int seg_size = flist[0].seg_size;
	int index = log2(head->size) - log2(seg_size);
	return index;
	}

extern Header* add_to_list(std::vector<Flist> flist, Header* head){
	
	//get index
	int index = index_search(flist, head);
	//if flist is empty, just add to it
	if (flist[index].first == NULL) {flist[index].first = head; flist[index].last = head;}
	else{
		flist[index].last->next = head;
		head -> prev = flist[index].last;
		flist[index].last = head;
	}
	
}

extern void* remove_from_list(std::vector<Flist> flist, Header* head){
	
	//get index
	int index = index_search(flist, head);
	
	
	
	//IF HEADER IS FIRST
	if (flist[index].first == head)
		flist[index].first = head -> next;
	
	//IF HEADER IS LAST
	else if (flist[index].last == head)
		flist[index].last = head -> prev;
		
	else if (head->prev == NULL && head->next == NULL) return 0;
		
	//IF HEADER IS FIRST AND LAST
	else if (flist[index].first == head && flist[index].last == head){
		flist[index].first = NULL;
		flist[index].last = NULL;
	}
	
	else if (head->next == NULL){
		flist[index].last = head->prev;
		head->prev->next = NULL;
	}
	
	else if (head->prev == NULL){
		flist[index].first = head->next;
		head->next->prev = NULL;
	}

	
	//OTHERWISE
	else{
		Header* tmp = head -> prev;
		head->prev->next = head->next;
		head->next->prev = tmp;
		delete tmp;
	}
	
}

Header* allocator_find_bi(int value){
		int space; 						// The "next 1"
		if (!power_of_two(value))		// If the number isnt a power of two
			space = floor(log2(value)) + 1;
		else(space = log2(value) + 1);
		space = pow(2, space);
	
		// Space now holds the next 2^n value
		// Find the index of the freelist that holds the block size we want
		
		int index = log2(space) - log2(flist[0].seg_size);
	
		if (index < 0) return NULL;		// No size in our free list
	
		// Find a free block
	
		Header* free_block = NULL;
		Header* head;
	
	
		// This loop goes through all the blocks that have a size greater than or equal to the desired value, and returns the first free block
		for (int i = index; i < flist.size(); i++){
			head = flist[i].first;
			for (Header* ptr = head; (ptr != NULL && ptr->next != ptr); ptr = ptr->next){
				if (ptr->isFree == true){
					free_block = ptr;
					break;}
			}
		}
	
	if (free_block == NULL) return NULL;		// If we didn't
	
	return free_block;
			

  

}

Header* segment_cut(Header* h){
  remove_from_list(flist, h);
  intptr_t new_head = (intptr_t)h + sizeof(Header) +  h->size/2;
  h->size = h->size/2;
  Header* h2 = (Header*)new_head;
  add_to_list(flist, h);
  add_to_list(flist, h2);
  return h2;
}



//Need int return for exponential function




////////////////////// This function was taken from StackOverflow. It's simply an int version of pow() provided in the C library.

int IntPow(int x, unsigned int pow)
{
    int ret = 1;
    while ( pow != 0 )
    {
        if ( (pow & 1) == 1 )
            ret *= x;
        x *= x;
        pow >>= 1;
    }
    return ret;
}
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


extern Header* allocator_coalesce(Header* h){
	
	//finding the buddy

		// Get relative address
		intptr_t rel_addr = ((intptr_t)h - (intptr_t)memstart);
		//XOR with size of block
		intptr_t xor_addr = ((intptr_t)rel_addr ^ (intptr_t)(h->size));
		//Get absolute address
		intptr_t abs_addr = xor_addr + (intptr_t)memstart;
		Header* buddy = (Header*)abs_addr;
	
	//Check if buddy can be coalesced
		if(buddy -> isFree == false || h -> size != buddy -> size) return 0;	// Can't do anything
		else if(buddy -> isFree == true && h -> size == buddy -> size){
			//coalesce
			//find which one will absorb the other, which is leftmost in physical memory
			Header* leftmost;
			if (buddy > h) {leftmost = h; h = NULL;}
			else {leftmost = buddy; buddy = NULL;}
			
			//Remove the new block from the list and add it back into right place after adjusting memory;
			remove_from_list(flist, leftmost);
			leftmost -> size = leftmost->size*2;
			add_to_list(flist, leftmost);
			return leftmost;
		}
}
extern unsigned int init_allocator(unsigned int _basic_block_size, 
			    unsigned int _length){
	
	// Handle impossible conditions
	if (_basic_block_size <= 0 || _length <=0 ) return 0;
	else if (_length < _basic_block_size) return 0;
		
		
		
	else {	
		// Reserve next power of 2 of space
		int space;
		if (!power_of_two(_length))		// If the number isnt a power of two
			space = floor(log2(_length)) + 1;
		else(space = log2(_length));
		space = pow(2, space);
		std::cout << " \n Length Allocated : " << space << std::endl;
		
		// Same for block size
		int block;
		if (!power_of_two(_basic_block_size))		// If the number isnt a power of two
			block = floor(log2(_basic_block_size)) + 1;
		else(block = log2(_basic_block_size));
		block = pow(2, block);
		std::cout << " \n Block Size Allocated : " << block << std::endl;
		
		//Find how many free lists you need, set the correct amount
		int num_lists = 0;
		for (int i = space; i >= block; i = i/2)
			num_lists++;
			
		flist.resize(num_lists);
		std::cout << " \n Number of free lists is : " << num_lists << std::endl;
		
		//Initialize the free lists
		for (int i = 0; i < num_lists; i++){
			flist[i] = Flist();
			flist[i].first = NULL;
			flist[i].last = NULL;
			flist[i].seg_size = block*IntPow(2, i);
			
		}
		
		//Load the free lists with blocks
		int current_bytes = _length;
		for (int i = num_lists - 1; i >= 0; i--){
			flist[i].first = new Header();
			flist[i].last = flist[i].first;
			flist[i].first->next = NULL;
			flist[i].first->prev = NULL;
			
			if (_length < block * pow(2, i) ) {flist[i].first->size = 0;}
			else{
				if (current_bytes > block * IntPow(2, i-1) ){
				current_bytes = current_bytes - block * IntPow(2, i);
				flist[i].first->size = block * IntPow(2, i);
			}
			else{
				flist[i].first->size = 0;}
			}
			flist[i].first->isFree = true;
			std::cout << "Size of block in freelist " << i << " is " << flist[i].first->size << std::endl;
			}
			
		
		// Remove all blocks that have a size of zero, or blocks that should not be there
			for (int i = num_lists - 1; i >= 0; i--){
				if (flist[i].first->size==0){
					flist[i].first == NULL; 
					flist[i].last == NULL;
				}
			}
					
		// Malloc the memory pool
		int memory;
		for (int i = num_lists - 1; i >= 0; i--)
			memory = memory + flist[i].first->size;
		
		memstart = malloc(memory);
		
		return space;
			
			
		
	}
}
	
extern void release_allocator(){
	
	// Free the memory allocated
	free(memstart);
	
	//Trick to deallocate memory for flist vector, swap with empty vector (free memory)
	std::vector<Flist>().swap(flist);
}
		

extern Addr my_malloc(unsigned int _length) {
  /* 
     Allocate _length number of bytes of free memory and returns the 
     address of the allocated portion. Returns 0 when out of memory.

     This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */

		// Find the next highest byte value we want to look for
		
  if (_length == 0) return 0;
  Header *h = allocator_find_bi(_length + sizeof(Header));
  if (h == NULL){
	  return 0;
  }
//See if we should split the block
  if ((h->size)/2 >= (_length + sizeof(Header))){		// If we can split block in two and still have room, split it
     Header *h2 = segment_cut(h);
     
     add_to_list(flist, h2);
     return my_malloc(_length);
  } else {
     //cannot further split, return the address
     h->isFree = false;
     return (void *)(h+1);  
     //the address returned is for the memory, not including the header
}


  return malloc((size_t)_length);
}

extern int my_free(Addr _a) {

	Header* h = (Header*) _a -1;
	h->isFree = true;
	
	//Coalesce, allocator_coalesce takes care of adding back into list
	Header *after_coalesce = allocator_coalesce(h);
	
  return 0;
}

