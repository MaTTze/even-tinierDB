/*
 * TreeGenerator.cpp
 *
 *  Created on: June 19, 2013
 *      Author: jan
 */
#include "TreeGenerator.hpp"
#include "DyckGenerator.hpp"
#include "../ast/JoinNode.hpp"
#include <iostream>
#include <list>
#include <math.h>

TreeGenerator::TreeGenerator(std::vector<ASTNode*>& rels, QueryGraph* qg) {
	relations = rels;
	querygraph = qg;
	std::random_device rd;
	mt.seed(rd());

	//TODO:not sure if dists will get set properly.
	std::uniform_int_distribution<unsigned> distStructure(0, (unsigned)DyckGenerator::getCatalanNumber(relations.size()-1));		//dirty cast. Maybe rethink ULL usage in DyckGenerator.
	std::uniform_int_distribution<unsigned> distPermutation(0, factorial(relations.size()));
}

ASTNode* TreeGenerator::generateRandomTree() {
	return generateTree(distStructure(mt), distPermutation(mt));
}


ASTNode* TreeGenerator::generateTree(unsigned rankStructure, unsigned rankPermutation) {
 	//Get specified tree structure and permutation
 	std::list<int> randomStructure = DyckGenerator::generateWord(relations.size()-1, rankStructure);		//relations.size()-1 == amount of inner nodes
 	std::vector<ASTNode*> randomPermutation = getPermutation(rankPermutation);

 	//combine them to a join tree
 	ASTNode* root;
 	for(unsigned i = 0; i < relations.size()-1; i++) {
 		//JoinNode* n = JoinNode()
 	}


 	return relations.at(0);
}

std::vector<ASTNode*> TreeGenerator::getPermutation(unsigned rank) {
	std::vector<ASTNode*> result;
	ASTNode* tmp;
	for(unsigned i = relations.size(); i > 0; i++) {
		tmp = relations.at(i-1);
		result.at(i-1) = relations.at(rank % i);
		result.at(rank % i) = tmp;
		rank = floor((double)rank / i);
	}

	return result;
}

unsigned factorial(unsigned n) {
	if(n == 1 || n == 0)
		return 1;
	return factorial(n-1) * n;
}