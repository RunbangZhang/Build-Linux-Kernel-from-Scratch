#include "../../include/myPrintk.h"
// 一个EEB表示一个空闲可用的Block
struct EEB {
	unsigned long next_start;
};

void showEEB(struct EEB *eeb){
	myPrintk(0x7,"EEB(start=0x%x, next=0x%x)\n", eeb, eeb->next_start);
}

//eFPartition是表示整个内存的数据结构
struct eFPartition{
	unsigned long totalN;
	unsigned long perSize;  //unit: byte	
	unsigned long firstFree;
};

#define OVERHEAD_EFP (sizeof(struct eFPartition))

void showeFPartition(struct eFPartition *efp){
	myPrintk(0x5,"eFPartition(start=0x%x, totalN=0x%x, perSize=0x%x, firstFree=0x%x)\n", efp, efp->totalN, efp->perSize, efp->firstFree);
}

void eFPartitionWalkByAddr(unsigned long efpHandler){				//查看内存分配结构
	struct eFPartition * theEFP = (struct eFPartition*)efpHandler;
	showeFPartition(theEFP);				//打印eFPartiiton结构体的信息
	unsigned long addr = theEFP->firstFree;
	struct EEB* eeb;
	while(addr!=0){							//遍历每一个EEB，打印出他们的地址以及下一个EEB的地址
		eeb=(struct EEB*)(addr);
		showEEB(eeb);
		addr = eeb->next_start;
	}
}


unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n){		//计算A大小
	unsigned actualSize = ((perSize + 3) >> 2) << 2;   // aligned up to 4
	return (actualSize*n+OVERHEAD_EFP);				//加上结构体eFPartition自身大小
}

unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n){    //初始化内存
	struct EEB * nextEEB;
	unsigned long nextStart = start + OVERHEAD_EFP;
	unsigned long actualSize = ((perSize + 3) >> 2) << 2;   // aligned up to 4
	struct eFPartition * theEFP = (struct eFPartition*)start;

	theEFP->totalN  	= n;						//创建一个eFPartition记录整个内存的结构
	theEFP->perSize 	= actualSize;
	theEFP->firstFree = nextStart;

	for(int i=0; i<n ; i++) {						//对每块内存创建一个EEB
		nextEEB = (struct EEB *)nextStart;
		nextStart += perSize;
		nextEEB->next_start = nextStart;
	}

	nextEEB->next_start = 0;
	return start;
}


unsigned long eFPartitionAlloc(unsigned long EFPHandler){	//分配空闲内存
	struct eFPartition * theEFP = (struct eFPartition*)EFPHandler;
	unsigned long addr = theEFP->firstFree;
	if (addr!=0)		 			//分配第一块空闲的内存
		theEFP->firstFree = ((struct EEB*)(addr))->next_start;

	return addr;
}


unsigned long eFPartitionFree(unsigned long EFPHandler,unsigned long mbStart){ //释放内存
	struct eFPartition * theEFP;
    struct EEB* newFree;

	theEFP = (struct eFPartition*)EFPHandler;

	newFree = (struct EEB*)mbStart;
	newFree->next_start = theEFP->firstFree;  //更新EBB组成的链表
	theEFP->firstFree = mbStart;    //mbstart成为第一个空闲块

	return 1;
}
