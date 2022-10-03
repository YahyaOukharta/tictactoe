
all:
	g++ -std=c++14 main.cpp #-O -g -fsanitize=address

c: all
	#clear
	./a.out

perf : all
	sudo perf record -e instructions ./a.out
	sudo perf report