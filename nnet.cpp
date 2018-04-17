// $Id: nnet.cpp,v 1.2 2018-01-14 20:34:56-07 - - $

#include "nnet.h"

#include <cmath> //round
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
	for (i = 0; i < in; ++i){ 
        struct node tmp; 
        tmp.node_val = round((((double)rand())/RAND_MAX)*(MAX_NODE_VALUE-MIN_NODE_VALUE) + MIN_NODE_VALUE); 
        inputvals.push_back(tmp); 
    }
	for (i = 0; i < hid; ++i){ 
        struct node tmp; 
        tmp.node_val = round((((double)rand())/RAND_MAX)*(MAX_NODE_VALUE-MIN_NODE_VALUE) + MIN_NODE_VALUE); 
        hiddenvals.push_back(tmp); 
    }
	for (i = 0; i < out; ++i){ 
        struct node tmp; 
        tmp.node_val = round((((double)rand())/RAND_MAX)*(MAX_NODE_VALUE-MIN_NODE_VALUE) + MIN_NODE_VALUE); 
        outputvals.push_back(tmp); 
    }
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
	printf("Done.\n");
}

void NeuralNet::setInputs(std::vector<std::string> data, char method = 'b'){
    switch(method){
        case 'b':
            break;
        case 's':
            setInputsFromSTFData(data);
            break;
        default:
            printf("loadData with unrecognized method.\n");
    }
}

void NeuralNet::setInputsFromSTFData(std::vector<std::string> data){
	if(num_inputs != 128) std::cerr << "Error: Number of inputs not set correctly for reading from the Stanford data." << std::endl;
	if(num_outputs != 26) std::cerr << "Error: Number of outputs not set correctly for reading from the Stanford data." << std::endl;
	//we ignore the first 6 inputs, and then read the remaining 128.
	for(int a = 6; a < 134; ++a){
		if((atoi(data.at(a).c_str())) == 1){
            inputvals.at(a-6).node_val = MAX_NODE_VALUE;
        }else{ 
            inputvals.at(a-6).node_val = MIN_NODE_VALUE;
        } 
	}
}

void NeuralNet::printNodes(){
	std::cout << "Input Layer Values:  [";
	for (int i = 0; i < num_inputs; ++i){ std::cout << " " << inputvals.at(i).node_val; }
	std::cout << " ]\nHidden Layer Values: [";
	for (int i = 0; i < num_hidden; ++i){ std::cout << " " << hiddenvals.at(i).node_val; }
	std::cout << " ]\nOutput Layer Values: [";
	for (int i = 0; i < num_outputs; ++i){ std::cout << " " << outputvals.at(i).node_val; }
	std::cout << " ]\n";
}

//PRINTS EITHER COLD OR HOT
void NeuralNet::printNodesOCRformat(){
	char hot = 35, cold = 46;
	std::cout << "hot: " << hot << " cold: " << cold << std::endl;
	if(num_inputs != 128 || num_outputs != 26){
		std::cout << "Incorrect Network Format for printing in the OCR format.\n";
		return;
	}
	std::cout << "Input Layer Values:[\n";
	for (int i = 0; i < num_inputs; ++i){
		if(inputvals.at(i).node_val == MIN_NODE_VALUE){
			std::cout << " " << cold;
		}else{
			std::cout << " " << hot;
        }
		if((i+1) % 8 == 0) std::cout << "\n";
	}
	std::cout << " ]\n";
	std::cout << "Output Layer Values: [";
	for (int i = 0; i < num_outputs; ++i){
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

//evaluates right or wrongness of output
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

// Passing values forward
// @param data A single input sized vector
void NeuralNet::forwardpropagate(){
    //1) update hiddens
    for(int a=0;a<numHiddens();++a){
        double total = 0.0;
        double max = 0.0;
        for(int b=0;b<numInputs();++b){
            total += inputvals.at(b).weights.at(a) * inputvals.at(b).node_val;
            max += inputvals.at(b).weights.at(a) * MAX_NODE_VALUE;
        }
        double halfmax = max / 2;
        double newnodeval = ((sigmoid(total-halfmax)+1.0)/2.0) * MAX_NODE_VALUE; //this is the proposed new node value based on prev outputs
        hiddenvals.at(a).node_val = newnodeval;
    }
    //2) update outputs (make guess)
    for(int a=0;a<numOutputs();++a){
        double total = 0.0;
        double max = 0.0;
        for(int b=0;b<numHiddens();++b){
            total += hiddenvals.at(b).weights.at(a) * hiddenvals.at(b).node_val;
            max += hiddenvals.at(b).weights.at(a) * MAX_NODE_VALUE;
        }
        double halfmax = max / 2;
        double newnodeval = ((sigmoid(total-halfmax)+1.0)/2.0) * MAX_NODE_VALUE; //this is the proposed new node value based on prev outputs
        outputvals.at(a).node_val = newnodeval;
    }
}

//Backpropagation
void NeuralNet::backpropagate(std::vector<double> solution){

}

//computes the fast sigmoid value of the input value.
// Domain: (-inf,+inf) Range: (-1,1)
double NeuralNet::sigmoid(double x){
    return x / (1.0 + abs(x));
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

//************************************************************************************************************

testResult Tester::singleHoldoutTesting(NeuralNet nnet, std::vector<std::vector<std::string>> data, char inputMethod = 'b'){
	std::cout << "Beginning Single Holdout testing with input method ";
	if(inputMethod == 'b'){ std::cout << "'basic'." << std::endl; }
	else if(inputMethod == 's'){ std::cout << "'stanford'" << std::endl; }
	else{ std::cerr << "[INPUT FORMAT NOT RECOGNIZED]" << std::endl; } 
	//Metrics
	testResult results = {-1, -1, -1, -1.0, "[]"}; //load into this at end
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
                    nnet.setInputs(datum, inputMethod); //load data in with specified method
					//\nnet.trainForward(datum); TODO
					std::string solution = datum.at(1); //returns char + nullplug
					//nnet.backpropagate(solution); string.at??
				}
                //2) determine correctness
                //3) punish/reward accordingly
			}
            //4) load withheld value
            //5) compute guess
            //6) score based on the withheld value.
            exit(0); //<**********************************************************************************######################
		}
	}else{
		std::cout << "Nothing was done." << std::endl;
	}
	return results;
}

void NeuralNet::debugTest(std::vector<std::vector<std::string>> data){
    printf("\n");
    printNodes();

    //try forward pass of data with first data point
    //1)
    double subtotals[numHiddens()];
    for(int a=0;a<numHiddens();++a){
        double total = 0.0;
        double max = 0.0;
        for(int b=0;b<numInputs();++b){
            //printf("\t%f * %f = %f\n", inputvals.at(b).weights.at(a), inputvals.at(b).node_val, inputvals.at(b).weights.at(a) * inputvals.at(b).node_val);
            total += inputvals.at(b).weights.at(a) * inputvals.at(b).node_val;
            max += inputvals.at(b).weights.at(a) * MAX_NODE_VALUE;
        }
        double halfmax = max / 2;
        double newnodeval = ((sigmoid(total-halfmax)+1.0)/2.0) * MAX_NODE_VALUE; //this is the proposed new node value based on prev outputs
        printf("Total Input to Hidden node %d (%f) / Max Input (%f) => %f ", a, total, max, total-halfmax);
        printf(" > (sig(%f)+1)/2 * MAX_VAL = %f\n", total-halfmax, newnodeval);
    }

    //then try backprop of data
    //2) 

}