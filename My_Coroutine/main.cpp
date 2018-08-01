#include <stdio.h>
#include <memory>
#include "Coroutine.h"
#include "Schedule.h"


class Logic1 : public Coroutine{
	void CoProcess(){
		puts("1");
		Yield();
		puts("4");
		Yield();
		puts("7");
	}
};

class Logic2 : public Coroutine{
	void CoProcess(){
		puts("2");
		Yield();
		puts("5");
		Yield();
		puts("8");
	}
};

class Logic3 : public Coroutine{
	void CoProcess(){
		puts("3");
		Yield();
		puts("6");
		Yield();
		puts("9");
	}
};

int main() {

	std::shared_ptr<Coroutine> ct1(new Logic1());
	std::shared_ptr<Coroutine> ct2(new Logic2());
	std::shared_ptr<Coroutine> ct3(new Logic3());

	ct1->SetId(1);
	ct2->SetId(2);
	ct3->SetId(3);

	SingleSchedule::GetInst()->CoroutineNew(ct1.get());
	SingleSchedule::GetInst()->CoroutineNew(ct2.get());
	SingleSchedule::GetInst()->CoroutineNew(ct3.get());

	SingleSchedule::GetInst()->Resume(1);
	SingleSchedule::GetInst()->Resume(2);
	SingleSchedule::GetInst()->Resume(3);
	SingleSchedule::GetInst()->Resume(1);
	SingleSchedule::GetInst()->Resume(2);
	SingleSchedule::GetInst()->Resume(3);


	//SingleSchedule::GetInst()->Remove(1);
	//SingleSchedule::GetInst()->Remove(2);
	//SingleSchedule::GetInst()->Remove(3);

	int count = SingleSchedule::GetInst()->HasCoroutine();
	printf("%d\n", count);

	return 0;
}
