#include <iostream>
#include <vector>

#define MAX_CONNECTION_NUMBER 100
#define LOW_CONNECTION_VALUE 0.05
#define MAX_CONNECTION_VALUE 1.95

typedef struct node {
	int node_val;
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

	void printNodes();
	void printWeights();

	void trainTo(std::vector<node> solution); //size must equal number of inputs/outputs
	void trainFor(int itors);

	void train();
	void evaluate();
	void backpropagate();
};