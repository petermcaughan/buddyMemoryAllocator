# makefile

all: memtest


ackerman.o: ackerman.h ackerman.c
	g++ -c -g ackerman.c 
	
	
my_allocator.o: my_allocator.h my_allocator.c 
	g++ -g -std=c++11 -c -g my_allocator.c 

memtest: memtest.c my_allocator.o ackerman.o
	g++ -g -std=c++11 -o memtest -g memtest.c 

clean :  
		rm memtest my_allocator.o ackerman.o
