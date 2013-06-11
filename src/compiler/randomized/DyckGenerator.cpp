/*
 * DyckGenerator.cpp
 *
 *  Created on: June 10, 2013
 *      Author: jan
 */
#include "DyckGenerator.hpp"
#include <iostream>
#include <math.h>

std::list<int> DyckGenerator::generateWord(int n, int r) {
 	int open = 1;
 	int close = 0;
 	int pos = 1;
 	std::list<int> encoding; 
 	Binomial b = Binomial(2*n);

 	while(open-1 < n) {
 		int k = DyckGenerator::q(&b,n, open+close, open-close);

 		if(k <= r) { 
 			r -= k;
 			++close;
 		} else {
 			encoding.push_back(pos);
 			++open;
 		}
 		++pos;
 	}

 	return encoding;

}

/*
	Prints a Dyck word given in list representation
*/
void DyckGenerator::printWord(std::list<int> encoding) {
	
	// Bracket Notation
	int x = 0;
	for(auto it = encoding.begin(); it != encoding.end(); it++) {
		for(int i = x+1; i < *(it); i++) {
			std::cout << ")";
		}
		std::cout << "(";
		x = *(it);
	}
	
	for(int i = x; i < encoding.size()*2; i++) 
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

	for(int i = x; i < encoding.size()*2; i++) 
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

int DyckGenerator::p(Binomial* b, int i, int j) {
	double factor = (((double)j+1)/((double)i+1));
	double ret = factor * (double)b->Choose((i+1),((i+j)/2 + 1));
	
	return (int)ret;
}

int DyckGenerator::q(Binomial* b, int n, int i, int j) {
	return DyckGenerator::p(b, 2*n - i, j);
}