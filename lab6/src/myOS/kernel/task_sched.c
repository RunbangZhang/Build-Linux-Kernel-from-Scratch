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

//这里列举了常见的所有的调度算法 本实验只要求实现两个即可（不包含FCFS）

extern void context_switch(myTCB *prevTsk, myTCB *nextTsk);

struct scheduler *sysScheduler = &scheduler_FCFS;  // default

unsigned int getSysScheduler(void){
    return sysScheduler->type;
}

void setSysScheduler(unsigned int what){    
    ////本函数需要实现！！！
    //设置*sysScheduler使用最上面列举的哪种调度器
    switch (what){
        case SCHEDULER_FCFS:        sysScheduler = &scheduler_FCFS; break;
        //case SCHEDULER_SJF:         sysScheduler = &scheduler_SJF; break;
        case SCHEDULER_PRIORITY0:   sysScheduler = &scheduler_PRIO0; break;
        //case SCHEDULER_RR:          sysScheduler = &scheduler_RR; break;
       //case SCHEDULER_PRIORITY:    sysScheduler = &scheduler_PRIO; break;
        //case SCHEDULER_MQ:          sysScheduler = &scheduler_MQ; break;
        //case SCHEDULER_FMQ:         sysScheduler = &scheduler_FMQ; break;
        default :;
    }
}

//由于RR涉及到设置时间片大小 所以需要如下的两个函数getSysSchedulerPara setSysSchedulerPara
//其他调度器则不需要
void getSysSchedulerPara(unsigned int who, unsigned int *para){
    switch (who){
        case SCHED_RR_SLICE: *para = defaultSlice; break;
        case SCHED_RT_RR_SLICE: *para = defaultRtSlice; break;
        default :;
    }

}

void setSysSchedulerPara(unsigned int who, unsigned int para){
//本函数需要实现！！！
//和上面的get函数是对应的，设置两个参数
    switch (who){
        case SCHED_RR_SLICE: defaultSlice = para; break;
        case SCHED_RT_RR_SLICE: defaultRtSlice = para; break;
        default :;
    }
}

//每一个调度器中集成了几个函数 参考./task_sched/task_fifo.c中的scheduler scheduler_FCFS的结构
//实现以下的nextTsk enqueueTsk dequeueTsk schedulerInit scheduler_tick
myTCB *nextTsk(void){ 
    return sysScheduler->nextTsk_func();
}

void enqueueTsk(myTCB *tsk){
    sysScheduler->enqueueTsk_func(tsk);
}

void dequeueTsk(myTCB *tsk){
    sysScheduler->dequeueTsk_func(tsk);
}

void createTsk_hook(myTCB *created){
    if(sysScheduler->createTsk_hook)
        sysScheduler->createTsk_hook(created);
}

extern void scheduler_hook_main(void);

void schedulerInit(void){
   //注意除了调用调度器中的函数 还需要设置一下SysScheduler的调度算法使用哪个
    scheduler_hook_main();
    sysScheduler->schedulerInit_func();
}

void scheduler_tick(void){
    if(sysScheduler->tick_hook)
        sysScheduler->tick_hook();
}

void schedule(void){
	static int idle_times=0;
	myTCB * prevTsk;

    disable_interrupt();

	//此处实现调度功能！！
    //调度不可被中断
    //可参考实验五的scheduleFCFS的实现
    prevTsk = currentTsk;
    currentTsk = sysScheduler->nextTsk_func();
    if(currentTsk == idleTsk && idle_times==0)    
    {
    	idle_times++;
    	context_switch(prevTsk,currentTsk);
    }
    else if(currentTsk == idleTsk && prevTsk == idleTsk);
    else  context_switch(prevTsk,currentTsk);

    enable_interrupt();
}