/*
 * DPsubStrategy.cpp
 *
 *  Created on: June 11, 2013
 *      Author: jan
 */
#include "DPsubStrategy.hpp"
#include "../ast/ASTNode.hpp"
#include "../ast/JoinNode.hpp"
#include "../ast/ASTPrinter.hpp"
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

	std::cout << "Compiler: Running DPsub" << std::endl;

	std::vector<std::pair<ASTNode*, std::pair<double, double> > > dpTable(1 <<relations.size(), std::make_pair(nullptr, std::make_pair(0,0)));

	for(unsigned i = 0; i < relations.size(); i++) {
		dpTable.at(1 << i) = std::make_pair(relations.at(i), std::make_pair(querygraph.getNode(1)->getECardinality(),0));
	}

	unsigned i, i1, i2;
	std::set<unsigned> s1, s2;
	std::pair<ASTNode*, std::pair<double, double> > p1, p2, p;
	for(i = 2; i <= (1 << relations.size()) - 1; i++) {
		std::cout << i << std::endl;
		i1 = i&(-i);
		do {
			i2 = i - i1;
			std::cout << i1 << " - " << i2 << std::endl;
			s1 = querygraph.convertBitmapToSet(i1);
			s2 = querygraph.convertBitmapToSet(i2);
			if (!crossproducts && !querygraph.isConnected(s1,s2)) {
				i1 = i&(i1 - i);
				continue;
			}
			p1 = dpTable.at(i1);
			p2 = dpTable.at(i2);
			if ((p1.first == nullptr) || (p2.first == nullptr)){
				i1 = i&(i1 - i);
				continue;
			}
			p = createJoinTree(p1,p2);
			querygraph.addConditionsToJoin(dynamic_cast<JoinNode*>(p.first), s1, s2);
			p.second.first = querygraph.evalSelectivity(s1, s2)*p1.second.first*p2.second.first;
			if (dpTable.at(i).first == nullptr || dpTable.at(i).second.second > p.second.second) {
				dpTable.at(i) = p;
			}
			i1 = i&(i1 - i);
		} while (i1 != i);
	}
	for(i = 2; i <= (1 << relations.size()) - 1; i++) {
		std::cout << i << ": " << std::endl;
		if (dpTable.at(i).first == nullptr){
			std::cout << "null" << std::endl;
			continue;
		}
		ASTPrinter::print(dpTable.at(i).first);
	}
	return dpTable.at(--i).first;
}

std::pair<ASTNode*, std::pair<double, double> > DPsubStrategy::createJoinTree(
		std::pair<ASTNode*, std::pair<double, double> > tree1,
		std::pair<ASTNode*, std::pair<double, double> > tree2) {
	//compare hashjoin costs (1.2 is irrelevant)
	if (tree1.second.first > tree2.second.first){
		return std::make_pair(new JoinNode(tree2.first, tree1.first), std::make_pair(0, tree2.second.second*1.2));//set size afterwards
	} else {
		return std::make_pair(new JoinNode(tree1.first, tree2.first), std::make_pair(0, tree1.second.second*1.2));//set size afterwards
	}
}

