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
	generateTablescans();
	generateSelections();
	generateJoinTree();
	generateProjections();
	return currentRoot;
}

void Compiler::generateTablescans() {
	auto rel = q.getRelations();
	for (unsigned i = 0; i < rel.size(); i++) {
		relations.push_back(new TablescanNode(rel.at(i), i));
	}
}

void Compiler::generateSelections() {
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

void Compiler::generateJoinTree() {
	QueryGraphCompiler qgc = QueryGraphCompiler(q.getDB());
	QueryGraph qg = *(qgc.compile(q));
	currentRoot = s->generateJoinTree(qg, std::copy(relations));
}

Compiler::Compiler(OrderStrategy* strategy):s(strategy){
}

void Compiler::generateProjections() {
	auto projections = q.getProjections();
	if (projections.empty()){
		return;
	}
	ASTNode* tmp = new ProjectionNode(currentRoot, projections);
	currentRoot->setParent(tmp);
	currentRoot = tmp;
}
