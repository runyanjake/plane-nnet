// $Id: nnet.h,v 1.2 2018-01-14 20:34:56-07 - - $

#include <iostream>
#include <vector>

#define MAX_CONNECTION_NUMBER 100
#define MIN_CONNECTION_VALUE 5
#define MAX_CONNECTION_VALUE 95
#define MAX_NODE_VALUE 5.0 //range is maxNV-1 to minNV, should be whole, pos, ints
#define MIN_NODE_VALUE 1.0
#define PUNISHMENT_FACTOR 0.90
#define REWARD_FACTOR 1.15

typedef struct node {
	double node_val;
	std::vector<double> weights;
}node;

class NeuralNet {
private:
	int num_inputs, num_hidden, num_outputs;
	std::vector<node> inputvals, hiddenvals, outputvals;
public:
	NeuralNet(int in, int hid, int out);

	int getInputs(){ return num_inputs; }
	int getHidden(){ return num_hidden; }
	int getOutputs(){ return num_outputs; }

	void setInputsFromSTFData(std::vector<std::string> data);

	void printNodes();
	void printNodesOCRformat();
	void printWeights();

	void trainTo(std::vector<double> solution); //depr, redo?
	void trainFor(int itors, std::vector<double> solution); //depr, redo
	void train(); //depr, redo?

	void trainForward(std::vector<std::string> data);
	int evaluate(std::vector<double> solution);
	void backpropagate(std::vector<double> solution);

	void makeGuessFromInput(); //TODO, should produce multiple guesses with probability guesses

	void reseed_network_check();
	void reset_network();

	void begin_log();
	FILE* begin_entries();
	void entry(int numcorrect, FILE* log, int log_width);
	void finish_entries(FILE* closer);
	void finish_log();
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
private:
	void loadData(NeuralNet nnet, std::vector<std::string> data, char inputMethod);
};






