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
#include <math.h>

GOOStrategy::GOOStrategy() {
	// TODO Auto-generated constructor stub

}

GOOStrategy::~GOOStrategy() {
	// TODO Auto-generated destructor stub
}

ASTNode* GOOStrategy::generateJoinTree(QueryGraph querygraph,	std::vector<ASTNode*>& relations) {
	std::cout << "Compiler: Running GOO" << std::endl;

	std::vector<std::tuple<ASTNode*, std::set<unsigned>, double > > trees;
	JoinNode* n;

	//insert all base relations, their bindings as starting set and their estimated cardinalities into the vector trees
	for(unsigned i = 0; i < relations.size(); i++) {
		trees.push_back(std::make_tuple(relations.at(i), std::set<unsigned>({i}), querygraph.getNode(i)->getECardinality()));	
	}
	while(trees.size() > 1) { 		//while there are more than one tree:
		double min_val = std::numeric_limits<double>::max();		//init the threshold for estimated output size of a join as maximum value for a double
		auto child1 = trees.begin();	//init iterators
		auto child2 = trees.begin();
		
		for(auto it = trees.begin(); it != --trees.end(); it++) {		//for all trees except the last:
			for(auto it2 = it+1; it2 != trees.end(); it2++) {		//for all trees from current tree to last:

				//estimates output size(eos) of joining these trees as: ceiling of(eos(tree1) * eos(tree2) * estimated selectivity of their joinconditions) 
				double outputsize = ceil(std::get<2>(*it2) * std::get<2>(*it) * querygraph.evalSelectivity(std::get<1>(*it), std::get<1>(*it2)));
				if(outputsize < min_val) {		//if this is a new best known join:
					child1 = it;		//store current sub trees and their estimated output size
					child2 = it2;
					min_val = outputsize;
				}
			}
		}

		n = new JoinNode(std::get<0>(*child1), std::get<0>(*child2));		//generate a join of the best estimated combination of sub trees

		//add all conditions to the newly created joinNode that exist from the set of relations in child1 to the set of relations in child2
		querygraph.addConditionsToJoin(n, std::get<1>(*child1), std::get<1>(*child2));		

		std::get<0>(*child1)->setParent(n);		//set pointers correctly
		std::get<0>(*child2)->setParent(n);

		std::set<unsigned> combinedRelations = std::get<1>(*child1);
		combinedRelations.insert(std::get<1>(*child2).begin(), std::get<1>(*child2).end());		//the resulting tree represents now all relations of child1 AND child2
		std::cout << "	";
		for (auto it = combinedRelations.begin(); it != combinedRelations.end(); it++) {
			std::cout << *it;
		}

		//print output sizes
		std::cout << " = Join(";
		for (auto it = std::get<1>(*child1).begin(); it != std::get<1>(*child1).end(); it++) {
			std::cout << *it;
		}
		std::cout << ", ";
		for (auto it = std::get<1>(*child2).begin(); it != std::get<1>(*child2).end(); it++) {
			std::cout << *it;
		}
		std::cout << "; " << min_val << ")" << std::endl;
		
		//don't alter the order of these operations. 
		trees.erase(child2);
		trees.erase(child1);

		//add a the combined tree to the set of trees.
		trees.push_back(std::make_tuple(n, combinedRelations, min_val));
	}

	return n;
}
