/*
 * QuickPickStrategy.cpp
 *
 *  Created on: June 24, 2013
 *      Author: jan
 */
#include "QuickPickStrategy.hpp"
#include "QuickPickTree.hpp"
#include "../ast/JoinNode.hpp"
#include "../querygraph/Edge.hpp"
#include <iostream>


QuickPickStrategy::QuickPickStrategy() {
	std::random_device rd;
	mt.seed(rd());
}

QuickPickStrategy::~QuickPickStrategy() {
	// TODO Auto-generated destructor stub
}

ASTNode* QuickPickStrategy::generateJoinTree(QueryGraph querygraph,	std::vector<ASTNode*>& relations) {
	std::cout << "Compiler: Running Quick Pick" << std::endl;

	std::map<std::pair<unsigned, unsigned>, Edge*> edges_tmp = querygraph.getEdges();
	std::vector<std::pair<unsigned, unsigned> > edges;

	for(auto it = edges_tmp.begin(); it != edges_tmp.end(); it++) {
		edges.push_back(it->first);
	}

	QuickPickTree qpt = QuickPickTree(relations, &querygraph);
	std::pair<unsigned, unsigned> edge;
	ASTNode* n;
	unsigned rdmIndex;

	while(qpt.getTreeCount() > 1) {
		std::cout << qpt.getTreeCount() << std::endl;
		if (edges.size() == 0) {
			n = qpt.crossproductify();
			break;
		}
		std::uniform_int_distribution<unsigned> dist(0, edges.size()-1);
		rdmIndex = dist(mt);
		edge = edges.at(rdmIndex);
		edges.erase(edges.begin()+rdmIndex);

		n = qpt.unionTrees(edge.first, edge.second);
	}

	return n;
}

