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

//Prints a vector of doubles in human readable form
void NeuralNet::printDoubleVector(std::vector<double> vect){
    printf("[");
    for(unsigned long a=0;a<vect.size();++a){
        printf("%f ", vect.at(a));
    }
    printf("]\n");
}

//evaluates right or wrongness of output
//correct answer is 0, all others are naked value difference.
std::vector<double> NeuralNet::getCorrectValueOffsets(char solution){
    std::vector<double> correctionvals;
	double maxval = -1.0;
    int index = -1;
    for(int a=0;a<numOutputs();++a){
        if(outputvals.at(a).node_val > maxval){
            maxval = outputvals.at(a).node_val;
            index = a;
        }
    }
    printf("\ngetCorrectValueOffsets with answer %c: [", solution);
    for(int a=0;a<numOutputs();++a){
        double correctionvalue = outputvals.at(a).node_val - (outputvals.at(((int)solution)-97).node_val); //numeric value relative to correct value
        if(correctionvalue > MAX_CONNECTION_VALUE/100.0)
            correctionvalue = MAX_CONNECTION_VALUE/100.0;
        if(correctionvalue < MIN_CONNECTION_VALUE/100.0)
            correctionvalue = MIN_CONNECTION_VALUE/100.0;
        correctionvals.push_back(correctionvalue);
        printf("%f ", correctionvalue);
    }
    printf("\n");
    return correctionvals;
}

//returns true if network guesses the passed character, false if not
bool NeuralNet::evaluate(char solution){
    double maxval = -1.0;
    int index = -1;
    for(int a=0;a<numOutputs();++a){
        printf("Comparing %f and %f.\n", outputvals.at(a).node_val, maxval);
        if(outputvals.at(a).node_val > maxval){
            maxval = outputvals.at(a).node_val;
            index = a;
        }
    }
    printf("The network guesses %c. (index=%d)\n", index+97, index);
    if(index==((int)solution)-97)
        return true;
    return false;
}

// Passing values forward, modifies all hidden and output nodes
// Must have had data loaded into the input nodes for this to work.
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
    //2) update outputs (make guess as a PERCENTAGE of total)
    std::vector<double> rawvals;
    for(int a=0;a<numOutputs();++a){
        double total = 0.0;
        double max = 0.0;
        for(int b=0;b<numHiddens();++b){
            total += hiddenvals.at(b).weights.at(a) * hiddenvals.at(b).node_val;
            max += hiddenvals.at(b).weights.at(a) * MAX_NODE_VALUE;
        }
        double halfmax = max / 2;
        double newnodeval = ((sigmoid(total-halfmax)+1.0)/2.0) * MAX_NODE_VALUE; //this is the proposed new node value based on prev outputs
        rawvals.push_back(newnodeval);
    }
    //roll up rawvals to use for percent calcs
    double total = 0.0;
    for(int a=0;a<rawvals.size();++a){
        total += rawvals.at(a);
    }
    //put the percentage vals. TOTAL IS 1.0 = 100%
    for(int a=0;a<rawvals.size();++a){
        double percentval = rawvals.at(a) / total;
        outputvals.at(a).node_val = percentval;
    }
    
}

