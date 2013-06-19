/*
 * QuickPickStrategy.cpp
 *
 *  Created on: June 19, 2013
 *      Author: jan
 */
#include "QuickPickStrategy.hpp"
#include "../ast/ASTNode.hpp"
#include "../ast/JoinNode.hpp"
#include "../ast/ASTPrinter.hpp"
#include "../randomized/DyckGenerator.hpp"
#include "../randomized/TreeGenerator.hpp"
#include <vector>
#include <set>
#include <math.h>
#include <limits>
#include <tuple>
#include <math.h>
#include <string>

QuickPickStrategy::QuickPickStrategy() {
	// TODO Auto-generated constructor stub

}

QuickPickStrategy::~QuickPickStrategy() {
	// TODO Auto-generated destructor stub
}

ASTNode* QuickPickStrategy::generateJoinTree(QueryGraph querygraph,	std::vector<ASTNode*>& rels) {
	
	std::cout << "Compiler: Running Quickpick" << std::endl;

	TreeGenerator tg = TreeGenerator(rels, &querygraph);

	return tg.generateRandomTree();		//return root of best tree for complete set
}

std::pair<ASTNode*, std::pair<double, double> > QuickPickStrategy::createJoinTree(
		std::pair<ASTNode*, std::pair<double, double> > tree1,
		std::pair<ASTNode*, std::pair<double, double> > tree2) {
	//compare hashjoin costs (1.2 is irrelevant)
	if (tree1.second.first > tree2.second.first){
		return std::make_pair(new JoinNode(tree2.first, tree1.first), std::make_pair(0, tree2.second.second*1.2));//set size afterwards
	} else {
		return std::make_pair(new JoinNode(tree1.first, tree2.first), std::make_pair(0, tree1.second.second*1.2));//set size afterwards
	}
}

std::vector<ASTNode*> QuickPickStrategy::getPermutation(std::vector<ASTNode*>& list, unsigned rank) {
	std::vector<ASTNode*> relations;
	ASTNode* tmp;
	for(unsigned i = list.size(); i > 0; i++) {
		tmp = list.at(i-1);
		relations.at(i-1) = list.at(rank % i);
		relations.at(rank % i) = tmp;
		rank = floor((double)rank / i);
	}

	return relations;
}