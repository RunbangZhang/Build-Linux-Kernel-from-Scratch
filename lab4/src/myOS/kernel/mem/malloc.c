#include "../../include/mem.h"

unsigned long malloc(unsigned long size){
	dPartitionAlloc(pMemHandler,size);
}

unsigned long free(unsigned long start){
	dPartitionFree(pMemHandler,start);
}