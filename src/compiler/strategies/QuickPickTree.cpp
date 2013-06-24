/*
 * QuickPickTree.cpp
 *
 *  Created on: June 24, 2013
 *      Author: jan
 */
#include "QuickPickTree.hpp"
#include "../ast/JoinNode.hpp"
#include "../querygraph/Edge.hpp"
#include <iostream>

QuickPickTree::QuickPickTree(std::vector<ASTNode*> relations, QueryGraph* qg) {
	treeCount = relations.size();
	this->qg = qg;

	for(unsigned i = 0; i < relations.size(); i++) {
		nodes.push_back(new QuickPickTree::QPNode(relations.at(i), std::set<unsigned>({i})));
	}
}

unsigned QuickPickTree::getTreeCount() {
	return treeCount;
}

ASTNode* QuickPickTree::unionTrees(unsigned i1, unsigned i2) {
	QPNode* root1 = nodes.at(i1)->getRoot();
	QPNode* root2 = nodes.at(i2)->getRoot();

	ASTNode* tree1 = root1->getASTNode();
	ASTNode* tree2 = root2->getASTNode();

	if(tree1 != tree2) {
		ASTNode* join = new JoinNode(tree1, tree2);
		qg->addConditionsToJoin(dynamic_cast<JoinNode*>(join), root1->getRelations(), root2->getRelations());

		QPNode* newRoot = new QPNode(join, root1->getRelations());
		newRoot->addRelations(root2->getRelations());
		
		root1->setParent(newRoot);
		root1->setRoot(false);

		root2->setParent(newRoot);
		root2->setRoot(false);

		treeCount--;
		return join;
	} 

	return nullptr;
}

ASTNode* QuickPickTree::crossproductify() {
	std::set<ASTNode*> trees;
	for (auto it = nodes.begin(); it != nodes.end(); it++){
		trees.insert((*it)->getRoot()->getASTNode());
	}
	ASTNode* currentRoot = (*trees.begin());
	ASTNode* tmp;
	for (auto it = ++trees.begin(); it != trees.end(); it++){
		tmp = new JoinNode(currentRoot,*it);
		currentRoot->setParent(tmp);
		(*it)->setParent(tmp);
		currentRoot = tmp;
	}
	return currentRoot;
}
