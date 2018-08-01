#include <assert.h>
#include "Schedule.h"

Schedule::Schedule()
{

}

Schedule::~Schedule()
{

}

void Schedule::CoroutineEntry(void * crt) {
	((Coroutine *)crt)->SetState(RUNNING);
	((Coroutine *)crt)->CoProcess();
	((Coroutine *)crt)->SetState(FREE);
}

void Schedule::CoroutineNew(Coroutine * crt) {
	
	int id = crt->GetId();
	CoState state = CoState(crt->GetState());
	assert(id != 0);
	assert(state == FREE);
	//printf("--%d,%d--\n",id, state);

	if (crtPool[id] != nullptr) {
		CrtMap::iterator it = crtPool.find(id);
		crtPool.erase(it);
	}
	
	// 构建用户协程上下文
	getcontext(&(crt->ctx));
	//memset(stack, 0, DEFAULT_STACK_SIZE);
	crt->ctx.uc_stack.ss_sp = stack;
	crt->ctx.uc_stack.ss_size = DEFAULT_STACK_SIZE;
	crt->ctx.uc_stack.ss_flags = 0;
	crt->ctx.uc_link = &mainCtx;
	crtPool[id] = crt;
	
	makecontext(&crt->ctx, (void(*)(void))CoroutineEntry, 1, (void *)crt);
	swapcontext(&mainCtx, &crt->ctx);
}

void Schedule::Resume(int id){
	if (crtPool[id] != nullptr) {
		crtPool[id]->Resume();
	}
}

int Schedule::HasCoroutine() {
	int count = 0;
	CrtMap::iterator it;
	for (it = crtPool.begin(); it != crtPool.end(); it++) {
		if (it->second->GetState() != FREE) {
			count++;
		}else{
			it=crtPool.erase(it);
			it--;
		}
	}
	return count;
}

void Schedule::Remove(int id) {
	if (crtPool[id] != nullptr) {
		crtPool.erase(crtPool.find(id));
	}
}