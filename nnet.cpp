// $Id: nnet.cpp,v 1.2 2018-01-14 20:34:56-07 - - $

#include "nnet.h"

#include <time.h>
#include <cstdlib> //atoi

//**** Helper Function Declarations ****

//************************************************************************************************************

NeuralNet::NeuralNet(int in, int hid, int out): num_inputs(in), num_hidden(hid), num_outputs(out){
	printf("Network initializing with %d input nodes, %d hidden nodes, and %d output nodes.\n",
			num_inputs, num_hidden, num_outputs);

	srand(std::time(nullptr)); //seed clock with time elapsed since jan 1 1970

	//setup random node values
	int i,j;
	for (i = 0; i < in; ++i){ struct node tmp; tmp.node_val = (double)(rand() % (int)MAX_NODE_VALUE); inputvals.push_back(tmp); }
	for (i = 0; i < hid; ++i){ struct node tmp; tmp.node_val = (double)(rand() % (int)MAX_NODE_VALUE); hiddenvals.push_back(tmp); }
	for (i = 0; i < out; ++i){ struct node tmp; tmp.node_val = (double)(rand() % (int)MAX_NODE_VALUE); outputvals.push_back(tmp); }

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

void NeuralNet::setInputsFromSTFData(std::vector<std::string> data){
	if(num_inputs != 128) std::cerr << "Error: Number of inputs not set correctly for reading from the Stanford data." << std::endl;
	if(num_outputs != 26) std::cerr << "Error: Number of outputs not set correctly for reading from the Stanford data." << std::endl;
	//we ignore the first 6 inputs, and then read the remaining 128.
	for(int a = 6; a < 134; ++a){
        printf("%d ", atoi(data.at(a).c_str()));
		if((atoi(data.at(a).c_str())) == 1){
            inputvals.at(a-6).node_val = MAX_NODE_VALUE;
		}else{
			inputvals.at(a-6).node_val = MIN_NODE_VALUE;
		} 
	}
	printf("\n");
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

//PRINTS EITHER COLD OR HOT
void NeuralNet::printNodesOCRformat(){
	char hot = 35;
	char cold = 46;
	std::cout << "hot: " << hot << " cold: " << cold << std::endl;
	if(num_inputs != 128 || num_outputs != 26){
		std::cout << "Incorrect Network Format for printing in the OCR format.\n";
		return;
	}

	std::cout << "Input Layer Values:[\n";
	for (int i = 0; i < num_inputs; ++i)
	{
		if(inputvals.at(i).node_val == MIN_NODE_VALUE){
			std::cout << " " << cold;
		}else{
			std::cout << " " << hot;
		}
		
		if((i+1) % 8 == 0) std::cout << "\n";
	}
	std::cout << " ]\n";
	std::cout << "Output Layer Values: [";
	for (int i = 0; i < num_outputs; ++i)
	{
		if(outputvals.at(i).node_val == 0){
			std::cout << " " << cold;
		}else{
			std::cout << " " << hot;
		}
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

void NeuralNet::trainForward(std::vector<std::string> data){
	if(data.size() < (unsigned long)num_inputs) std::cerr << "Error: Training datum does not contain enough discrete values for the network." << std::endl;

}

int NeuralNet::evaluate(std::vector<double> solution){
	if((int)(solution.size()) != num_inputs){
		printf("ERROR: Solution size not equal to network size.\n");
		exit(-1);
	}
	int numcorrect = 0;

	for(int a = 0; a < num_outputs; ++a){
		if(outputvals.at(a).node_val == solution.at(a)){
			//printf("Correct Node value.\n");
			++numcorrect;
		}else{
			//printf("Incorrect Node value: %f vs %f.\n", outputvals.at(a).node_val, solution.at(a));
		}
	}
	return numcorrect;
}

//WILL NEED TO MAKE CHANGE TO CHARACTER SOLUTION
void NeuralNet::backpropagate(std::vector<double> solution){
	for(int i = 0; i < num_outputs; ++i){
		for(int j = 0; j < num_hidden; ++j){
			//change hidden-output weights
			if(outputvals.at(i).node_val == solution.at(i)){ //reward
				hiddenvals.at(j).weights.at(i) *= REWARD_FACTOR;
			}else{ //punish
				hiddenvals.at(j).weights.at(i) *= PUNISHMENT_FACTOR;
			}
		}
	}
	//for each output, propagate through the hidden and input's weights
	std::vector<double> hidden_correctness_vals;
	for(int k = 0; k < num_hidden; ++k){
		double avg = 0.0;
		int num_contributions = 0;
		for(int j = 0; j < num_outputs; ++j){
			//compute hidden correctness values
			if(outputvals.at(j).node_val == solution.at(j)){
				++num_contributions;
				avg += 1.0 * hiddenvals.at(k).weights.at(j);
			}
		}
		avg /= num_contributions;
		//printf("Hidden Layer %d correctness value (avg weight going to outputs): %f\n", k, avg);
		//change input-hidden weights
		for(int j = 0; j < num_inputs; ++j){
			if(avg < 0.50){
				inputvals.at(j).weights.at(k) *= (PUNISHMENT_FACTOR * avg);
			}else{
				inputvals.at(j).weights.at(k) *= (REWARD_FACTOR * avg);
			}
		}
	}
}

//reseeds network and weights, must have already been initialized/ 
void NeuralNet::reseed_network_check(){
	//check input layer for a full layer of zeroes which is a bad idea.
	bool has_nonzero = false;
	int a = 0;
	while(!has_nonzero && a < num_inputs){
		if(inputvals.at(a).node_val != 0) has_nonzero = true;
		++a;
	}
	if(!has_nonzero){ return; }
	int i,j;
	srand(std::time(nullptr)); //seed clock with time elapsed since jan 1 1970
	//node vals
	for(a = 0 ; a < num_inputs; ++a){
		inputvals.at(a).node_val = (double)(rand() % 2);
	}
	for(a = 0 ; a < num_hidden; ++a){
		hiddenvals.at(a).node_val = (double)(rand() % 2);	
	}
	for(a = 0 ; a < num_outputs; ++a){
		outputvals.at(a).node_val = (double)(rand() % 2);
	}
	//weight vals
	for (i = 0; i < num_inputs; ++i){
		for(j=0; j < num_hidden; ++j){
			double weight = (double)(rand()%(MAX_CONNECTION_VALUE - MIN_CONNECTION_VALUE) + MIN_CONNECTION_VALUE)/100.0;
			inputvals.at(i).weights.at(j) = weight;
		}
	}
	for (i = 0; i < num_hidden; ++i){
		for(j=0; j < num_outputs; ++j){
			double weight = (double)(rand()%(MAX_CONNECTION_VALUE - MIN_CONNECTION_VALUE) + MIN_CONNECTION_VALUE)/100.0;
			hiddenvals.at(i).weights.at(j) = weight;
		}
	}
	printf("after reseeding\n");
	printNodes();
}

void NeuralNet::reset_network(){
	if(num_inputs != 128) std::cerr << "Error: Number of inputs not set correctly for reading from the Stanford data." << std::endl;
	if(num_outputs != 26) std::cerr << "Error: Number of outputs not set correctly for reading from the Stanford data." << std::endl;
	int a;
	srand(std::time(nullptr)); //seed clock with time elapsed since jan 1 1970
	for(a=0; a<num_inputs; ++a){
		inputvals.at(a).node_val = 0;
		for(int b=0; b<num_hidden; ++b){
			inputvals.at(a).weights.at(b) = (double)(rand()%(MAX_CONNECTION_VALUE - MIN_CONNECTION_VALUE) + MIN_CONNECTION_VALUE)/100.0;
		}
	}
	for(a=0; a<num_hidden; ++a){
		hiddenvals.at(a).node_val = 0;
		for(int b=0; b<num_outputs; ++b){
			hiddenvals.at(a).weights.at(b) = (double)(rand()%(MAX_CONNECTION_VALUE - MIN_CONNECTION_VALUE) + MIN_CONNECTION_VALUE)/100.0;
		}
	}
	for(a=0; a<num_outputs; ++a){
		outputvals.at(a).node_val = 0;
	}
}

void NeuralNet::begin_log(){
	FILE* log = fopen("log.txt", "a");
	time_t timer;
	long deltat = std::time(&timer);
	long secs = deltat % 60;
	long mins = (int)(deltat/60)%60;
	long hrs = (int)(deltat/3600)%24;
	fprintf(log, "\n******************* Log Entry at %ld:%ld.%ld ********************\n", hrs, mins, secs);
	fclose(log);
}

FILE* NeuralNet::begin_entries(){
	return fopen("log.txt", "a");
}

void NeuralNet::entry(int numcorrect, FILE* log, int log_width){
	double acc_ratio = (double)numcorrect / (double)num_outputs;
	int numticks = (int)((double)log_width * acc_ratio);
	fprintf(log, "[");
	for(int a = 0; a < numticks; ++a) fprintf(log, "=");
	for(int a = 0; a < log_width - numticks; ++a) fprintf(log, " ");
	fprintf(log, "] Accuracy was %.2f percent.\n", acc_ratio*100);
}

void NeuralNet::finish_entries(FILE* closer){
	fclose(closer);
}

void NeuralNet::finish_log(){
	FILE* log = fopen("log.txt", "a");
	time_t timer;
	long deltat = std::time(&timer);
	long secs = deltat % 60;
	long mins = (int)(deltat/60)%60;
	long hrs = (int)(deltat/3600)%24;
	fprintf(log, "****************** Log Complete at %ld:%ld.%ld *******************\n", hrs, mins, secs);
	fclose(log);
}

testResult Tester::singleHoldoutTesting(NeuralNet nnet, std::vector<std::vector<std::string>> data, char inputMethod = 'b'){
	std::cout << "Beginning Single Holdout testing with input method ";
	if(inputMethod == 'b'){ std::cout << "'basic'." << std::endl; }
	else if(inputMethod == 's'){ std::cout << "'stanford'" << std::endl; }
	else{ std::cerr << "[INPUT FORMAT NOT RECOGNIZED]" << std::endl; } 

	//Metrics
	testResult results = {-1, -1, -1, -1.0, "[]"};
	int numAttempted = 0;
	int numPassed = 0;
	int numFailed = 0;
	double passingRate = 0.0;

	if(inputMethod == 'b'){
		//another method!!!
	}else if(inputMethod == 's'){
		for(unsigned long holdoutIndex = 0; holdoutIndex < data.size(); ++holdoutIndex){
			std::cout << "Training with index " << holdoutIndex << " withheld... \n";
			//1) ZERO OUT NETOWRK VALUES FOR EACH TRIAL
			for(unsigned long ctr = 0; ctr < data.size(); ++ctr){
				std::vector<std::string> datum = data.at(ctr);
				if(ctr != holdoutIndex){
					nnet.printNodes();
					nnet.setInputsFromSTFData(datum); //this works
                    //loadData(nnet, datum, inputMethod); //this doesn't
					nnet.printNodes();
                    exit(0);
					//nnet.trainForward(datum); TODO
					std::string solution = datum.at(1); //returns char + nullplug
					//nnet.backpropagate(solution); string.at??
				}
			}
			//2) determine correctness
			//3) punish/reward accordingly
		}
		//4) load withheld value
		//5) compute guess
		//6) score based on the withheld value.
	}else{
		std::cout << "Nothing was done." << std::endl;
	}
	return results;
}

void Tester::loadData(NeuralNet nnet, std::vector<std::string> data, char inputMethod){
    // printf("Processing size %d datum with mode %c\n", data.size(), inputMethod);
    // nnet.setInputsFromSTFData(data);
    // switch(inputMethod){
    //     case 'b':
    //         printf("loadData with method b.\n");
    //         break;
    //     case 's':
    //         printf("loadData with method stanford.\n");
    //         nnet.setInputsFromSTFData(data);
    //         break;
    //     default:
    //         printf("loadData with unrecognized method.\n");
    // }
}

