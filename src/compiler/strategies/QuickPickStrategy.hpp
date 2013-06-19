/*
 * QuickPickStrategy.hpp
 *
 *  Created on: June 19, 2013
 *      Author: jan
 */

#ifndef QUICKPICKSTRATEGY_HPP_
#define QUICKPICKSTRATEGY_HPP_

#include "OrderStrategy.hpp"
#include <random>


class QuickPickStrategy: public OrderStrategy {
public:
	QuickPickStrategy();
	virtual ~QuickPickStrategy();
	virtual ASTNode* generateJoinTree(QueryGraph, std::vector<ASTNode*>&);

private:
	std::pair<ASTNode*, std::pair<double, double> > createJoinTree(std::pair<ASTNode*, std::pair<double, double> >, std::pair<ASTNode*, std::pair<double, double> >);
	std::vector<ASTNode*> getPermutation(std::vector<ASTNode*>&, unsigned);
};

#endif /* QUICKPICKSTRATEGY_HPP_ */