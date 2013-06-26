/*
 * RepeatedRandomStrategy.hpp
 *
 *  Created on: June 26, 2013
 *      Author: jan
 */

#ifndef REPEATEDRANDOMSTRATEGY_HPP_
#define REPEATEDRANDOMSTRATEGY_HPP_
#include "../ast/ASTNode.hpp"
#include "../querygraph/QueryGraph.hpp"
#include "QuickPickTree.hpp"
#include "OrderStrategy.hpp"
#include <random>
#include <vector>

class RepeatedRandomStrategy: public OrderStrategy {
public:
	RepeatedRandomStrategy();
	virtual ~RepeatedRandomStrategy();
	virtual ASTNode* generateJoinTree(QueryGraph, std::vector<ASTNode*>&);
private:
	static const unsigned repetitions = 100;
};


#endif /* REPEATEDRANDOMSTRATEGY_HPP_ */
