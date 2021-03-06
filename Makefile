all: 
	g++ -std=gnu++14 -Wall -Wextra main.cpp nnet.cpp nnetTwo.cpp -o nnet

clean: 
	rm nnet log.txt *.o

run:
	./nnet 128 8 26

singletest:
	-@ make clean
	make
	make run

test:
	./nnet 3 4 3