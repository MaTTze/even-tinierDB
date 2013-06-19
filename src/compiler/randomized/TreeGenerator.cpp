/*
 * TreeGenerator.cpp
 *
 *  Created on: June 19, 2013
 *      Author: jan
 */
#include "TreeGenerator.hpp"
#include "DyckGenerator.hpp"
#include "../ast/JoinNode.hpp"
#include "../ast/TablescanNode.hpp"
#include "../ast/SelectionNode.hpp"


#include <iostream>
#include <list>
#include <math.h>

TreeGenerator::TreeGenerator(std::vector<ASTNode*>& rels, QueryGraph* qg) {
	relations = rels;
	querygraph = qg;
	std::random_device rd;
	mt.seed(rd());

	std::uniform_int_distribution<unsigned> dist1(0, (unsigned)DyckGenerator::getCatalanNumber(relations.size()-1));		//dirty cast. Maybe rethink ULL usage in DyckGenerator.
	std::uniform_int_distribution<unsigned> dist2(0, factorial(relations.size()));

	distStructure = dist1; 
	distPermutation = dist2;
}

ASTNode* TreeGenerator::generateRandomTree() {
	return generateTree(distStructure(mt), distPermutation(mt));
}


ASTNode* TreeGenerator::generateTree(unsigned rankStructure, unsigned rankPermutation) {
 	//Get specified tree structure and permutation
 	std::vector<int> randomStructure = DyckGenerator::generateWord(relations.size()-1, rankStructure);		//relations.size()-1 == amount of inner nodes
 	std::vector<ASTNode*> randomPermutation = getPermutation(rankPermutation);

 	return generateJoin(randomStructure, randomPermutation);
}

ASTNode* TreeGenerator::generateJoin(std::vector<int> structure, std::vector<ASTNode*> permutation) {
	if(permutation.size() == 1)
		return permutation.at(0);
	if(structure.size() == 1)
		return dynamic_cast<ASTNode*>(new JoinNode(permutation.at(0), permutation.at(1)));

	std::vector<int> leftStructure, rightStructure;
	std::vector<ASTNode*> leftPermutation, rightPermutation;

	unsigned lastVal = 1;
	bool left = true;
	unsigned breakpoint = permutation.size();
	unsigned maxOpen = ((relations.size()-1)*2)-1;

	for(unsigned i = 0; i < structure.size(); i++) {
		unsigned diff = structure.at(i) - lastVal;
		if(left && (diff > i || structure.at(i) == maxOpen)) {
			left = false;
			breakpoint = i;
		}

		if(left && i != 0) 
			leftStructure.push_back(structure.at(i)-1);
		else if(!left)
			rightStructure.push_back(structure.at(i) - (structure.at(breakpoint) - 1));

		lastVal = structure.at(i);
	}

	for(unsigned i = 0; i < breakpoint; i++)
		leftPermutation.push_back(permutation.at(i));

	for(unsigned i = breakpoint; i < permutation.size(); i++)
		rightPermutation.push_back(permutation.at(i));

	if(breakpoint == permutation.size()) {
		rightPermutation.push_back(leftPermutation.back());
		leftPermutation.pop_back();
	}

	JoinNode* n = new JoinNode(generateJoin(leftStructure, leftPermutation), generateJoin(rightStructure, rightPermutation));
	std::set<unsigned> bindings1;
	for(auto it = leftPermutation.begin(); it != leftPermutation.end(); it++) {
		if(ASTNode::Type::Tablescan == (*it)->getType()) 
			bindings1.insert(dynamic_cast<TablescanNode*>(*it)->getRelation());
		else if(ASTNode::Type::Selection == (*it)->getType())
			bindings1.insert(dynamic_cast<SelectionNode*>(*it)->getBinding());
	}
	std::set<unsigned> bindings2;
	for(auto it = rightPermutation.begin(); it != rightPermutation.end(); it++) {
		if(ASTNode::Type::Tablescan == (*it)->getType())
			bindings2.insert(dynamic_cast<TablescanNode*>(*it)->getRelation());
		else if(ASTNode::Type::Selection == (*it)->getType())
			bindings2.insert(dynamic_cast<SelectionNode*>(*it)->getBinding());
	}
	querygraph->addConditionsToJoin(n, bindings1, bindings2);

	return dynamic_cast<ASTNode*>(n);
}

std::vector<ASTNode*> TreeGenerator::getPermutation(unsigned rank) {
	std::vector<ASTNode*> result = relations;
	ASTNode* tmp;

	for(unsigned i = result.size(); i > 0; i--) {
		tmp = result.at(i-1);

		result.at(i-1) = result.at(rank % i);

		result.at(rank % i) = tmp;

		rank = floor((double)rank / (double)i);
	}

	return result;
}

unsigned TreeGenerator::factorial(unsigned n) {
	if(n == 1 || n == 0)
		return 1;
	return factorial(n-1) * n;
}