/*
 * Compiler.cpp
 *
 *  Created on: May 29, 2013
 *      Author: matthias
 */
#include "Compiler.hpp"
#include "ast/TablescanNode.hpp"
#include "ast/SelectionNode.hpp"
#include "ast/JoinNode.hpp"
#include "ast/ProjectionNode.hpp"
#include "QueryGraphCompiler.hpp"


ASTNode* Compiler::compile(Query query) {
	q = query;
	std::cout << "Compiler: Bulding AST..." << std::endl;
	generateTablescans();
	std::cout << "Compiler: Tablescans built." << std::endl;
	generateSelections();
	std::cout << "Compiler: Selections built." << std::endl;
	generateJoinTree();
	std::cout << "Compiler: Join Tree built." << std::endl;
	generateProjections();
	std::cout << "Compiler: Projections built." << std::endl;
	std::cout << "Compiler: Finished." << std::endl;
	return currentRoot;
}

void Compiler::generateTablescans() {
	auto rel = q.getRelations();
	for (unsigned i = 0; i < rel.size(); i++) {			//for all parsed relations:
		ASTNode* tablescan = new TablescanNode(rel.at(i), i);		//create a new tablescanNode with the relations name and binding
		relations.push_back(tablescan);		//store tablescanNodes additionally for easy push down of selection (see ::generateSelections)
		currentRoot = tablescan;		//set current root to this tablescan (only relevant for queries to a single relation)
	}
}

void Compiler::generateSelections() {
	auto sel = q.getSelections();
	for (auto it = sel.begin(); it != sel.end(); it++){		//for all parsed selections:
		unsigned rel = it->first;		//binding of relation which this selection corresponds to
		auto list = it->second;			//all conditions that correspond to this relation
		for (auto it2 = list.begin(); it2 != list.end(); it2++){
			//create a new selectionNode with corresponding tablescanNode as child, the tablescan's binding, the attribute to be selected, the condition, 
			// and the flag, that this is a true selection node
			ASTNode* tmp = new SelectionNode(relations[rel], rel, it2->first, it2->second, true);		
			//set the node's pointers correctly.																											
			relations[rel]->setParent(tmp);
			relations[rel] = tmp;
			currentRoot->setParent(tmp);
			currentRoot = tmp; 	//set current root to this selection (only relevant for queries to a single relation)
		}
	}
	//Find all self-joins and consider them as a selection
	joinconditions = std::map<std::pair<unsigned,unsigned>, std::set<std::pair<std::string,std::string> > >(q.getJoinconditions());
	for (unsigned i = 0; i < relations.size(); i++) {		//iterate over all relations
		auto it = joinconditions.find(std::make_pair(i,i));		//consider only self-joins (meaning a joincondition with binding1=binding2)
		if (it != joinconditions.end()){
			auto list = it->second;		//get its conditions
			for (auto it2 = list.begin(); it2 != list.end(); it2++){
				ASTNode* tmp = new SelectionNode(relations[i], i, it2->first, it2->second, false);	//same creation of selection node as above, but stating that this is a constructed selection
				relations[i]->setParent(tmp);
				relations[i] = tmp;
				currentRoot->setParent(tmp);
				currentRoot = tmp;	//set current root to this selection (only relevant for queries to a single relation)
			}
			joinconditions.erase(it);	//remove this self-join from the map of joins.
		}
	}
}

void Compiler::generateJoinTree() {
	QueryGraphCompiler qgc = QueryGraphCompiler(q.getDB());		// initialize query graph compiler (builds query graph)
	QueryGraph qg = *(qgc.compile(q));		// build query graph
	ASTNode* joinTree = s->generateJoinTree(qg, relations);		//optimize query graph using strategy s
	if(joinTree != 0)
		currentRoot = joinTree;		//if we indeed generated a join tree, set this as current root.
}

Compiler::Compiler(OrderStrategy* strategy):s(strategy){
}

void Compiler::generateProjections() {
	auto projections = q.getProjections();
	if (projections.empty()){		//if there are no projections, we are done.
		return;
	}
	if(currentRoot == 0 ) {		//Only one relation - currentRoot hasn't been set. (legacy code. too tired to check if still necessary)
		ASTNode* tmp = new ProjectionNode(relations.at(0), projections);
		relations.at(0)->setParent(tmp);
		currentRoot = tmp;
	} else {		//Add a single projection node on top of the AS tree and set it as root.
		ASTNode* tmp = new ProjectionNode(currentRoot, projections);
		currentRoot->setParent(tmp);
		currentRoot = tmp;
	}
}
