#include <assert.h>
#include "Coroutine.h"
#include "Schedule.h"

Coroutine::Coroutine()
		:id(0),state(FREE),cap(0),stack_size(0),buffer(nullptr)
{

}

Coroutine::~Coroutine()
{
	delete [] buffer;
}

void Coroutine::CoProcess()
{

}

void Coroutine::Resume()
{
	if(state==SUSPEND){
		ReloadStack();
		state = RUNNING;
		swapcontext(&(SingleSchedule::GetInst()->mainCtx), &ctx);
	}
}

void Coroutine::Yield()
{
	if (state == RUNNING){
		SaveStack();
		state = SUSPEND;
		swapcontext(&ctx, &(SingleSchedule::GetInst()->mainCtx));
	}
}

void Coroutine::SaveStack()
{
	char * stackBottom = SingleSchedule::GetInst()->GetStackBottom();
	char dumy = 0;

	assert(stackBottom-&dumy <= DEFAULT_STACK_SIZE);
	if (cap<stackBottom-&dumy){
		if(buffer){
			delete [] buffer;
		}
		cap = stackBottom-&dumy;
		buffer = new char[cap];
	}
	stack_size = stackBottom-&dumy;
	memcpy(buffer, &dumy, stack_size);
}

void Coroutine::ReloadStack()
{
	memcpy(SingleSchedule::GetInst()->GetStackBottom()-stack_size, buffer, stack_size);
}