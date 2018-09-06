all:
	g++ main.cpp -o main.o -c -pthread -std=c++11
	g++ PthreadPool.cpp -o PthreadPool.o -c -pthread -std=c++11
	g++ main.o PthreadPool.o -o main.exe -pthread -std=c++11 -g

clean:
	rm -rf *.o
	rm -rf *.exe
