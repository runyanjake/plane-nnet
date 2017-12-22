#include "nnet.h"
#include <time.h>

	//**** Helper Function Declarations ****

	//************************************************************************************************************

	NeuralNet::NeuralNet(int in, int hid, int out): num_inputs(in), num_hidden(hid), num_outputs(out){
		printf("Network initializing with %d input nodes, %d hidden nodes, and %d output nodes.\n",
				num_inputs, num_hidden, num_outputs);

		srand(std::time(nullptr)); //seed clock with time elapsed since jan 1 1970

		//setup random node values
		int i,j;
		for (i = 0; i < in; ++i){ struct node tmp; tmp.node_val = rand() % 2; inputvals.push_back(tmp); }
		for (i = 0; i < hid; ++i){ struct node tmp; tmp.node_val = rand() % 2; hiddenvals.push_back(tmp); }
		for (i = 0; i < out; ++i){ struct node tmp; tmp.node_val = rand() % 2; outputvals.push_back(tmp); }

		// setup random node connections
		for (i = 0; i < in; ++i){
			for(j=0; j< hid; ++j){
				double weight = (double)(rand()%190+5)/100.0;
				inputvals.at(i).weights.push_back(weight);
			}
		}
		for (i = 0; i < hid; ++i){
			for(j=0; j< out; ++j){
				double weight = (double)(rand()%190+5)/100.0;
				hiddenvals.at(i).weights.push_back(weight);
			}
		}
		printf("Network initialized.\n");
	}

	void NeuralNet::printNodes(){
		std::cout << "Input Layer Values:  [";
		for (int i = 0; i < num_inputs; ++i)
		{
			std::cout << " " << inputvals.at(i).node_val;
		}
		std::cout << " ]\n";
		std::cout << "Hidden Layer Values: [";
		for (int i = 0; i < num_hidden; ++i)
		{
			std::cout << " " << hiddenvals.at(i).node_val;
		}
		std::cout << " ]\n";
		std::cout << "Output Layer Values: [";
		for (int i = 0; i < num_outputs; ++i)
		{
			std::cout << " " << outputvals.at(i).node_val;
		}
		std::cout << " ]\n";
	}

	void NeuralNet::printWeights(){
		int i,j;
		printf("Input-Hidden Weights:\n");
		for (i = 0; i < num_inputs; ++i){
			for(j=0; j< num_hidden; ++j){
				printf("\tinput %d to hidden %d: %f\n", i, j, inputvals.at(i).weights.at(j));
			}
		}
		printf("Hidden-Output Weights:\n");
		for (i = 0; i < num_hidden; ++i){
			for(j=0; j< num_outputs; ++j){
				printf("\thidden %d to output %d: %f\n", i, j, hiddenvals.at(i).weights.at(j));
			}
		}
	}

	/*
	 * Copies the output values to the inputs and completes a training iteration.
	 * NOTE: DOES NOT PERFORM BACKPROPAGATION OR CORRECTNESS CHECKING
	 */
	void NeuralNet::train(){
		//copy values
		for(int i = 0; i < num_inputs; ++i){
			inputvals.at(i).node_val = outputvals.at(i).node_val;
		}
		//update hidden values
		
		//update output values

	}