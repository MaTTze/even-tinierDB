/*
 * SelectionNode.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "SelectionNode.hpp"

ASTNode::Type SelectionNode::getType() {
	return ASTNode::Type::Selection;
}

SelectionNode::SelectionNode(ASTNode* n, unsigned b, std::string a, std::string r, bool c) {
	child = n;
	binding = b;
	attribute = a;
	right = r;
	constant = c;
}

ASTNode* SelectionNode::getChild() {
	return child;
}

std::string SelectionNode::getBinding() {
	return binding;
}

std::string SelectionNode::getAttribute() {
	return attribute;
}

std::string SelectionNode::getRight() {
	return right;
}

bool SelectionNode::isConstant() {
	return constant;
}
