/**
 * @file 	: Coroutine.h
 * @author	: neilzwshen
 * @time	: 2018-7-31
 * @version	: 3.0
 * @remark	: 用户协程类
 */

#ifndef COROUTINE_H_
#define COROUTINE_H_
#define DEFAULT_STACK_SIZE (1024*1024)
#define _XOPEN_SOURCE
#include <stdio.h>
#include <string.h>
#include <ucontext.h>


enum CoState {FREE = 0, RUNNING = 1, SUSPEND = 2};

class Coroutine
{
public:
	Coroutine();
	virtual ~Coroutine();

	/**
	 * 用户协程入口函数
	 */
	virtual void CoProcess();
	
	/**
	 * 用户协程恢复函数
	 */
	void Resume();

	/**
	 * 获取协程id
	 * @return [返回id]
	 */
	int GetId()const {
		return id;
	}

	/**
	 * 设置协程id
	 */
	void SetId(int _id) {
		id = _id;
	}

	/**
	 * 获取协程状态
	 * @return [返回协程状态]
	 */
	int GetState()const {
		return state;
	}

	/**
	 * 设置协程状态
	 */
	void SetState(CoState _state) {
		state = _state;
	}

protected:
	/**
	 * 用户协程挂起函数
	 */
	void Yield();

	/**
	 * 堆栈缓存
	 */
	void SaveStack();

	/**
	 * 堆栈恢复
	 */
	void ReloadStack();
	
public:
	char *buffer;		// 缓存协程堆栈
	ucontext_t ctx;

private:
	int stack_size;
	int cap;
	int id;
	CoState state;
};

#endif