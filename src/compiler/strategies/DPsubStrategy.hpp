/*
 * DPsubStrategy.hpp
 *
 *  Created on: June 11, 2013
 *      Author: jan
 */

#ifndef DPSUBSTRATEGY_HPP_
#define DPSUBSTRATEGY_HPP_

#include "OrderStrategy.hpp"

class DPsubStrategy: public OrderStrategy {
public:
	DPsubStrategy();
	virtual ~DPsubStrategy();
	virtual ASTNode* generateJoinTree(QueryGraph, std::vector<ASTNode*>&);

private:
	std::pair<ASTNode*, std::pair<double, double> > createJoinTree(std::pair<ASTNode*, std::pair<double, double> >, std::pair<ASTNode*, std::pair<double, double> >);
	void printSubsets(QueryGraph*, unsigned, unsigned);
	static const bool crossproducts = true;
};

#endif /* DPSUBSTRATEGY_HPP_ */