/*
 * ProjectionNode.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "ProjectionNode.hpp"

	ASTNode::Type ProjectionNode::getType() {
		return ASTNode::Type::Projection;
	}

	ASTNode* ProjectionNode::getChild() {
		return child;
	}

	std::unordered_map<unsigned,std::unordered_set<std::string>> ProjectionNode::getProjections() {
		return projections;
	}

ProjectionNode::ProjectionNode(ASTNode* child,
		std::unordered_map<unsigned, std::unordered_set<std::string> > projections): child(child), projections(projections) {
}

	void ProjectionNode::setChild(ASTNode* c) {
		child = c;
	}

	void ProjectionNode::addProjection(unsigned id ,std::unordered_set<std::string> att) {
		projections.insert(std::make_pair(id, att));
}

