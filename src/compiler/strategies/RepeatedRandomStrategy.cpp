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

	std::cout << "Compiler: Running repeated random strategy" << std::endl;


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

	std::cout << "-----------------------" << std::endl;
	std::cout << "Cost distribution:" << std::endl;
	for (auto it = distribution.begin(); it != distribution.end(); it++){
		printf("cost: %.2F | occurrences: %d\n", it->first, it->second); 
	}
	std::cout << "-----------------------" << std::endl;

	return best;
}

