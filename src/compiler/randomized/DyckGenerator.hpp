/*
 * DyckGenerator.hpp
 *
 *  Created on: June 10, 2013
 *      Author: jan
 */

#ifndef DYCKGENERATOR_HPP_
#define DYCKGENERATOR_HPP_
#include <string>
#include <vector>
#include "Binomial.hpp"

class DyckGenerator {
public:
	static std::vector<int> generateWord(int, int);
	static void printWord(std::vector<int>);
	static unsigned long long getCatalanNumber(int);

private:
	static unsigned long long p(Binomial*, int, int);
	static unsigned long long q(Binomial*, int, int, int);
};


#endif /* DYCKGENERATOR_HPP_ */
