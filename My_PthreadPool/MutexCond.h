/**
 *  基于pthread.h中的互斥量及条件变量实现的互斥条件量(即将两者功能组合封装)
 */

#ifndef MUTEXCOND_H_
#define MUTEXCOND_H_
#include <pthread.h>
#include <time.h>

class MutexCond{
public:
    /**
     *  构造函数, 初始化条件变量与互斥量
     */
    MutexCond(){
        pthread_cond_init(&cond, NULL);
        pthread_mutex_init(&mutex, NULL);
    }
    
    /**
     *  析构函数, 销毁条件变量与互斥量
     */
    ~MutexCond(){
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex);
    }
    
    /**
     *  挂起当前线程
     */
    inline int wait(){
        return pthread_cond_wait(&cond, &mutex);
    }

    /**
     *  挂起当前线程, 并设置最大等待时长
     *  @_time  最大等待时长, 如果为0表示不设等待时长
     */
    inline int timedwait(unsigned int _time){
        if (_time==0)
            return pthread_cond_wait(&cond, &mutex);
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += _time;
        return pthread_cond_timedwait(&cond, &mutex, &abstime);
    }

    /**
     *  释放mutexcond上挂起的第一个线程
     */
    inline int signal(){
        return pthread_cond_signal(&cond);
    }

    /**
     *  释放mutexcond上挂起的全部线程
     */
    inline int broadcast(){
        return pthread_cond_broadcast(&cond);
    }

    /**
     *  加锁
     */
    inline int lock(){
        return pthread_mutex_lock(&mutex);
    }

    /**
     *  解锁
     */
    inline int unlock(){
        return pthread_mutex_unlock(&mutex);
    }

private:
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};




#endif
