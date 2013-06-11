/*
 * DPsizeStrategy.hpp
 *
 *  Created on: June 11, 2013
 *      Author: jan
 */

#ifndef DPSIZESTRATEGY_HPP_
#define DPSIZESTRATEGY_HPP_

#include "OrderStrategy.hpp"

class DPsizeStrategy: public OrderStrategy {
public:
	DPsizeStrategy();
	virtual ~DPsizeStrategy();
	virtual ASTNode* generateJoinTree(QueryGraph, std::vector<ASTNode*>&);

private:
};

#endif /* DPSIZESTRATEGY_HPP_ */
