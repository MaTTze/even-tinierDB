/*
 * SimpleCompiler.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "SimpleCompiler.hpp"
#include "ast/TablescanNode.hpp"
#include "ast/SelectionNode.hpp"
#include "ast/JoinNode.hpp"
#include "ast/ProjectionNode.hpp"


ASTNode* SimpleCompiler::compile(Query query) {
	q = query;
	generateTablescans();
	generateSelections();
	generateCrossproducts();
	pushdownSelections();
	generateProjections();
	return currentRoot;
}

void SimpleCompiler::generateTablescans() {
	auto rel = q.getRelations();
	for (unsigned i = 0; i < rel.size(); i++) {
		relations.push_back(new TablescanNode(rel.at(i), i));
	}
}

void SimpleCompiler::generateSelections() {
	auto sel = q.getSelections();
	for (auto it = sel.begin(); it != sel.end(); it++){
		unsigned rel = it->first;
		auto list = it->second;
		for (auto it2 = list.begin(); it2 != list.end(); it2++){
			ASTNode* tmp = new SelectionNode(relations[rel], rel, it2->first, it2->second, true);
			relations[rel]->setParent(tmp);
			relations[rel] = tmp;
		}
	}
	joinconditions = std::map<std::pair<unsigned,unsigned>, std::set<std::pair<std::string,std::string> > >(q.getJoinconditions());
	for (unsigned i = 0; i < relations.size(); i++) {
		auto it = joinconditions.find(std::make_pair(i,i));
		if (it != joinconditions.end()){
			auto list = it->second;
			for (auto it2 = list.begin(); it2 != list.end(); it2++){
				ASTNode* tmp = new SelectionNode(relations[i], i, it2->first, it2->second, false);
				relations[i]->setParent(tmp);
				relations[i] = tmp;
			}
			joinconditions.erase(it);
		}
	}

}

void SimpleCompiler::generateCrossproducts() {
	currentRoot = relations[0];
	ASTNode* tmp;
	for (unsigned i = 1; i < relations.size(); i++){
		tmp = new JoinNode(currentRoot,relations[i]);
		currentRoot->setParent(tmp);
		relations[i]->setParent(tmp);
		currentRoot = tmp;
	}
}

void SimpleCompiler::pushdownSelections() {
	ASTNode* current = currentRoot;
	unsigned rel = relations.size()-1;
	for (auto it = joinconditions.rbegin(); it != joinconditions.rend();it++){
		while (rel > it->first.first){
			current = dynamic_cast<JoinNode*>(current)->getLeft();
			rel--;
		}
		dynamic_cast<JoinNode*>(current)->addCondition(it->first,it->second);
	}
}

void SimpleCompiler::generateProjections() {
	auto projections = q.getProjections();
	if (projections.empty()){
		return;
	}
	ASTNode* tmp = new ProjectionNode(currentRoot, projections);
	currentRoot->setParent(tmp);
	currentRoot = tmp;
}
