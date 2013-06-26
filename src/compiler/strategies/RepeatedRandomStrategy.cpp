/*
 * RepeatedRandomStrategy.cpp
 *
 *  Created on: June 24, 2013
 *      Author: jan
 */
#include "RepeatedRandomStrategy.hpp"
#include "../randomized/TreeGenerator.hpp"
#include "../ast/JoinNode.hpp"
#include "../querygraph/Edge.hpp"
#include "../CostCalculator.hpp"
#include <iostream>


RepeatedRandomStrategy::RepeatedRandomStrategy() {}

RepeatedRandomStrategy::~RepeatedRandomStrategy() {
	// TODO Auto-generated destructor stub
}

ASTNode* RepeatedRandomStrategy::generateJoinTree(QueryGraph querygraph,	std::vector<ASTNode*>& relations) {
	ASTNode* n;
	ASTNode* best;
	double bestCost = std::numeric_limits<double>::max();
	std::map<double, unsigned> distribution;
	TreeGenerator tg = TreeGenerator(relations, &querygraph);
	for (unsigned i = 0; i < repetitions; i++){
		n = tg.generateRandomTree();
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

