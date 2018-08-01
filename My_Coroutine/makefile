all:
	g++ -std=c++11 -g Schedule.cpp -c -o Schedule.o
	g++ -std=c++11 -g Coroutine.cpp -c -o Coroutine.o
	g++ -std=c++11 -g main.cpp Schedule.o Coroutine.o -o main
clean:
	rm -rf *.o
