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
	unsigned amountRelations;
	unsigned getCurrentSet(unsigned);
};

#endif /* DPSUBSTRATEGY_HPP_ */
