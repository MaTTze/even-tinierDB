/*
 * DPsizeStrategy.cpp
 *
 *  Created on: June 11, 2013
 *      Author: jan
 */
#include "DPsizeStrategy.hpp"
#include "../ast/ASTNode.hpp"
#include "../ast/JoinNode.hpp"
#include <vector>
#include <set>
#include <math.h>
#include <limits>
#include <tuple>
#include <math.h>

DPsizeStrategy::DPsizeStrategy() {
	// TODO Auto-generated constructor stub

}

DPsizeStrategy::~DPsizeStrategy() {
	// TODO Auto-generated destructor stub
}

ASTNode* DPsizeStrategy::generateJoinTree(QueryGraph querygraph,	std::vector<ASTNode*>& relations) {
	std::cout << "Compiler: Running DPsize" << std::endl;

	
	return relations.at(0);
}
