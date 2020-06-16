#include "../include/tcb.h"
#include "../include/taskPara.h"
#include "../include/task_sched.h"

extern void initLeftExeTime_sjf(myTCB* tsk);

// may modified during scheduler_init
tskPara defaultTskPara = {
    .priority = MAX_PRIORITY_NUM,
    .exeTime = MAX_EXETIME,
    .arrTime = 0,
    .schedPolicy = SCHED_UNDEF
}; //task设计调度的一些参数的默认值

void _setTskPara(myTCB *task, tskPara *para){   //将para的中的参数值赋值给task的对应参数

    if(para == (void*)0)
        task->para = defaultTskPara;
    else task->para = *para;
}

void initTskPara(tskPara *buffer){    //将任务buffer的参数设置为默认值
    *buffer = defaultTskPara;
}

void setTskPara(unsigned int option, unsigned int value, tskPara *buffer){    //设置task的设计调度的四个参数
    //option控制buffer的哪个参数要被赋值，value是具体的数值
    switch (option){
        case PRIORITY:        buffer->priority=value; break;
        case EXETIME:         buffer->exeTime=value; break;
        case ARRTIME:         buffer->arrTime=value; break;
        case SCHED_POLICY:    buffer->schedPolicy=value; break;
        default :;
    }
}

void getTskPara(unsigned option, unsigned int *para){      //查看task的设计调度的四个参数
    //option控制buffer的哪个参数要查看 赋值给para
    switch (option){
        case PRIORITY:        *para = currentTsk->para.priority; break;
        case EXETIME:         *para = currentTsk->para.exeTime; break;
        case ARRTIME:         *para = currentTsk->para.arrTime; break;
        case SCHED_POLICY:    *para = currentTsk->para.schedPolicy; break;
        default :;
    }

}
