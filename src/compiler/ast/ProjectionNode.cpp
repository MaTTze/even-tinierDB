/*
 * ASTNode.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "ASTNode.hpp"

	Type ProjectionNode::getType() {

	}

	ASTNode* ProjectionNode::getChild() {
		return child;
	}

	std::unordered_map<unsigned,std::unordered_set<std::string>> ProjectionNode::getProjections() {
		return projections;
	}

	void ProjectionNode::setChild(ASTNode* c) {
		child = c;
	}

	void ProjectionNode::addProjection(unsigned x ,std::unordered_set<std::string> y) {

	}