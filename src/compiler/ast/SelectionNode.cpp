/*
 * ASTNode.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "ASTNode.hpp"

ASTNode::Type SelectionNode::getType() {
	return ASTNode::Type::Selection;
}

SelectionNode::SelectionNode(TablescanNode* n, std::string f, std::string s, bool c) {
	child = n;
	first = f;
	second = s;
	constant = c;
}

TablescanNode* SelectionNode::getChild() {
	return child;
}

std::string SelectionNode::getFirst() {
	return first;
}

std::string SelectionNode::getSecond() {
	return second;
}

bool SelectionNode::isConstant() {
	return constant;
}