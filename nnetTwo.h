// $Id: nnetTwo.h $

#include <iostream>
#include <vector>

#define MIN_CONNECTION_VALUE -1.0 //PERCENTAGE
#define MAX_CONNECTION_VALUE 1.0 //PERCENTAGE

typedef struct nodeTwo {
	double node_val;
	std::vector<double> weights;
}nodeTwo;

class NeuralNetTwo {
private:
	int num_inputs, num_hidden, num_outputs;
	std::vector<nodeTwo> inputvals, hiddenvals, outputvals;
    double bias, reward, max_node_val, min_node_val, max_weight_val, min_weight_val=-1.0;

public:
    //Ctor
	NeuralNetTwo(int in, int hid, int out, double bi, double rwd, double max_node, double min_node);

    //Setter
    void setInputs(std::vector<std::string> values, char method);

    //Getters/Print Functions.
	void printNodes();
	void printNodesHumanReadable(std::vector<double> layer_nodes);
	void printWeights();
    void printDoubleVector(std::vector<double> vect);
    int numInputs(){ return num_inputs; }
	int numHiddens(){ return num_hidden; }
	int numOutputs(){ return num_outputs; }

    //Network Operations.
    double sigmoid(double x); //(helper)
	void forward_propagate();
	void backward_propagate();
    std::vector<double> create_answer();
    void evaluate_answer();

    //Reset if necessary.
	void needs_reset();
	void reset_network();

    //Logging utilities.
	void begin_log();
	FILE* begin_entries();
	void entry(int numcorrect, FILE* log, int log_width);
    void entry_percent_confidence(double acc_ratio, int log_width);
    void print_ho_weights_to_file(FILE* log, double iter);
	void finish_entries(FILE* closer);
	void finish_log();

};

typedef struct testResultTwo{
	int testsAttempted;
	int testsPassed;
	int testsFailed;
	double percentPassingRate;
	std::string visualPassingRate; //a progress bar style depiction of correctness, '[' 78x['*'|'_'] ']'
}testResultTwo;

class TesterTwo {
public:
	testResultTwo singleHoldoutTesting(NeuralNetTwo nnet, std::vector<std::vector<std::string>> data, char inputMethod);
};






