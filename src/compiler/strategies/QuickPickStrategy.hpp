/*
 * QuickPickStrategy.hpp
 *
 *  Created on: June 24, 2013
 *      Author: jan
 */

#ifndef QUICKPICKSTRATEGY_HPP_
#define QUICKPICKSTRATEGY_HPP_
#include "../ast/ASTNode.hpp"
#include "../querygraph/QueryGraph.hpp"
#include "QuickPickTree.hpp"
#include "OrderStrategy.hpp"
#include <random>
#include <vector>

class QuickPickStrategy: public OrderStrategy {
public:
	QuickPickStrategy();
	virtual ~QuickPickStrategy();
	virtual ASTNode* generateJoinTree(QueryGraph, std::vector<ASTNode*>&);
private:
	std::mt19937 mt;
	std::vector<ASTNode*> relations;
};


#endif /* QUICKPICKSTRATEGY_HPP_ */
