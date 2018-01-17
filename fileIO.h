// $Id: fileIO.h,v 1.2 2018-01-14 20:34:56-07 - - $

//#include <string.h>

#include <vector>
#include <fstream>
#include <sstream>

std::vector<std::vector<std::string>> readLine(std::string filename){
	std::vector<std::vector<std::string>> ans; //length depends, can be char/int/string.
	
	std::ifstream in = std::ifstream(filename, std::ifstream::in);
	int linenr = 0, wordnr = 0;
	//read in
	while(in.good()){
		wordnr = 0;
		std::string input;
		std::getline(in, input, '\n'); //newline deliniate then

		if(input.compare("") == 0) break; //no empty lines.

		std::vector<std::string> lineentry;

		std::cout << "Processing line " << (linenr+1) <<": " << input << std::endl;

		char* line = (char*)input.c_str();	
		char* word = strtok(line, "	");
		while(word != NULL){
			++wordnr;

			//std::cout << "\t" << wordnr << ": " << word << std::endl;
			//std::cout << linenr << ": " << word << std::endl;
			lineentry.push_back(word);

			word = strtok(NULL, "	");
		}

		ans.push_back(lineentry);
		++linenr;
	}

	std::cout << "size of line list: " << ans.size() << std::endl;
	for(int a = 0; ; ++a){
		std::cout << "\tans[0][" << a << "]: " << ans.at(0).at(a) << std::endl;
	}

	return ans;
}