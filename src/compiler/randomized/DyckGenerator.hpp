/*
 * DyckGenerator.hpp
 *
 *  Created on: June 10, 2013
 *      Author: jan
 */

#ifndef DYCKGENERATOR_HPP_
#define DYCKGENERATOR_HPP_
#include <string>
#include <list>
#include "Binomial.hpp"

class DyckGenerator {
public:
	static std::list<int> generateWord(int, int);
	static void printWord(std::list<int>);

private:
	static int p(Binomial*, int, int);
	static int q(Binomial*, int, int, int);
};


#endif /* DYCKGENERATOR_HPP_ */
