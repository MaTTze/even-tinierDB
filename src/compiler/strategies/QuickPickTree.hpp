/*
 * QuickPickTree.hpp
 *
 *  Created on: June 24, 2013
 *      Author: jan
 */

#ifndef QUICKPICKTREE_HPP_
#define QUICKPICKTREE_HPP_
#include "../ast/ASTNode.hpp"
#include "../querygraph/QueryGraph.hpp"
#include <random>
#include <vector>

class QuickPickTree {
public:
	QuickPickTree(std::vector<ASTNode*>, QueryGraph*);


	class QPNode {
	public:
		QPNode(ASTNode* n, std::set<unsigned> bindings):isRoot(true), parent(dynamic_cast<QPNode*>(n)), relations(bindings) {} 
		
		QPNode* getRoot() {
			if(isRoot)
				return this;
			return parent->getRoot();
		}

		void setRoot(bool isRoot) {
			this->isRoot = isRoot;
		}

		void setParent(QPNode* parent) {
			this->parent = parent;
		}

		ASTNode* getASTNode() {
			return (ASTNode*)parent;
		}

		void addRelations(std::set<unsigned> rels) {
			relations.insert(rels.begin(), rels.end());
		}

		std::set<unsigned> getRelations() {
			return relations;
		}
	private:
		bool isRoot;
		QPNode* parent;
		std::set<unsigned> relations;
	};

	unsigned getTreeCount();
	ASTNode* unionTrees(unsigned, unsigned);

private:
	std::vector<QPNode*> nodes;
	unsigned treeCount;
	QueryGraph* qg;


};


#endif /* QUICKPICKTREE_HPP_ */
