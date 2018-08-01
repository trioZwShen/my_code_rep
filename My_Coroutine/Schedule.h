/**
 * @file 	: Schedule.h
 * @author	: neilzwshen
 * @time	: 2018-7-31
 * @version	: 3.0
 * @remark	: 协程调度类
 */

#ifndef SCHEDULE_H_
#define SCHEDULE_H_
#define _XOPEN_SOURCE
#include <stdio.h>
#include <map>
#include <ucontext.h>
#include "Coroutine.h"
#include "Singleton.h"


typedef std::map<int, Coroutine*> CrtMap;

class Schedule
{
public:
	Schedule();
	virtual ~Schedule();

	/**
	 * 用户协程入口函数
	 */
	static void CoroutineEntry(void * crt);

	/**
	 * 将协程crt加入协程池, 并开启
	 * @param  crt [协程指针]
	 */
	void CoroutineNew(Coroutine * crt);

	/**
	 * 恢复用户协程
	 * @param id [description]
	 */
	void Resume(int id);

	/**
	 * 判断协程池中是否还有未完成的协程, 并将已经终止的协程删除
	 * @return [返回协程数]
	 */
	int HasCoroutine();

	/**
	 * 根据协程id删除协程
	 * @param id [协程id]
	 */
	void Remove(int id);

	/**
	 * 获取到栈底
	 * @return [返回栈底地址]
	 */
	char* GetStackBottom(){
		return stack + DEFAULT_STACK_SIZE;
	}

public:
	ucontext_t mainCtx;
	char stack[DEFAULT_STACK_SIZE];		// 运行协程堆栈

private:
	CrtMap crtPool;
};

typedef Singleton<Schedule> SingleSchedule;

#endif