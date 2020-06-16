#include "../../include/task_sched.h"
#include "../../include/taskPara.h"

myTCB *rqSJF;  //SJF队列

void rqSJFInit(void) {			//初始化SJF调度队列
	rqSJF = idleTsk;	// head <- idleTsk
	dLinkListInit((dLinkedList *)(&(rqSJF->thisNode)));
}

int Start_tick;				//记录任务开始的时刻
extern int getTick(void);
extern void tskEnd(void);

myTCB * nextSJFTsk(void) {		//返回队列中下一个待调度的任务
	Start_tick = getTick();
	return (myTCB*)dLinkListFirstNode((dLinkedList*)rqSJF);	
}


void tskEnqueueSJF(myTCB *tsk){			//任务入队列
    dLink_node *q = ((dLinkedList*)rqSJF)->next;
    while(q!=(dLinkedList*)rqSJF && ((myTCB *)q)->para.exeTime<=tsk->para.exeTime)
         q=q->next;
    dLinkInsertBefore((dLinkedList*)rqSJF,q,(dLink_node *)tsk);
}


void tskDequeueSJF(myTCB *tsk){			//任务出队列
	dLinkDelete((dLinkedList*)rqSJF,(dLink_node*)tsk);	
}


void schedulerInit_SJF(void){			//调度器初始化
    rqSJFInit();

	/* default for all task except idleTsk*/
	defaultTskPara.schedPolicy = SCHED_SJF;

	/* special for idleTsk*/
	_setTskPara(idleTsk,&defaultTskPara);
	idleTsk->para.schedPolicy = SCHED_IDLE;

	initTsk_para.exeTime = MAX_EXETIME;    //init任务执行时间设为MAX以保证不被强行中断
}

void tickSJF_hook(void){				//每次tick检查一次是否超时
	if(getTick()-Start_tick>=currentTsk->para.exeTime)
		tskEnd();
}

struct scheduler scheduler_SJF = {       //将各接口封装成一个调度器
    .type = SCHEDULER_SJF,
    .nextTsk_func = nextSJFTsk,
    .enqueueTsk_func = tskEnqueueSJF,
    .dequeueTsk_func = tskDequeueSJF,
    .schedulerInit_func = schedulerInit_SJF,
	.createTsk_hook = NULL,
    .tick_hook = tickSJF_hook
};

