// $Id: main.cpp,v 1.2 2018-01-14 20:34:56-07 - - $

#include <iostream>

#include "fileIO.h"
#include "nnet.h"

int main(int argc, char *argv[]){
	if(argc != 4){
		std::cout << "Usage: nnet [inputsize] [hiddensize] [outputsize]" << std::endl;
		exit(-1); //error
	}

	std::vector<std::vector<std::string>> data = importData("data/letter.data");

	NeuralNet network = NeuralNet::NeuralNet(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
	Tester tester;

	tester.singleHoldoutTesting(network, data, 's');

	return 0;
}