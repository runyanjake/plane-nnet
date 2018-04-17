// $Id: nnet.h,v 1.2 2018-01-14 20:34:56-07 - - $

#include <iostream>
#include <vector>

#define MAX_CONNECTION_NUMBER 100 //may not be neeeded
#define MIN_CONNECTION_VALUE 5 //PERCENTAGE
#define MAX_CONNECTION_VALUE 95 //PERCENTAGE
#define MAX_NODE_VALUE 1.0 //MAXIMUM INTEGER VAL
#define MIN_NODE_VALUE 0.0 //MINIMUM INTEGER VAL
#define PUNISHMENT_FACTOR 0.90 
#define REWARD_FACTOR 1.15

typedef struct node {
	float node_val;
	std::vector<double> weights;
}node;

class NeuralNet {
private:
	int num_inputs, num_hidden, num_outputs;
	std::vector<node> inputvals, hiddenvals, outputvals;

	void setInputsFromSTFData(std::vector<std::string> data);

public:
	NeuralNet(int in, int hid, int out);

	int numInputs(){ return num_inputs; }
	int numHiddens(){ return num_hidden; }
	int numOutputs(){ return num_outputs; }

    void setInputs(std::vector<std::string> data, char method); //this need not be implemented now

	void printNodes();
	void printNodesOCRformat();
	void printWeights();

	int evaluate(std::vector<double> solution);
	void forwardpropagate();
	void backpropagate(std::vector<double> solution);
    double sigmoid(double x);

	void makeGuessFromInput(); //TODO, should produce multiple guesses with probability guesses

	void reseed_network_check();
	void reset_network();

	void begin_log();
	FILE* begin_entries();
	void entry(int numcorrect, FILE* log, int log_width);
	void finish_entries(FILE* closer);
	void finish_log();


    void debugTest(std::vector<std::vector<std::string>> data);


};

typedef struct testResult{
	int testsAttempted;
	int testsPassed;
	int testsFailed;
	double percentPassingRate;
	std::string visualPassingRate; //a progress bar style depiction of correctness, '[' 78x['*'|'_'] ']'
}testResult;

class Tester {
public:
	testResult singleHoldoutTesting(NeuralNet nnet, std::vector<std::vector<std::string>> data, char inputMethod);
};






