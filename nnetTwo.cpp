// $Id: nnetTwo.cpp
#include "nnetTwo.h"

NeuralNetTwo::NeuralNetTwo(int in, int hid, int out, double bi, double rwd, double max_node, double min_node): num_inputs(in), num_hidden(hid), num_outputs(out), bias(bi), reward(rwd), max_node_val(max_node), min_node_val(min_node){
    std::cout << "============================" << std::endl << "New Neural Net created:" << std::endl
        << "\tNum Inputs: " << in << std::endl
        << "\tNum Hiddens: " << hid << std::endl
        << "\tNum Outputs: " << out << std::endl
        << "\tBias: " << bi << std::endl
        << "\tReward Factor: " << rwd << std::endl
        << "\tMax Node Value: " << max_node << std::endl
        << "\tMin Node Value: " << min_node << std::endl
        << "=============================" << std::endl;

    std::cout << "Setting up nodes and weights... ";
    int i,j;
	for (i = 0; i < in; ++i){ 
        struct nodeTwo tmp; 
        tmp.node_val = min_node_val;
        inputvals.push_back(tmp); 
    }
	for (i = 0; i < hid; ++i){ 
        struct nodeTwo tmp; 
        tmp.node_val = min_node_val;
        hiddenvals.push_back(tmp); 
    }
	for (i = 0; i < out; ++i){ 
        struct nodeTwo tmp; 
        tmp.node_val = min_node_val;
        outputvals.push_back(tmp); 
    }
	// setup random node connections
	for (i = 0; i < in; ++i){
		for(j=0; j< hid; ++j){
			double weight = (MIN_CONNECTION_VALUE + MAX_CONNECTION_VALUE) / 2.0;
			inputvals.at(i).weights.push_back(weight);
		}
	}
	for (i = 0; i < hid; ++i){
		for(j=0; j< out; ++j){
			double weight = (MIN_CONNECTION_VALUE + MAX_CONNECTION_VALUE) / 2.0;
			hiddenvals.at(i).weights.push_back(weight);
		}
	}
	std::cout << "Done." << std::endl;
}

void NeuralNetTwo::setInputs(std::vector<std::string> values, char method = 'b'){
    switch(method){
        case 'b':
            if(num_inputs != values.size()) std::cerr << "Error: Number of inputs not set correctly for the supplied input values." << std::endl;
            for(int a = 0; a < values.size(); ++a){
                if((std::stod(values.at(a).c_str())) > max_node_val){
                    inputvals.at(a).node_val = max_node_val;
                }else if((std::stod(values.at(a).c_str())) < min_node_val){
                    inputvals.at(a).node_val = min_node_val;
                }else{ 
                    inputvals.at(a).node_val = (std::stod(values.at(a).c_str()));
                } 
            }
            break;
        case 's':
            if(num_inputs != 128) std::cerr << "Error: Number of inputs not set correctly for reading from the Stanford data." << std::endl;
            if(num_outputs != 26) std::cerr << "Error: Number of outputs not set correctly for reading from the Stanford data." << std::endl;
            //we ignore the first 6 inputs, and then read the remaining 128.
            for(int a = 6; a < 134; ++a){
                if((atoi(values.at(a).c_str())) == 1){
                    inputvals.at(a-6).node_val = max_node_val;
                }else{ 
                    inputvals.at(a-6).node_val = min_node_val;
                } 
            }
            break;
        default:
            printf("loadData with unrecognized method.\n");
    }
}

void NeuralNetTwo::printNodes(){
	std::cout << "Input Layer Values:  [";
	for (int i = 0; i < num_inputs; ++i){ std::cout << " " << inputvals.at(i).node_val; }
	std::cout << " ]\nHidden Layer Values: [";
	for (int i = 0; i < num_hidden; ++i){ std::cout << " " << hiddenvals.at(i).node_val; }
	std::cout << " ]\nOutput Layer Values: [";
	for (int i = 0; i < num_outputs; ++i){ std::cout << " " << outputvals.at(i).node_val; }
	std::cout << " ]\n";
}