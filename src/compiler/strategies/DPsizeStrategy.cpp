/*
 * DPsizeStrategy.cpp
 *
 *  Created on: June 11, 2013
 *      Author: jan
 */
#include "DPsizeStrategy.hpp"
#include "../ast/ASTNode.hpp"
#include "../ast/JoinNode.hpp"
#include "../ast/ASTPrinter.hpp"
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

ASTNode* DPsizeStrategy::generateJoinTree(QueryGraph querygraph, std::vector<ASTNode*>& relations) {

	std::cout << "Compiler: Running DPsize" << std::endl;

	std::vector<std::pair<ASTNode*, std::pair<double, double> > > dpTable(1 << relations.size(), std::make_pair(nullptr, std::make_pair(0,0)));

	for(unsigned i = 0; i < relations.size(); i++) {
		dpTable.at(1 << i) = std::make_pair(relations.at(i), std::make_pair(querygraph.getNode(i)->getECardinality(),0));
	}

	unsigned i, i1, i2, set, limit, lastSet;
	std::set<unsigned> s1, s2;
	std::pair<ASTNode*, std::pair<double, double> > p1, p2, p;

	for(i = 2; i <= relations.size(); i++) {		
		set = (1 << i) - 1;
		limit = (1 << relations.size());

		while (set < limit) {		//generate all possible subsets for set size i
		    std::cout << "DPsize: Relations under consideration: ";
		    printSet(&querygraph, set);		    
		   
		    i1 = set&(-set);
			do {		//enumerate all possible permutations of these subsets
				i2 = set - i1;
				// Do something with S1 and S2

				if(i1 == 0 || i2 == 0) {
					i1 = i&(i1 - set);
					continue;
				}

				s1 = querygraph.convertBitmapToSet(i1);
				s2 = querygraph.convertBitmapToSet(i2);
				if (!crossproducts && !querygraph.isConnected(s1,s2)) {
					i1 = i&(i1 - set);
					continue;
				}  

				p1 = dpTable.at(i1);
				p2 = dpTable.at(i2);
				if ((p1.first == nullptr) || (p2.first == nullptr)){
					i1 = i&(i1 - set);
					continue;
				}

				p = createJoinTree(p1,p2);
				querygraph.addConditionsToJoin(dynamic_cast<JoinNode*>(p.first), s1, s2);
				p.second.first = querygraph.evalSelectivity(s1, s2)*p1.second.first*p2.second.first;

				if (dpTable.at(set).first == nullptr || dpTable.at(set).second.second > p.second.second) {
					dpTable.at(set) = p;
				}

				//Print DP-TABLE
				//std::cout << "DPsub: Subsets in integer representation are: " << i1 << " - " << i2 << std::endl;
				printSubsets(&querygraph, i1, i2);

				lastSet = set;

				i1 = set&(i1 - set);
			} while (i1 != set);
			std::cout << "DPsize: Resulting join tree is: " << std::endl;
			if (dpTable.at(set).first == nullptr)
				std::cout << "DPsize: subsets not calculated.." << std::endl;
			else 
				ASTPrinter::print(dpTable.at(set).first);
			std::cout << "-------------" << std::endl;

		    // Gosper's hack: Here be dragons!
		    unsigned c = set & -set;
		    unsigned r = set + c;
		    set = (((r^set) >> 2) / c) | r;
		}
	}

	return dpTable.at(lastSet).first;
}

std::pair<ASTNode*, std::pair<double, double> > DPsizeStrategy::createJoinTree(
		std::pair<ASTNode*, std::pair<double, double> > tree1,
		std::pair<ASTNode*, std::pair<double, double> > tree2) {
	//compare hashjoin costs (1.2 is irrelevant)
	if (tree1.second.first > tree2.second.first){
		return std::make_pair(new JoinNode(tree2.first, tree1.first), std::make_pair(0, tree2.second.second*1.2));//set size afterwards
	} else {
		return std::make_pair(new JoinNode(tree1.first, tree2.first), std::make_pair(0, tree1.second.second*1.2));//set size afterwards
	}
}

void DPsizeStrategy::printSet(QueryGraph* qg, unsigned map) {
	std::string out = "{";

	for(unsigned i = 0; i < qg->getNodes().size(); i++) {
		if(map & 1 << i)
			out += qg->getNodes().at(i)->getName() + ", ";
	}
	out = out.substr(0, out.size() -2 );

	std::cout << out + "}" << std::endl;
}

void DPsizeStrategy::printSubsets(QueryGraph* qg, unsigned map1, unsigned map2) {
	std::string out = "DPsize: Subsets are: {";

	for(unsigned i = 0; i < qg->getNodes().size(); i++) {
		if(map1 & 1 << i)
			out += qg->getNodes().at(i)->getName() + ", ";
	}
	out = out.substr(0, out.size() -2 );

	out += "} and {";
	for(unsigned i = 0; i < qg->getNodes().size(); i++) {
		if(map2 & 1 << i)
			out+= qg->getNodes().at(i)->getName() + ", ";
	}

	out = out.substr(0, out.size() -2 );

	std::cout << out + "}" << std::endl;
}