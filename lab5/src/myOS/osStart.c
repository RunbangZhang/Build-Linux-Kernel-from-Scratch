#include "include/i8253.h"
#include "include/i8259.h"
#include "include/irq.h"
#include "include/uart.h"
#include "include/vga.h"
#include "include/mem.h"
#include "include/task.h"
#include "include/myPrintk.h"

unsigned long BspContextBase[STACK_SIZE];
unsigned long *BspContext;

void osStart(void){
     //pressAnyKeyToStart(); // prepare for uart device
     init8259A();
     init8253();
     enable_interrupt();

     clear_screen();


     pMemInit();
     {
     	unsigned long tmp = dPartitionAlloc(pMemHandler,100);
     	dPartitionWalkByAddr(pMemHandler);
     	dPartitionFree(pMemHandler,tmp);
     	dPartitionWalkByAddr(pMemHandler);
     }

     TaskManagerInit();

     while(1);
}
