/**
 *  基于pthread.h以及类实现c++轻量级线程池
 *
 */

#ifndef PTHREADPOOL_H_
#define PTHREADPOOL_H_
#include <pthread.h>
#include <queue>
#include "MutexCond.h"

class PthreadPool{
    /**
     *  线程入口函数
     */
    friend void * pthread_routine(void * arg);

public:
    /**
     *  构造函数, 传入参数需要初始化参数最大线程数
     */
    PthreadPool(unsigned int _max, unsigned int _wait_time=0);

    /**
     *  析构函数
     */
    ~PthreadPool();

    /**
     *  任务注册函数
     *  @void*(*run)(void *)    任务回调函数
     *  @arg                    回调函数参数
     */
    void add_task(void*(*_run)(void *), void * _arg);

protected:
    /**
     *  线程池销毁函数
     */
    void destroy();

private:

    /**
     *  任务结构体
     */
    struct TaskNode{
        void*(*run)(void *);
        void * arg;
    };

    /**
     *  任务队列
     */
    std::queue<TaskNode*> task_queue;

    /**
     *  线程上限
     */
    unsigned int pthread_max;

    /**
     *  当前线程数
     */
    unsigned int pthread_count;

    /**
     *  当前线程空闲线程数
     */
    unsigned int pthread_idle;

    /**
     *  线程等待时间
     */
    unsigned int wait_time;

    /**
     *  退出标志
     */
    bool quit_flag;

    /**
     *  互斥条件量
     */
    MutexCond m_cond;
};

#endif
