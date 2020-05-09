#include "../../include/myPrintk.h"


//dPartition 是整个动态分区内存的数据结构
struct dPartition{
	unsigned long size;
	unsigned long firstFreeStart; 
};

void showdPartition(struct dPartition *dp){
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}


// 一个EMB表示一个空闲可用的Block，userdata可以暂时不用管。
struct EMB{
	unsigned long size;
	union {
		unsigned long nextStart;    // if free: pointer to next block
        unsigned long userData;		// if allocated, belongs to user
	};	                           
};

#define MINI_EMB_SIZE sizeof(struct EMB)
#define OVERHEAD_EMB (sizeof(struct EMB)-sizeof(struct EMB*))   //记录内存块大小所需内存
#define MINI_DP_SIZE (sizeof(struct EMB)+sizeof(struct dPartition))

void showEMB(struct EMB * emb){
	myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x)\n", emb, emb->size, emb->nextStart);
}

unsigned long dPartitionInit(unsigned long start, unsigned long totalSize){  //初始化内存
	struct dPartition * theDP;
	struct EMB *firstEMB;

	if (totalSize < MINI_DP_SIZE) return 0;  //totalSize至少要容纳一个EMB和一个dP

	theDP = (struct dPartition*)start;      //theDP结构体表示整个数据结构
	theDP->size  = totalSize;
	theDP->firstFreeStart = start+sizeof(struct dPartition);     
	
	firstEMB = (struct EMB*)(theDP->firstFreeStart);      //一整块的EMB被分配,在内存中紧紧跟在dP后面
	firstEMB->size = totalSize-sizeof(struct dPartition);  
	firstEMB->nextStart = 0;	 //for the end of List

	return start;
}

void dPartitionWalkByAddr(unsigned long dp){	//查看内存分配结构
	struct dPartition * theDP = (struct dPartition *) dp;
	showdPartition(theDP);                          //先印dP的信息
	unsigned long addr = theDP->firstFreeStart;
	struct EMB* emb;
	while(addr!=0){              //遍历EMB并打印其地址，大小和以及下一个EEB的地址
		emb=(struct EMB*)(addr);
		showEMB(emb);
		addr = emb->nextStart;
	}
}

//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
**/
unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size){ //使用firstfit算法分配内存块
	struct dPartition *theDP;
	unsigned long curr, prev=0, next, rear;
	unsigned long actualSize = size + OVERHEAD_EMB;    
	unsigned long sizeLeft;
	int notfind=1;

	actualSize = ((actualSize+3)>>2)<<2;
	if (actualSize<MINI_EMB_SIZE) actualSize = MINI_EMB_SIZE;

	// find the first fit
	theDP = (struct dPartition*)dp;
	curr = theDP->firstFreeStart;

	while (notfind) {
		if (curr == 0) {
			notfind = 1;
			break;   // not find, &finish
		}

		if (actualSize<=((struct EMB*)curr)->size) {
			notfind=0;
			break;  // find, &finish
		}
		
		//next loop
		prev = curr;
		curr = ((struct EMB*)prev)->nextStart;
	}

	if(notfind) 
		return 0;
	else {	//find
		sizeLeft = ((struct EMB*)curr)->size - actualSize;
		if (sizeLeft >= MINI_EMB_SIZE) { 
			//need cut, return curr, insert rear
			rear = curr + actualSize;
			((struct EMB*)rear)->size = ((struct EMB*)curr)->size - actualSize;
			((struct EMB*)rear)->nextStart = ((struct EMB*)curr)->nextStart;
			if(prev) ((struct EMB*)prev)->nextStart = rear;
			else theDP->firstFreeStart = rear;

			((struct EMB*)curr)->size = 	actualSize;
		} else { // do not cut
			if(prev) ((struct EMB*)prev)->nextStart = ((struct EMB*)curr)->nextStart;
			else theDP->firstFreeStart = ((struct EMB*)curr)->nextStart;
		}

		return curr+OVERHEAD_EMB;
	} 
}

/*
 *r
 */
unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){  //释放内存
	unsigned long curr = start-OVERHEAD_EMB;
	unsigned long prev=0, next;
	struct dPartition *theDP;
	int notfind=1;
	theDP = (struct dPartition*)dp;
	next = theDP->firstFreeStart;
	prev = 0;
	
	//检查start是否在dp范围内
	if(curr<dp+sizeof(struct dPartition)||curr>=dp+theDP->size)
		return 0;

	//find position in freelist
	while(notfind){		
		if ((next == 0)||(next > curr)) { // insert curr before next
			
			notfind = 0;
			if (prev == 0) // the first
				theDP->firstFreeStart = curr;
			else ((struct EMB*)prev)->nextStart = curr;
			
			((struct EMB*)curr)->nextStart = next;		

			if(next&&next==curr+((struct EMB*)curr)->size){				//向后合并
				((struct EMB*)curr)->nextStart = ((struct EMB*)next)->nextStart;		
				((struct EMB*)curr)->size += ((struct EMB*)next)->size;		
			}
			if(prev&&curr==prev+((struct EMB*)prev)->size){				//向前合并
				((struct EMB*)prev)->nextStart = ((struct EMB*)curr)->nextStart;		
				((struct EMB*)prev)->size += ((struct EMB*)curr)->size;		
			}

			break;
		}
		prev = next;
		next = ((struct EMB*)next)->nextStart;
	}

	return 1;
	
}

//wrap: we select firstFit, you can select another one
//user need not know this
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long	 dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}
