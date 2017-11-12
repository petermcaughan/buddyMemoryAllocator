#include "ackerman.c"
#include <iostream>
#include <getopt.h>
#include "my_allocator.c"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	
  atexit(release_allocator);
  unsigned int block, size, tmp = -2;
	
  while ((tmp = getopt (argc, argv, "b:s:")) != -1)
    switch (tmp)
      {
      case 'b':
        block = atoi(optarg);
       break;
      case 's':
      	size = atoi(optarg);
       break;
  	}
		  
	if (block <= 0 || size <= 0) { std::cout << " \n Error:  Invalid Argument \n"; return 0; }
	
	init_allocator(block, size);
	

	ackerman_main();
	

	
	std::cout << "\n Program Complete." << std::endl;
	
	
	return 0;
}
