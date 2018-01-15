# plane-nnet
Plane program

Basic Nnet practice.

--Makefile Directives--
make 		- builds program
make clean	- cleans all generated files
make run	- executes the program with default values.
make singletest	- make clean > make > make run

**** UPDATE 1/14/2018 ****
Due to bad foresight, the original implementation of a n-bit guesser doesn't fit the train -> execute pipeline as it should to be considered a neural net.
So, the project will be now guided toward a OCR based program that learns to categorize n by n pixel images and classify them as a specific letter.

Future implementations of this could include word recognition but for now i'll start it with black and white single letter OCR.
