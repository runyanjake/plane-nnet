#include "nnet.h"
#include <time.h>

	//**** Helper Function Declarations ****

	//************************************************************************************************************

	NeuralNet::NeuralNet(int in, int hid, int out): num_inputs(in), num_hidden(hid), num_outputs(out){
		if(in != out){
			printf("ERROR: Network intended to have same input and output size to be self-feeding.\n");
			exit(-1);
		}

		printf("Network initializing with %d input nodes, %d hidden nodes, and %d output nodes.\n",
				num_inputs, num_hidden, num_outputs);

		srand(std::time(nullptr)); //seed clock with time elapsed since jan 1 1970

		//setup random node values
		int i,j;
		for (i = 0; i < in; ++i){ struct node tmp; tmp.node_val = (double)(rand() % 2); inputvals.push_back(tmp); }
		for (i = 0; i < hid; ++i){ struct node tmp; tmp.node_val = (double)(rand() % 2); hiddenvals.push_back(tmp); }
		for (i = 0; i < out; ++i){ struct node tmp; tmp.node_val = (double)(rand() % 2); outputvals.push_back(tmp); }

		// setup random node connections
		for (i = 0; i < in; ++i){
			for(j=0; j< hid; ++j){
				double weight = (double)(rand()%(MAX_CONNECTION_VALUE - MIN_CONNECTION_VALUE) + MIN_CONNECTION_VALUE)/100.0;
				inputvals.at(i).weights.push_back(weight);
			}
		}
		for (i = 0; i < hid; ++i){
			for(j=0; j< out; ++j){
				double weight = (double)(rand()%(MAX_CONNECTION_VALUE - MIN_CONNECTION_VALUE) + MIN_CONNECTION_VALUE)/100.0;
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
		int i;
		//copy values from output to input
		for(i = 0; i < num_inputs; ++i){
			inputvals.at(i).node_val = outputvals.at(i).node_val;
		}
		//update hidden values
		for(i = 0; i < num_hidden; ++i){
			printf("\nInitial node value for hidden %d is %f\n", i, hiddenvals.at(i).node_val);
			int numzeroes = 0;
			double total = 0;
			for(int j = 0; j < num_inputs; ++j){
				if(inputvals.at(j).node_val == 0) ++numzeroes;
				printf("\t%f * %f.\n", inputvals.at(j).node_val, inputvals.at(j).weights.at(i));
				total += inputvals.at(j).node_val * inputvals.at(j).weights.at(i);
				printf("\tFound input value of %f for hidden node %d\n", inputvals.at(j).weights.at(i), i);
			}
			printf("\tAverage is %f / %d =  %f\n", total, numzeroes, total / (num_inputs - numzeroes));
			if(numzeroes != num_inputs) 
				total /= (num_inputs - numzeroes); 
			//update hidden node values as a 0 or 1 
			if(total < 0.50){
				hiddenvals.at(i).node_val = 0.0;
				printf("Adjusted node value for hidden %d is %f\n", i, 0.0);
			}else{
				hiddenvals.at(i).node_val = 1.0;
				printf("Adjusted node value for hidden %d is %f\n", i, 1.0);
			}
		}
		//update output values
		for(i = 0; i < num_outputs; ++i){
			printf("\nInitial node value for output %d is %f\n", i, outputvals.at(i).node_val);
			int numzeroes = 0;
			double total = 0;
			for(int j = 0; j < num_hidden; ++j){
				if(hiddenvals.at(j).node_val == 0) ++numzeroes;
				printf("\t%f * %f.\n", hiddenvals.at(j).node_val, hiddenvals.at(j).weights.at(i));
				total += hiddenvals.at(j).node_val * hiddenvals.at(j).weights.at(i);
				printf("\tFound input value of %f for output node %d\n", hiddenvals.at(j).weights.at(i), i);
			}
			printf("\tAverage is %f / %d =  %f\n", total, numzeroes, total / (num_inputs - numzeroes));
			if(numzeroes != num_inputs)
				total /= (num_inputs - numzeroes);
			//update output node values as a 0 or 1 
			if(total < 0.50){
				outputvals.at(i).node_val = 0.0;
				printf("Adjusted node value for output %d is %f\n", i, 0.0);
			}else{
				outputvals.at(i).node_val = 1.0;
				printf("Adjusted node value for output %d is %f\n", i, 1.0);
			}
		}
	}

	int NeuralNet::evaluate(std::vector<double> solution){
		if((int)(solution.size()) != num_inputs){
			printf("ERROR: Solution size not equal to network size.\n");
			exit(-1);
		}
		int numcorrect = 0;

		for(int a = 0; a < num_outputs; ++a){
			if(outputvals.at(a).node_val == solution.at(a)){
				printf("Correct Node value.\n");
				++numcorrect;
			}else{
				printf("Incorrect Node value: %f vs %f.\n", outputvals.at(a).node_val, solution.at(a));
			}
		}

		return numcorrect;
	}




