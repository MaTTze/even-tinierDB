/*
 * GOOStrategy.cpp
 *
 *  Created on: May 29, 2013
 *      Author: matthias
 */

#include "GOOStrategy.hpp"
#include "../ast/ASTNode.hpp"
#include "../ast/JoinNode.hpp"
#include <vector>
#include <set>
#include <math.h>
#include <limits>
#include <tuple>

GOOStrategy::GOOStrategy() {
	// TODO Auto-generated constructor stub

}

GOOStrategy::~GOOStrategy() {
	// TODO Auto-generated destructor stub
}

ASTNode* GOOStrategy::generateJoinTree(QueryGraph querygraph,	std::vector<ASTNode*> relations) {
	std::vector<std::tuple<ASTNode*, std::set<unsigned>, double > > trees;
	JoinNode* n;
	for(unsigned i = 0; i < relations.size(); i++) {
		trees.push_back(std::make_tuple(relations.at(i), std::set<unsigned>({i}), querygraph.getNode(i)->getECardinality()));
	}
	while(trees.size() > 1) {
		double min_val = std::numeric_limits<double>::max();
		auto child1 = trees.begin();
		auto child2 = trees.begin();

		for(auto it = trees.begin(); it != --trees.end(); it++) {
			for(auto it2 = it; it2 != trees.end(); it2++) {
				double outputsize = std::get<2>(*it2) * std::get<2>(*it) * querygraph.evalSelectivity(std::get<1>(*it), std::get<1>(*it2));
				if(outputsize < min_val) {
					child1 = it;
					child2 = it2;
					min_val = outputsize;
				}
			}
		}

		n = new JoinNode(std::get<0>(*child1), std::get<0>(*child2));
		querygraph.addConditionsToJoin(n, std::get<1>(*child1), std::get<1>(*child2));

		std::set<unsigned> combinedRelations = std::get<1>(*child1);
		combinedRelations.insert(std::get<1>(*child2).begin(), std::get<1>(*child2).end());

		trees.erase(child1);
		trees.erase(child2);

		trees.push_back(std::make_tuple(n, combinedRelations, min_val));
	}

	return n;
}
