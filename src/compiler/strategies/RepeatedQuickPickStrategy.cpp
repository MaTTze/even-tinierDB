/*
 * QuickPickStrategy.cpp
 *
 *  Created on: June 24, 2013
 *      Author: jan
 */
#include "RepeatedQuickPickStrategy.hpp"
#include "QuickPickStrategy.hpp"
#include "../ast/JoinNode.hpp"
#include "../querygraph/Edge.hpp"
#include "../CostCalculator.hpp"
#include <iostream>


RepeatedQuickPickStrategy::RepeatedQuickPickStrategy() {}

RepeatedQuickPickStrategy::~RepeatedQuickPickStrategy() {
	// TODO Auto-generated destructor stub
}

ASTNode* RepeatedQuickPickStrategy::generateJoinTree(QueryGraph querygraph,	std::vector<ASTNode*>& relations) {
	ASTNode* n;
	ASTNode* best;
	double bestCost = std::numeric_limits<double>::max();
	std::map<double, unsigned> distribution;
	QuickPickStrategy s = QuickPickStrategy();
	for (unsigned i = 0; i < repetitions; i++){
		n = s.generateJoinTree(querygraph, relations);
		double cost = CostCalculator::getCosts(n, &querygraph);
		distribution[cost]++;
		if (cost < bestCost){
			best = n;
			bestCost = cost;
		}
	}
	for (auto it = distribution.begin(); it != distribution.end(); it++){
		std::cout << "cost: " << it->first << " occurrences: " << it->second << std::endl;
	}
	return best;
}

