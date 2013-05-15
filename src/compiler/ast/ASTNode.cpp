/*
 * ASTNode.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "ASTNode.hpp"

ASTNode* ASTNode::getParent() {
	return parent;
}

void ASTNode::setParent(ASTNode* p) {
	parent = p;
}
