#include "../../include/task_sched.h"
#include "../../include/taskPara.h"

myTCB *rqPRIO0;  //优先级调度队列

void rqPRIO0Init(void) {		//初始化优先级调度队列
	rqPRIO0 = idleTsk;	// head <- idleTsk
	dLinkListInit((dLinkedList *)(&(rqPRIO0->thisNode)));
}

myTCB * nextPRIO0Tsk(void) {	//返回队列中下一个待调度的任务
	return (myTCB*)dLinkListFirstNode((dLinkedList*)rqPRIO0);	
}


void tskEnqueuePRIO0(myTCB *tsk){ 			//任务入队列
    dLink_node *q = ((dLinkedList*)rqPRIO0)->next;
    while(q!=(dLinkedList*)rqPRIO0 && ((myTCB *)q)->para.priority<tsk->para.priority)	//按优先级从小到大顺序入队
         q=q->next;
    dLinkInsertBefore((dLinkedList*)rqPRIO0,q,(dLink_node *)tsk);
}


void tskDequeuePRIO0(myTCB *tsk){			//任务出队列
	dLinkDelete((dLinkedList*)rqPRIO0,(dLink_node*)tsk);	
}


void schedulerInit_PRIO0(void){            //调度器初始化
    rqPRIO0Init();

	/* default for all task except idleTsk*/
	defaultTskPara.schedPolicy = SCHED_PRIO;

	/* special for idleTsk*/
	_setTskPara(idleTsk,&defaultTskPara);
	idleTsk->para.schedPolicy = SCHED_IDLE;

	initTsk_para.priority = 0;			  //init任务优先级设为0，为最高优先级
}

struct scheduler scheduler_PRIO0 = {	  //将各接口封装成一个调度器
    .type = SCHEDULER_PRIORITY0,
    .nextTsk_func = nextPRIO0Tsk,
    .enqueueTsk_func = tskEnqueuePRIO0,
    .dequeueTsk_func = tskDequeuePRIO0,
    .schedulerInit_func = schedulerInit_PRIO0,
	.createTsk_hook = NULL,
    .tick_hook = NULL
};

