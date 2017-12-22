#include "nnet.h"

	NeuralNet::NeuralNet(int in, int hid, int out): num_inputs(in), num_hidden(hid), num_outputs(out){
		printf("Network initializing with %d input nodes, %d hidden nodes, and %d output nodes.\n",
				num_inputs, num_hidden, num_outputs);

		//default values for matrix
		int i;
		for (i = 0; i < in; ++i){ 
			struct node tmp;
			tmp.node_val = rand() % 2; // 0 or 1
			inputvals.push_back(tmp);
		}
		for (i = 0; i < hid; ++i){ 
			struct node tmp;
			tmp.node_val = rand() % 2; // 0 or 1
			hiddenvals.push_back(tmp);
		}
		for (i = 0; i < out; ++i){ 
			struct node tmp;
			tmp.node_val = rand() % 2; // 0 or 1
			outputvals.push_back(tmp);
		}
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