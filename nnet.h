// $Id: nnet.h,v 1.2 2018-01-14 20:34:56-07 - - $

#include <iostream>
#include <vector>

#define MAX_CONNECTION_NUMBER 100 //may not be neeeded
#define MIN_CONNECTION_VALUE 5 //PERCENTAGE
#define MAX_CONNECTION_VALUE 95 //PERCENTAGE
#define MAX_NODE_VALUE 5.0 //MAXIMUM INTEGER VAL
#define MIN_NODE_VALUE 0.0 //MINIMUM INTEGER VAL
#define MAX_REWARD_FACTOR 1.15 //highest value a weight can be modified by
#define MIN_REWARD_FACTOR 1.15 //delete?
#define MAX_PUNISHMENT_FACTOR 0.97 //highest value a weight can be modified by
#define MIN_PUNISHMENT_FACTOR 0.10 //lowest value a weight can be modified by
#define BIAS 0.01 //Bias applied to weighted sum during pass forward

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
    void printDoubleVector(std::vector<double> vect);

	std::vector<double> getCorrectValueOffsets(char solution);
    double getMaxOutputValue();
    bool evaluate(char solution);
	void forwardpropagate();
	void backpropagate(std::vector<double> outputWills);
    double sigmoid(double x);
    std::vector<double> getOutputWills(char answer);

	void makeGuessFromInput(); //TODO, should produce multiple guesses with probability guesses

	void reseed_network_check();
	void reset_network();

	void begin_log();
	FILE* begin_entries();
	void entry(int numcorrect, FILE* log, int log_width);
    void entry_percent_confidence(double acc_ratio, int log_width);
    void print_ho_weights_to_file(FILE* log, double iter);
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






