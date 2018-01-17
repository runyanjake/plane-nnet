// $Id: main.cpp,v 1.2 2018-01-14 20:34:56-07 - - $

#include <iostream>

#include "fileIO.h"
#include "nnet.h"

int main(int argc, char *argv[]){
	if(argc != 4){
		std::cout << "Usage: nnet [inputsize] [hiddensize] [outputsize]" << std::endl;
		exit(-1); //error
	}

	readLine("data/letter.data");

	NeuralNet network = NeuralNet::NeuralNet(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));

	network.printNodesOCRformat();
	//network.printWeights();

	// std::vector<double> solution;
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);
	// solution.push_back(0.0);
	// solution.push_back(1.0);

	//network.trainFor(10, solution);

	// network.train();
	// int numcorrect = network.evaluate(solution);
	// network.backpropagate(solution);
	// network.begin_log();
	// network.entry(numcorrect, 59);

	return 0;
}