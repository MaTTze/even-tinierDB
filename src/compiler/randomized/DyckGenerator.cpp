/*
 * DyckGenerator.cpp
 *
 *  Created on: June 10, 2013
 *      Author: jan
 */
#include "DyckGenerator.hpp"
#include <iostream>

/*
	Generates the Dyck word for the tree with n inner nodes and rank r.
	Returns list representation of the Dyck word.
	>>  Note: not sure if working for n > 33  <<
*/
std::vector<int> DyckGenerator::generateWord(int n, int r) {
 	int open = 1;
 	int close = 0;
 	int pos = 1;
 	std::vector<int> encoding; 
 	Binomial b = Binomial(2*n);		//class for calculating binomial coefficients

 	while(open-1 < n) {		// open-1 is equal to encoding.size()
 		unsigned long long k = DyckGenerator::q(&b,n, open+close, open-close);		//calculate number of different paths from here

 		if(k <= r) { 		
 			r -= k;			//closing bracket found. decrease r, increase count of closing brackets.
 			++close;
 		} else {
 			encoding.push_back(pos);	//opening bracket found. Append position to list.
 			++open;
 		}
 		++pos;		//next position
 	}

 	return encoding;
}

/*
	Prints a Dyck word given in list representation
*/
void DyckGenerator::printWord(std::vector<int> encoding) {
	
	// Bracket Notation
	int x = 0;
	for(auto it = encoding.begin(); it != encoding.end(); it++) {
		for(int i = x+1; i < *(it); i++) {
			std::cout << ")";
		}
		std::cout << "(";
		x = *(it);
	}
	
	for(unsigned i = x; i < encoding.size()*2; i++) 
		std::cout << ")";

	std::cout << std::endl;

	// Binary String
	x = 0;
	for(auto it = encoding.begin(); it != encoding.end(); it++) {
		for(int i = x+1; i < *(it); i++) {
			std::cout << "0";
		}
		std::cout << "1";
		x = *(it);
	}

	for(unsigned i = x; i < encoding.size()*2; i++) 
		std::cout << "0";

	std::cout << std::endl;

	// List Notation
	for(auto it = encoding.begin(); it != encoding.end(); it++) {
		std::cout << *(it);
		if(it != --encoding.end())
			std::cout << ", ";
	}
	std::cout << std::endl;
}

unsigned long long DyckGenerator::getCatalanNumber(int n) {
	Binomial b = Binomial(5*n);

	return b.Choose(2*n, n) / (n+1.0);
}

unsigned long long DyckGenerator::p(Binomial* b, int i, int j) {
	double factor = (j + 1.0) / (i + 1.0);
	
	return factor * b->Choose((i+1),((i+j)/2 + 1));
}

unsigned long long DyckGenerator::q(Binomial* b, int n, int i, int j) {
	return DyckGenerator::p(b, 2*n - i, j);
}