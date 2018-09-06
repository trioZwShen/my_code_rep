/**
 *  简单线程池的测试函数
 */

#include <iostream>
#include <unistd.h>
#include "PthreadPool.h"

void * my_task(void * arg){
    printf("task %d\n", *(int *)arg);
    sleep(rand()%3);
}

int main()
{
    // 创建一个线程池, 并且初始化线程上限为3
    PthreadPool pthread_pool(3, 2);
    // 往线程池中注册任务
    for (int i=0; i<20; ++i){
        int * tmp = new int;
        *tmp = i;
        pthread_pool.add_task(my_task, (void *)tmp);
        sleep(6);
    }

    //sleep(30);

    return 0;
}
