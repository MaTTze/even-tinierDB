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
#include <string>

DPsubStrategy::DPsubStrategy() {
	// TODO Auto-generated constructor stub

}

DPsubStrategy::~DPsubStrategy() {
	// TODO Auto-generated destructor stub
}

ASTNode* DPsubStrategy::generateJoinTree(QueryGraph querygraph,	std::vector<ASTNode*>& relations) {

	std::cout << "Compiler: Running DPsub" << std::endl;

	//vector: <Root of tree, <estimated cardinality, estimated selectivity>>
	std::vector<std::pair<ASTNode*, std::pair<double, double> > > dpTable(1 <<relations.size(), std::make_pair(nullptr, std::make_pair(0,0)));

	for(unsigned i = 0; i < relations.size(); i++) {
		dpTable.at(1 << i) = std::make_pair(relations.at(i), std::make_pair(querygraph.getNode(i)->getECardinality(),0));
	}

	unsigned i, i1, i2;
	std::set<unsigned> s1, s2;
	std::pair<ASTNode*, std::pair<double, double> > p1, p2, p;

	std::cout << "DPsub: Building DPTable ";
	if(crossproducts)
		std::cout << "with ";
	else
		std::cout << "without "; 
	std::cout << "crossproducts..." << std::endl;

	for(i = 3; i <= (1 << relations.size()) - 1; i++) {			//i = 3, as it's the first set with two relations
		i1 = i&(-i);
		do {
			i2 = i - i1;

			if(i1 == 0 || i2 == 0) {		//if any set is empty: continue.
				i1 = i&(i1 - i);
				continue;
			}

			s1 = querygraph.convertBitmapToSet(i1);		//QueryGraph functions for sets operate on std::set<unsigned>. Convert the bitmaps to such sets.
			s2 = querygraph.convertBitmapToSet(i2);
			if (!crossproducts && !querygraph.isConnected(s1,s2)) {		
				i1 = i&(i1 - i);				//if no crossproducts allowed, or sets are not connected: continue.
				continue;	
			}  

			p1 = dpTable.at(i1);		//get sub trees
			p2 = dpTable.at(i2);
			if ((p1.first == nullptr) || (p2.first == nullptr)){		//if either of them doesn't exist: continue.
				i1 = i&(i1 - i);
				continue;
			}

			p = createJoinTree(p1,p2);		//create join tree
			querygraph.addConditionsToJoin(dynamic_cast<JoinNode*>(p.first), s1, s2);	//add correct conditions to it.
			p.second.first = querygraph.evalSelectivity(s1, s2)*p1.second.first*p2.second.first;	//evaluate estimated selectivity

			if (dpTable.at(i).first == nullptr || dpTable.at(i).second.second > p.second.second) {		
				dpTable.at(i) = p;			//insert into DPTable if no entry exists, or this tree is better than the old one
			}

			//Print DP-TABLE
			//std::cout << "DPsub: Subsets in integer representation are: " << i1 << " - " << i2 << std::endl;
			/*
			printSubsets(&querygraph, i1, i2);
			std::cout << "DPsub: Resulting join tree is: " << std::endl;
			if (dpTable.at(i).first == nullptr)
				std::cout << "DPsub: subsets not calculated.." << std::endl;
			else 
				ASTPrinter::print(dpTable.at(i).first);
			std::cout << "-------------" << std::endl;
			*/


			i1 = i&(i1 - i);
		} while (i1 != i);
	}

	return dpTable.at(--i).first;		//return root of best tree for complete set
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

void DPsubStrategy::printSubsets(QueryGraph* qg, unsigned map1, unsigned map2) {
	std::string out = "DPsub: Subsets are: {";

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




