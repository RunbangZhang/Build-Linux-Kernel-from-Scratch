#include "../../include/task_sched.h"
#include "../../include/taskPara.h"

myTCB *rqPRIO0;  //ready queue

void rqPRIO0Init(void) {
	rqPRIO0 = idleTsk;	// head <- idleTsk
	dLinkListInit((dLinkedList *)(&(rqPRIO0->thisNode)));
}

myTCB * nextPRIO0Tsk(void) {
	return (myTCB*)dLinkListFirstNode((dLinkedList*)rqPRIO0);	
}


void tskEnqueuePRIO0(myTCB *tsk){
	//dLinkInsertBefore((dLink_node *)rqPRIO0,(dLink_node *)rqPRIO0,(dLink_node*)tsk);
    dLink_node *q = ((dLinkedList*)rqPRIO0)->next;
    while(q!=(dLinkedList*)rqPRIO0 && ((myTCB *)q)->para.priority<tsk->para.priority)
         q=q->next;
    dLinkInsertBefore((dLinkedList*)rqPRIO0,q,(dLink_node *)tsk);
}


void tskDequeuePRIO0(myTCB *tsk){
	dLinkDelete((dLinkedList*)rqPRIO0,(dLink_node*)tsk);	
}


void schedulerInit_PRIO0(void){
    rqPRIO0Init();

	/* default for all task except idleTsk*/
	defaultTskPara.schedPolicy = SCHED_PRIO;

	/* special for idleTsk*/
	_setTskPara(idleTsk,&defaultTskPara);
	idleTsk->para.schedPolicy = SCHED_IDLE;

	initTsk_para.priority = 0;
}

struct scheduler scheduler_PRIO0 = {
    .type = SCHEDULER_PRIORITY0,
    .nextTsk_func = nextPRIO0Tsk,
    .enqueueTsk_func = tskEnqueuePRIO0,
    .dequeueTsk_func = tskDequeuePRIO0,
    .schedulerInit_func = schedulerInit_PRIO0,
	.createTsk_hook = NULL,
    .tick_hook = NULL
};