//Backpropagation
//Acts on a network that has been forward propagated. 
//Requires an "outputwills" vector holding the 'will' of output nodes to backpropagate.
void NeuralNet::backpropagate(std::vector<double> outputWills){
    //**** First do Secondary weights
    //propagate ONLY rewards
    for(int a=0;a<numOutputs();++a){
        if(outputWills.at(a) == MAX_REWARD_FACTOR){
            for(int b=0;b<numHiddens();++b){
                for(int c=0;c<numInputs();++c){
                    inputvals.at(c).weights.at(b) *= MAX_REWARD_FACTOR;
                }
            }
        }
    }
    //normalize between intended range
    double maxIHweight = inputvals.at(0).weights.at(0);
    double minIHweight = inputvals.at(0).weights.at(0);
    for(int b=0;b<numHiddens();++b){
        for(int c=1;c<numInputs();++c){
            if(inputvals.at(c).weights.at(b) > maxIHweight)
                maxIHweight = inputvals.at(c).weights.at(b);
            if(inputvals.at(c).weights.at(b) < minIHweight)
                minIHweight = inputvals.at(c).weights.at(b);
        }
    }
    for(int b=0;b<numHiddens();++b){
        for(int c=1;c<numInputs();++c){
            inputvals.at(c).weights.at(b) = ((((inputvals.at(c).weights.at(b) - minIHweight) * (MAX_CONNECTION_VALUE/100.0 - MIN_CONNECTION_VALUE/100.0)) / (maxIHweight - minIHweight)) + MIN_CONNECTION_VALUE/100.0);
        }
    }
    //**** Second do Primary weights
    //propagate ONLY rewards
    for(int a=0;a<numOutputs();++a){
        if(outputWills.at(a) == MAX_REWARD_FACTOR){
            for(int b=0;b<numHiddens();++b){
                hiddenvals.at(b).weights.at(a) *= MAX_REWARD_FACTOR;
            }
        }
    }
    //normalize between intended range
    double maxHOweight = hiddenvals.at(0).weights.at(0);
    double minHOweight = hiddenvals.at(0).weights.at(0);
    for(int b=0;b<numOutputs();++b){
        for(int c=1;c<numHiddens();++c){
            if(hiddenvals.at(c).weights.at(b) > maxHOweight)
                maxHOweight = hiddenvals.at(c).weights.at(b);
            if(hiddenvals.at(c).weights.at(b) < minHOweight)
                minHOweight = hiddenvals.at(c).weights.at(b);
        }
    }
    for(int b=0;b<numOutputs();++b){
        for(int c=1;c<numHiddens();++c){
            hiddenvals.at(c).weights.at(b) = ((((hiddenvals.at(c).weights.at(b) - minHOweight) * (MAX_CONNECTION_VALUE/100.0 - MIN_CONNECTION_VALUE/100.0)) / (maxHOweight - minHOweight)) + MIN_CONNECTION_VALUE/100.0);
        }
    }
}

//computes the fast sigmoid value of the input value.
// Domain: (-inf,+inf) Range: (-1,1)
double NeuralNet::sigmoid(double x){
    return x / (1.0 + std::abs(x));
}

