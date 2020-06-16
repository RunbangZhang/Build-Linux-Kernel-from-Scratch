#include "../include/tcb.h"
#include "../include/task_sched.h"
#include "../include/irq.h"

extern myTCB * currentTsk;

unsigned int defaultSlice = 10; 
unsigned int defaultRtSlice = 50; 
/*RR    */ extern struct scheduler scheduler_RR;
/*FCFS  */ extern struct scheduler scheduler_FCFS;
/*SJF   */ extern struct scheduler scheduler_SJF;
/*PRIO0 */ extern struct scheduler scheduler_PRIO0;
/*PRIO  */ extern struct scheduler scheduler_PRIO;
/*MQ    */ extern struct scheduler scheduler_MQ;
/*FMQ   */ extern struct scheduler scheduler_FMQ;


extern void context_switch(myTCB *prevTsk, myTCB *nextTsk);

struct scheduler *sysScheduler = &scheduler_FCFS;  // default

unsigned int getSysScheduler(void){
    return sysScheduler->type;
}

void setSysScheduler(unsigned int what){    //设置*sysScheduler选择使用哪种调度器
    switch (what){
        case SCHEDULER_FCFS:        sysScheduler = &scheduler_FCFS; break;
        case SCHEDULER_SJF:         sysScheduler = &scheduler_SJF; break;
        case SCHEDULER_PRIORITY0:   sysScheduler = &scheduler_PRIO0; break;
        //case SCHEDULER_RR:          sysScheduler = &scheduler_RR; break;
        //case SCHEDULER_PRIORITY:    sysScheduler = &scheduler_PRIO; break;
        //case SCHEDULER_MQ:          sysScheduler = &scheduler_MQ; break;
        //case SCHEDULER_FMQ:         sysScheduler = &scheduler_FMQ; break;
        default :;
    }
}

void getSysSchedulerPara(unsigned int who, unsigned int *para){
    switch (who){
        case SCHED_RR_SLICE: *para = defaultSlice; break;
        case SCHED_RT_RR_SLICE: *para = defaultRtSlice; break;
        default :;
    }

}

void setSysSchedulerPara(unsigned int who, unsigned int para){
    switch (who){
        case SCHED_RR_SLICE: defaultSlice = para; break;
        case SCHED_RT_RR_SLICE: defaultRtSlice = para; break;
        default :;
    }
}

myTCB *nextTsk(void){                       //返回下一个调度的任务
    return sysScheduler->nextTsk_func();
}

void enqueueTsk(myTCB *tsk){                //任务进入就绪队列
    sysScheduler->enqueueTsk_func(tsk);
}

void dequeueTsk(myTCB *tsk){                //任务出就绪队列
    sysScheduler->dequeueTsk_func(tsk);
}

void createTsk_hook(myTCB *created){        //创建任务时的hook
    if(sysScheduler->createTsk_hook)
        sysScheduler->createTsk_hook(created);
}

extern void scheduler_hook_main(void);

void schedulerInit(void){                   //调度器初始化
    scheduler_hook_main();                 
    sysScheduler->schedulerInit_func();
}

void scheduler_tick(void){                  //tick时的hook
    if(sysScheduler->tick_hook)
        sysScheduler->tick_hook();
}

void schedule(void){                        //实现调度功能
	static int idle_times=0;
	myTCB * prevTsk;

    disable_interrupt();

    prevTsk = currentTsk;
    currentTsk = sysScheduler->nextTsk_func();
    if(currentTsk == idleTsk && idle_times==0)    //第一次执行idle
    {
    	idle_times++;
    	context_switch(prevTsk,currentTsk);
    }
    else if(currentTsk == idleTsk && prevTsk == idleTsk);
    else  context_switch(prevTsk,currentTsk);

    enable_interrupt();
}