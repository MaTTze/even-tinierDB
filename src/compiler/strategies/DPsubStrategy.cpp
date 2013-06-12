/*
 * DPsubStrategy.cpp
 *
 *  Created on: June 11, 2013
 *      Author: jan
 */
#include "DPsubStrategy.hpp"
#include "../ast/ASTNode.hpp"
#include "../ast/JoinNode.hpp"
#include <vector>
#include <set>
#include <math.h>
#include <limits>
#include <tuple>
#include <math.h>

DPsubStrategy::DPsubStrategy() {
	// TODO Auto-generated constructor stub

}

DPsubStrategy::~DPsubStrategy() {
	// TODO Auto-generated destructor stub
}

ASTNode* DPsubStrategy::generateJoinTree(QueryGraph querygraph,	std::vector<ASTNode*>& relations) {
	bool crossproducts = false;
	std::cout << "Compiler: Running DPsub" << std::endl;

	amountRelations = relations.size();
	std::vector<std::set<ASTNode*> > dpTable(pow(2.0, (double)relations.size()));

	for(unsigned i = 0; i < relations.size(); i++) {
		dpTable.at(1 << i).insert(relations.at(i));
	}

	unsigned S, S1, S2;

	for(unsigned i = 2; i <= pow(2.0, (double)relations.size()) - 1; i++) {
		S = getCurrentSet(i);
		S1 = S&(-S);
		do {
			S2 = S - S1;
			// Do something with S1 and S2

			S1 = S&(S1 - S);
		} while (S1 != S);
	}

	return relations.at(0);
}

unsigned DPsubStrategy::getCurrentSet(unsigned i) {
	unsigned ret = 0;

	for(unsigned j = 1; j < amountRelations; j++) {
		if((unsigned)floor(i / pow(2.0, j - 1.0)) % 2 == 1)
			ret |= 1 << j;
	}

	return ret;
}
