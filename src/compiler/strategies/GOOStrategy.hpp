/*
 * GOOStrategy.hpp
 *
 *  Created on: May 29, 2013
 *      Author: matthias
 */

#ifndef GOOSTRATEGY_HPP_
#define GOOSTRATEGY_HPP_

#include "OrderStrategy.hpp"

class GOOStrategy: public OrderStrategy {
public:
	GOOStrategy();
	virtual ~GOOStrategy();
	virtual ASTNode* generateJoinTree(QueryGraph, std::vector<ASTNode*>&);

private:
};

#endif /* GOOSTRATEGY_HPP_ */
