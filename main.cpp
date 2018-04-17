// $Id: main.cpp,v 1.2 2018-01-14 20:34:56-07 - - $

#include <iostream>
#include <iomanip> // std::setprecision

#include "fileIO.h"
#include "nnet.h"

int main(int argc, char *argv[]){
	if(argc != 4){
		std::cout << "Usage: nnet [inputsize] [hiddensize] [outputsize]" << std::endl;
		exit(-1);
	}
    
	/*Testing pipeline
	 * 	1. create network w default values
	 * 	2. for each input datum
	 * 		a. perform single holdout testing...
	 * 	3. test output
	 */

	//1. instantiate network
	NeuralNet network = NeuralNet::NeuralNet(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
	Tester tester;

	//2. load data
	std::vector<std::vector<std::string>> data = importData("data/letter.data");
    network.printWeights();
    network.printNodes();

	//3. perform testing
	testResult results = {0,0,0,0.0,""};
    //results = tester.singleHoldoutTesting(network, data, 's');

    //TESTING STUFF
    //network.debugTest(data);
    network.forwardpropagate();
    network.printNodes();

	//4. Communicate test results
	std::cout << "\n\nAll tests completed.\n" << results.testsAttempted <<
		" tests were attemted, and " << results.testsPassed << " yielded a successful classification.\n";
	std::cout << results.visualPassingRate << " (" << std::setprecision(4) //2 before and after decimal
		 << results.percentPassingRate << "\%)" << std::endl;

	return 0;
}