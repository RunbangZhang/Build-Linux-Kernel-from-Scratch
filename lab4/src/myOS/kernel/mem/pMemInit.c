#include "../../include/myPrintk.h"
#include "../../include/mem.h"
unsigned long pMemStart;//可用的内存的起始地址
unsigned long pMemSize;//可用的大小

void memTest(unsigned long start, unsigned long grainSize){			//内存检测
	int notFinished = 1;
	unsigned short *toTestAddr,*rearAddr;
	unsigned short temp;

	if (start<0x100000) {		//开始的地址要大于1M
		myPrintk(0x7,"????????  IN memTest: start is too small, should>=1MB  ????????\n"); while(1);
	}

	if (grainSize<0x1000) {		//默认规定grainsize要大于4kB
		myPrintk(0x7,"????????  IN memTest: grainSize is too small, should>=4KB  ????????\n"); while(1);
	}

	pMemStart = start;
	toTestAddr = (unsigned short*) start;       //unsigned short占用2个字节
	while(1){							
		temp= *toTestAddr;
		
		*toTestAddr = 0xAA55;          //通过先覆盖再读入的方法检测grainsize的头2个字节
		if(*toTestAddr!=0xAA55) notFinished=0;

		*toTestAddr = 0x55AA;
		if(*toTestAddr!=0x55AA) notFinished=0;

		*toTestAddr = temp;				//恢复头2个字节

		rearAddr=toTestAddr+grainSize/sizeof(unsigned short)-1;
		temp= *rearAddr;

		*rearAddr = 0xAA55;          //检测grainsize的尾2个字节
		if(*rearAddr!=0xAA55) notFinished=0;

		*rearAddr = 0x55AA;
		if(*rearAddr!=0x55AA) notFinished=0;

		*rearAddr = temp;			//恢复尾2个字节

		if(!notFinished) break;

		toTestAddr += grainSize/sizeof(unsigned short);		//检测下一块内存
	}
	
	pMemSize = (unsigned long)toTestAddr - start;

	myPrintk(0x7,"MemStart: %x  \n",pMemStart);
	myPrintk(0x7,"MemSize:  %x  \n",pMemSize);
}

extern unsigned long _end;
void pMemInit(void){										//初始化,包括确定动态内存
	unsigned long _end_addr = (unsigned long) &_end;
	memTest(0x100000,0x1000);
	myPrintk(0x7,"_end:  %x  \n", _end_addr);
	if (pMemStart <= _end_addr) {
		pMemSize -= _end_addr - pMemStart;         //1M到_end的部分不计入动态内存
		pMemStart = _end_addr;
	}

	pMemHandler = dPartitionInit(pMemStart,pMemSize);	
}
