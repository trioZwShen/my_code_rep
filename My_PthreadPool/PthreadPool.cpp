/**
 *  基于pthread.h以及类实现c++轻量级线程池
 *
 */
#include <stdio.h>
#include <errno.h>
#include "PthreadPool.h"

void * pthread_routine(void * arg){
    pthread_detach(pthread_self());
    printf("thread 0x%x start...\n", pthread_self());
    PthreadPool * pool_ptr = reinterpret_cast<PthreadPool *>(arg);
    int is_timeout = 0;
    while(true){
        pool_ptr->m_cond.lock();
        pool_ptr->pthread_idle++;     // 空闲线程+1
        // 等待任务队列中出现新任务, 或者线程池销毁通知
        while (pool_ptr->task_queue.empty() && !pool_ptr->quit_flag){
            printf("thread 0x%x wait...\n", pthread_self());
            int STATUS = pool_ptr->m_cond.timedwait(pool_ptr->wait_time);
            if (STATUS == ETIMEDOUT){         // 如果返回值非0, 表示超时
                printf("thread 0x%x timeout\n", pthread_self());
                is_timeout = 1;
                break;
            }
        }
        pool_ptr->pthread_idle--;     // 空闲线程-1

        // 如果是任务队列非空
        if (!pool_ptr->task_queue.empty()){
            PthreadPool::TaskNode * tar_ptr = pool_ptr->task_queue.front();     // 从队头取出任务
            pool_ptr->task_queue.pop();                                         // 任务出队
            pool_ptr->m_cond.unlock();                                          // 解锁, 因为执行任务需要一定时间, 如需再次上锁, 在run中自行处理
            tar_ptr->run(tar_ptr->arg);                                         // 执行任务
            delete tar_ptr;                                                     // 释放空间
            pool_ptr->m_cond.lock();                                            // 上锁
        }

        // 如果是销毁通知且任务队列为空, 那么解锁, 并跳出循环, 结束线程
        if (pool_ptr->quit_flag && pool_ptr->task_queue.empty()){
            pool_ptr->pthread_count--;
            // 如果当前线程为0 那么解除destory中的条件变量
            if (pool_ptr->pthread_count==0){
                pool_ptr->m_cond.signal();
            }
            pool_ptr->m_cond.unlock();
            break;
        }

        // 如果是等待超时, 那么解锁, 并跳出循环, 结束线程
        if (is_timeout){
            pool_ptr->pthread_count--;
            pool_ptr->m_cond.unlock();
            break;
        }

        pool_ptr->m_cond.unlock();
    }
    printf("thread 0x%x exit\n", pthread_self());
    return NULL;
}

PthreadPool::PthreadPool(unsigned int _max, unsigned int _wait_time)
    :task_queue(), pthread_max(_max), pthread_count(0), pthread_idle(0), quit_flag(false), 
    wait_time(_wait_time), m_cond()
{
}

PthreadPool::~PthreadPool(){
    destroy();
}

void PthreadPool::add_task(void*(*_run)(void *), void * _arg){
    m_cond.lock();      // 因为涉及到对线程池中参数的变化, 因此需要加锁
    // 创建任务并加入队列
    TaskNode *new_task = new TaskNode;
    new_task->run = _run;
    new_task->arg = _arg;
    task_queue.push(new_task);

    if (pthread_idle>0){
        // 存在空闲线程
        m_cond.signal();

    }else if(pthread_count<pthread_max){
        // 不存在空闲线程, 但是当前线程数量小于线程上限
        pthread_t tid;
        pthread_create(&tid, NULL, pthread_routine, this);  // 开启一个新线程
        pthread_count++;                                    // 线程数+1
    }
    m_cond.unlock();
}

void PthreadPool::destroy(){
    if (!quit_flag){
        m_cond.lock();
        quit_flag = true;
        if (pthread_count>0){
            // 如果存在空闲线程, 发出信号
            if (pthread_idle>0){
                m_cond.broadcast();
            }
            
            // 如果队列非空, 那么将线程挂起
            while (pthread_count>0){
                m_cond.wait();
            }
        }
        m_cond.unlock();
    }
}
    
