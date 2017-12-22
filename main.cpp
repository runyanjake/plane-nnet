#include <iostream>
#include "nnet.h"

int main(int argc, char *argv[]){
	if(argc != 4){
		std::cout << "Usage: nnet [inputsize] [hiddensize] [outputsize]" << std::endl;
		exit(-1); //error
	}

	NeuralNet network = NeuralNet::NeuralNet(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));

	network.printNodes();

	std::cout << "Network dimensions: " << network.getInputs() << " input nodes, " << network.getHidden() << " hidden nodes, " << network.getOutputs() << " output nodes." << std::endl;
	return 0;
}