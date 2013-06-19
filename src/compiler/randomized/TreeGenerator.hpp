/*
 * TreeGenerator.hpp
 *
 *  Created on: June 19, 2013
 *      Author: jan
 */

#ifndef TREEGENERATOR_HPP_
#define TREEGENERATOR_HPP_
#include "../ast/ASTNode.hpp"
#include "../querygraph/QueryGraph.hpp"
#include "Binomial.hpp"
#include <random>
#include <vector>
#include <list>

class TreeGenerator {
public:
	TreeGenerator(std::vector<ASTNode*>&, QueryGraph*);
	ASTNode* generateRandomTree();
	ASTNode* generateTree(unsigned, unsigned);

private:
	ASTNode* generateJoin(std::vector<int>, std::vector<ASTNode*>);
	std::vector<ASTNode*> getPermutation(unsigned);
	unsigned factorial(unsigned);
	
	std::uniform_int_distribution<unsigned> distStructure;
	std::uniform_int_distribution<unsigned> distPermutation;
	std::mt19937 mt;
	std::vector<ASTNode*> relations;
	QueryGraph* querygraph;
};


#endif /* TREEGENERATOR_HPP_ */