//gets output wills, to be used in backpropagation
//These are output wills with one as MAX_REWARD_VALUE and the others between the 2 PUNISH values
std::vector<double> NeuralNet::getOutputWills(char answer){
    std::vector<double> correctValueOffsets = getCorrectValueOffsets(answer);
    double maxCorrectValueOffset = correctValueOffsets.at(0);
    int maxCorrectValueOffsetIndex = 0;
    double minCorrectValueOffset = correctValueOffsets.at(0);
    int minCorrectValueOffsetIndex = 0;
    for(int a=1;a<numOutputs();++a){
        if(std::abs(correctValueOffsets.at(a)) > maxCorrectValueOffset){
            maxCorrectValueOffset = std::abs(correctValueOffsets.at(a));
            maxCorrectValueOffsetIndex = a;
        }else if(std::abs(correctValueOffsets.at(a)) < minCorrectValueOffset && std::abs(correctValueOffsets.at(a)) > 0.0){
            minCorrectValueOffset = std::abs(correctValueOffsets.at(a));
            minCorrectValueOffsetIndex = a;
        }
    }
    std::vector<double> outputWills; //"Will" of a node to pass on its value. Between 0 and 100%
    double punishRange = maxCorrectValueOffset - minCorrectValueOffset;
    for(int a=0;a<numOutputs();++a){
        if(correctValueOffsets.at(a) != 0.0){
            double willFactor = ((correctValueOffsets.at(a) - minCorrectValueOffset) / punishRange) * MAX_PUNISHMENT_FACTOR;
            if(willFactor < MIN_PUNISHMENT_FACTOR){
                outputWills.push_back(MIN_PUNISHMENT_FACTOR);
            }else if(willFactor > MAX_PUNISHMENT_FACTOR){
                outputWills.push_back(MAX_PUNISHMENT_FACTOR);
            }else{
                outputWills.push_back(willFactor);
            }
        }else{
            outputWills.push_back(MAX_REWARD_FACTOR);
        }
    }
    return outputWills;
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
                    //2) determine correctness
                    nnet.setInputs(datum, inputMethod); //load data in with specified method
					nnet.forwardpropagate();
					char solution = datum.at(1).at(0); //returns char + nullplug
                    //3) punish/reward accordingly
                    nnet.backpropagate(nnet.getOutputWills(solution));
				}
                printf("%d/%d\n", ctr, data.size());
			}
            //4) load withheld value
            std::vector<std::string> heldout = data.at(holdoutIndex);
            //5) compute guess
            nnet.setInputs(heldout, inputMethod);
            nnet.forwardpropagate();
            //6) score based on the withheld value.
            printf("Network guessing on withheld value %d with correct value %c\n", holdoutIndex, heldout.at(1).at(0));
            bool correct = nnet.evaluate(heldout.at(0).at(0));
            if(correct) printf("The network guessed correctly.\n");
            if(!correct) printf("The network guessed incorrectly.\n");
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
    // double subtotals[numHiddens()];
    // for(int a=0;a<numHiddens();++a){
    //     double total = 0.0;
    //     double max = 0.0;
    //     for(int b=0;b<numInputs();++b){
    //         //printf("\t%f * %f = %f\n", inputvals.at(b).weights.at(a), inputvals.at(b).node_val, inputvals.at(b).weights.at(a) * inputvals.at(b).node_val);
    //         total += inputvals.at(b).weights.at(a) * inputvals.at(b).node_val;
    //         max += inputvals.at(b).weights.at(a) * MAX_NODE_VALUE;
    //     }
    //     double halfmax = max / 2;
    //     double newnodeval = ((sigmoid(total-halfmax)+1.0)/2.0) * MAX_NODE_VALUE; //this is the proposed new node value based on prev outputs
    //     printf("Total Input to Hidden node %d (%f) / Max Input (%f) => %f ", a, total, max, total-halfmax);
    //     printf(" > (sig(%f)+1)/2 * MAX_VAL = %f\n", total-halfmax, newnodeval);
    // }

    setInputs(data.at(0), 's');
    forwardpropagate();
    printNodes();
    bool correct = evaluate('f');
    if(correct) printf("The network guessed correctly.\n");
    if(!correct) printf("The network guessed incorrectly.\n");

    //then try backprop of data
    //2) 
    //2.1) evaluate desired amound of correction
    char answer = 'm';

    //holds numeric offset such that negative values indicate a need to add (basically the opposite)
    std::vector<double> correctValueOffsets = getCorrectValueOffsets(answer);

    //Find range of offsets (all that matters is the absolute max (and min))
    double maxCorrectValueOffset = correctValueOffsets.at(0);
    int maxCorrectValueOffsetIndex = 0;
    double minCorrectValueOffset = correctValueOffsets.at(0);
    int minCorrectValueOffsetIndex = 0;
    for(int a=1;a<numOutputs();++a){
        if(std::abs(correctValueOffsets.at(a)) > maxCorrectValueOffset){
            maxCorrectValueOffset = std::abs(correctValueOffsets.at(a));
            maxCorrectValueOffsetIndex = a;
        }else if(std::abs(correctValueOffsets.at(a)) < minCorrectValueOffset && std::abs(correctValueOffsets.at(a)) > 0.0){
            minCorrectValueOffset = std::abs(correctValueOffsets.at(a));
            minCorrectValueOffsetIndex = a;
        }
    }
    printf("Largest Delta (abs) From Correct: %f/%d. Smallest Delta (abs) From Correct : %f/%d.\n", maxCorrectValueOffset, maxCorrectValueOffsetIndex, minCorrectValueOffset, minCorrectValueOffsetIndex);

    //Scale Punishment factors according to network values
    //Between [MIN_PUNISHMENT_FACTOR, MAX_PUNISHMENT_FACTOR] with the 0'd value as MAX_REWARD_FACTOR
    std::vector<double> outputWills; //"Will" of a node to pass on its value. Between 0 and 100%
    double punishRange = maxCorrectValueOffset - minCorrectValueOffset;
    for(int a=0;a<numOutputs();++a){
        if(correctValueOffsets.at(a) != 0.0){
            double willFactor = ((correctValueOffsets.at(a) - minCorrectValueOffset) / punishRange) * MAX_PUNISHMENT_FACTOR;
            if(willFactor < MIN_PUNISHMENT_FACTOR){
                outputWills.push_back(MIN_PUNISHMENT_FACTOR);
            }else if(willFactor > MAX_PUNISHMENT_FACTOR){
                outputWills.push_back(MAX_PUNISHMENT_FACTOR);
            }else{
                outputWills.push_back(willFactor);
            }
        }else{
            outputWills.push_back(MAX_REWARD_FACTOR);
        }
    }

    printNodes();
    printf("OutputWills: ");
    printDoubleVector(outputWills);

    //****
    //**** This other (naiive?) way to do it: do rewards ONLY, and then scale everything between min and max weight values.
    //****

    //**** First do Secondary weights
    //propagate ONLY rewards
    for(int a=0;a<numOutputs();++a){
        if(outputWills.at(a) == MAX_REWARD_FACTOR){
            for(int b=0;b<numHiddens();++b){
                for(int c=0;c<numInputs();++c){
                    inputvals.at(c).weights.at(b) *= MAX_REWARD_FACTOR;
                }
            }
        }
    }
    //normalize between intended range
    double maxIHweight = inputvals.at(0).weights.at(0);
    double minIHweight = inputvals.at(0).weights.at(0);
    for(int b=0;b<numHiddens();++b){
        for(int c=1;c<numInputs();++c){
            if(inputvals.at(c).weights.at(b) > maxIHweight)
                maxIHweight = inputvals.at(c).weights.at(b);
            if(inputvals.at(c).weights.at(b) < minIHweight)
                minIHweight = inputvals.at(c).weights.at(b);
        }
    }
    printWeights();
    printf("Max weight is: %f Min weight is: %f\n", maxIHweight, minIHweight);
    for(int b=0;b<numHiddens();++b){
        for(int c=1;c<numInputs();++c){
            printf("Updating value %f to %f.\n", inputvals.at(c).weights.at(b), ((((inputvals.at(c).weights.at(b) - minIHweight) * (MAX_CONNECTION_VALUE - MIN_CONNECTION_VALUE)) / (maxIHweight - minIHweight)) + MIN_CONNECTION_VALUE) / 100.0);
            inputvals.at(c).weights.at(b) = ((((inputvals.at(c).weights.at(b) - minIHweight) * (MAX_CONNECTION_VALUE - MIN_CONNECTION_VALUE)) / (maxIHweight - minIHweight)) + MIN_CONNECTION_VALUE) / 100.0;
        }
    }

    //**** Second do Primary weights
    //propagate ONLY rewards
    for(int a=0;a<numOutputs();++a){
        if(outputWills.at(a) == MAX_REWARD_FACTOR){
            for(int b=0;b<numHiddens();++b){
                hiddenvals.at(b).weights.at(a) *= MAX_REWARD_FACTOR;
            }
        }
    }
    //normalize between intended range
    double maxHOweight = hiddenvals.at(0).weights.at(0);
    double minHOweight = hiddenvals.at(0).weights.at(0);
    for(int b=0;b<numOutputs();++b){
        for(int c=1;c<numHiddens();++c){
            if(hiddenvals.at(c).weights.at(b) > maxHOweight)
                maxHOweight = hiddenvals.at(c).weights.at(b);
            if(hiddenvals.at(c).weights.at(b) < minHOweight)
                minHOweight = hiddenvals.at(c).weights.at(b);
        }
    }
    printf("Max weight is: %f Min weight is: %f\n", maxHOweight, minHOweight);
    for(int b=0;b<numOutputs();++b){
        for(int c=1;c<numHiddens();++c){
            printf("Updating value %f to %f.\n", hiddenvals.at(c).weights.at(b), ((((hiddenvals.at(c).weights.at(b) - minHOweight) * (MAX_CONNECTION_VALUE - MIN_CONNECTION_VALUE)) / (maxHOweight - minHOweight)) + MIN_CONNECTION_VALUE) / 100.0);
            hiddenvals.at(c).weights.at(b) = ((((hiddenvals.at(c).weights.at(b) - minHOweight) * (MAX_CONNECTION_VALUE - MIN_CONNECTION_VALUE)) / (maxHOweight - minHOweight)) + MIN_CONNECTION_VALUE) / 100.0;
        }
    }


    //create and zero array for both primary and secondary weights
    // std::vector<std::vector<double>> hiddenPunishments;

    // for(int a=0;a<numInputs();++a){
    //     std::vector<double> tmp;
    //     for(int b=0;b<numHiddens();++b){
    //         tmp.push_back(0.0);
    //     }
    //     hiddenPunishments.push_back(tmp);
    // }


    //GoaL: obtain matrix of factors to multiply weight matrices by
    //Have: relative punishments such that punish(a) = value that all weights leading up to a's output should be multiplied by
    //What the vectors hold:
    //      getCorrectValueOffsets() -> distance from correct percentage weight thing (is absolute value only)
    //      
    //first order weights can be multiplied without storing, but their original weights need to be used to calculate second-tier modifications
    //proposed plan: do some sort of math to get raw hidden punishments, find max and range, then scale everything between MIN/MAX Punishments


    //going to try again a simple average of incoming punishments
    // for(int a=0;a<numOutputs();++a){
    //     for(int b=0;b<numHiddens();++b){
    //         for(int c=0;c<numInputs();++c){
    //             hiddenPunishments.at(c).at(b) += outputWills.at(a) * inputvals.at(c).weights.at(b);
    //         }
    //     }
    // }
    // for(int b=0;b<numHiddens();++b){
    //     for(int c=0;c<numInputs();++c){
    //         //hiddenPunishments.at(c).at(b) /= ;
    //     }
    // }

    //F


    // printf("\n\nPunishment Matrices For Weights:\n");
    // printDoubleVector(outputWills);
    // for(int a=0;a<numInputs();++a){
    //     printDoubleVector(hiddenPunishments.at(a));
    // }

}