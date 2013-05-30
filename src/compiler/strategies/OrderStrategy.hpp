/*
 * OrderStrategy.hpp
 *
 *  Created on: May 29, 2013
 *      Author: matthias
 */

#ifndef ORDERSTRATEGY_HPP_
#define ORDERSTRATEGY_HPP_

#include "../ast/ASTNode.hpp"
#include "../querygraph/QueryGraph.hpp"

class OrderStrategy {
public:
	OrderStrategy();
	virtual ~OrderStrategy();
	virtual ASTNode* generateJoinTree(QueryGraph, std::vector<ASTNode*>&) = 0;
};

#endif /* ORDERSTRATEGY_HPP_ */
