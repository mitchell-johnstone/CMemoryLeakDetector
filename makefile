all: driver.c 
	gcc -Wall -o main mem_lib.h mem_lib.c driver.c
