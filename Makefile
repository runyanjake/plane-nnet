all: 
	g++ -std=gnu++14 -Wall -Wextra main.cpp nnet.cpp -o nnet

clean: 
	rm nnet log.txt *.o

run:
	./nnet 8 10 8