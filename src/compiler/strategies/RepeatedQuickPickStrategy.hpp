/*
 * QuickPickStrategy.hpp
 *
 *  Created on: June 24, 2013
 *      Author: jan
 */

#ifndef REPEATEDQUICKPICKSTRATEGY_HPP_
#define REPEATEDQUICKPICKSTRATEGY_HPP_
#include "../ast/ASTNode.hpp"
#include "../querygraph/QueryGraph.hpp"
#include "QuickPickTree.hpp"
#include "OrderStrategy.hpp"
#include <random>
#include <vector>

class RepeatedQuickPickStrategy: public OrderStrategy {
public:
	RepeatedQuickPickStrategy();
	virtual ~RepeatedQuickPickStrategy();
	virtual ASTNode* generateJoinTree(QueryGraph, std::vector<ASTNode*>&);
private:
	static const unsigned repetitions = 10;
};


#endif /* REPEATEDQUICKPICKSTRATEGY_HPP_ */
