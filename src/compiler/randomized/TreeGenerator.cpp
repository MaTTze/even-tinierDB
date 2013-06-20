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
	return generateTree(distStructure(mt), distPermutation(mt));		//call the generation of a specific tree with random (uniformly distributed) variables.
}


ASTNode* TreeGenerator::generateTree(unsigned rankStructure, unsigned rankPermutation) {
 	//Get specified tree structure and permutation
 	std::vector<int> randomStructure = DyckGenerator::generateWord(relations.size()-1, rankStructure);		//relations.size()-1 == amount of inner nodes
 	std::vector<ASTNode*> randomPermutation = getPermutation(rankPermutation);		//get specific permutation of relations

 	return generateJoin(randomStructure, randomPermutation);
}

/*
	Build the Join tree recursively. The idea is that we can see where in the structure list (DyckWord) the right subtree begins.
	The structure of the left sub tree is the overall structure minus the first entry up until the breakpoint. 
	The right structure therefore is the overall structure from the breakpoint to the overall end.
*/
ASTNode* TreeGenerator::generateJoin(std::vector<int> structure, std::vector<ASTNode*> permutation) {
	if(permutation.size() == 1)		//Only one Relation, append corresponding ASTNode to Tree
		return permutation.at(0);
	if(structure.size() == 1)		//Only one Join, append Join of both relations to Tree
		return dynamic_cast<ASTNode*>(new JoinNode(permutation.at(0), permutation.at(1)));

	std::vector<int> leftStructure, rightStructure;		//Store left and right joins
	std::vector<ASTNode*> leftPermutation, rightPermutation;	//Store corresponding relations

	unsigned lastVal = 1;		//Store last value of the structure list
	bool left = true;			//Are we handling the left side of the structure?
	unsigned breakpoint = permutation.size();		//At which point begins the right subtree in the structure?
	unsigned maxOpen = ((relations.size()-1)*2)-1;		//Last position at which a right subtree might occur

	for(unsigned i = 0; i < structure.size(); i++) {		//iterate over the DyckWord
		unsigned diff = structure.at(i) - lastVal;			//difference between current value in structure and last read
		if(left && (diff > i || structure.at(i) == maxOpen)) {		//if the difference is larger than the current index, all coming values in the structure list refer to the right subtree
			left = false;											//(in general only diff>i is interesting, others are special cases)
			breakpoint = i;
		}

		if(left && i != 0) 				//We are in left structure and insert this value decreased by one to the left structure (to be correct out of perspective of the sub tree) 
			leftStructure.push_back(structure.at(i)-1);				//(we omit the first entry, as it only belongs to the current tree and not to the sub trees)
		else if(!left)
			rightStructure.push_back(structure.at(i) - (structure.at(breakpoint) - 1));		//Decrease of value due to same reason as above. Decrease by breakpoint-1, as this results in the correct "perspective" fot the sub tree.

		lastVal = structure.at(i);
	}

	for(unsigned i = 0; i < breakpoint; i++)		//add correct amount of relations to the left sub tree
		leftPermutation.push_back(permutation.at(i));

	for(unsigned i = breakpoint; i < permutation.size(); i++)	//add correct amount of relations to the right subtree.
		rightPermutation.push_back(permutation.at(i));

	if(breakpoint == permutation.size()) {		//most likely legacy code. Too brainfucked right now to verify. Does no harm.
		rightPermutation.push_back(leftPermutation.back());
		leftPermutation.pop_back();
	}

	JoinNode* n = new JoinNode(generateJoin(leftStructure, leftPermutation), generateJoin(rightStructure, rightPermutation));
	addConditionsToJoin(n, querygraph, leftPermutation, rightPermutation);		//outsourced, as conversion to correct datatype is quite ugly.

	return dynamic_cast<ASTNode*>(n);
}

//Unrank Permutation as shown in the lecture.
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

void TreeGenerator::addConditionsToJoin(JoinNode* n, QueryGraph* qg, std::vector<ASTNode*> leftPermutation, std::vector<ASTNode*> rightPermutation) {
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
}

