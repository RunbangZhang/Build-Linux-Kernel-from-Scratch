#include "../../include/task_sched.h"
#include "../../include/taskPara.h"

myTCB *rqSJF;  //ready queue

void rqSJFInit(void) {
	rqSJF = idleTsk;	// head <- idleTsk
	dLinkListInit((dLinkedList *)(&(rqSJF->thisNode)));
}

int Start_tick;
extern int getTick(void);
extern void tskEnd(void);

myTCB * nextSJFTsk(void) {
	Start_tick = getTick();
	return (myTCB*)dLinkListFirstNode((dLinkedList*)rqSJF);	
}


void tskEnqueueSJF(myTCB *tsk){
    dLink_node *q = ((dLinkedList*)rqSJF)->next;
    while(q!=(dLinkedList*)rqSJF && ((myTCB *)q)->para.exeTime<=tsk->para.exeTime)
         q=q->next;
    dLinkInsertBefore((dLinkedList*)rqSJF,q,(dLink_node *)tsk);
}


void tskDequeueSJF(myTCB *tsk){
	dLinkDelete((dLinkedList*)rqSJF,(dLink_node*)tsk);	
}


void schedulerInit_SJF(void){
    rqSJFInit();

	/* default for all task except idleTsk*/
	defaultTskPara.schedPolicy = SCHED_SJF;

	/* special for idleTsk*/
	_setTskPara(idleTsk,&defaultTskPara);
	idleTsk->para.schedPolicy = SCHED_IDLE;

	initTsk_para.exeTime = MAX_EXETIME;
}

void tickSJF_hook(void){
	if(getTick()-Start_tick>=currentTsk->para.exeTime)
		tskEnd();
}

struct scheduler scheduler_SJF = {
    .type = SCHEDULER_SJF,
    .nextTsk_func = nextSJFTsk,
    .enqueueTsk_func = tskEnqueueSJF,
    .dequeueTsk_func = tskDequeueSJF,
    .schedulerInit_func = schedulerInit_SJF,
	.createTsk_hook = NULL,
    .tick_hook = tickSJF_hook
};

