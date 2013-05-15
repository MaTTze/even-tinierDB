/*
 * JoinNode.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "JoinNode.hpp"


ASTNode::Type JoinNode::getType() {
	return ASTNode::Type::Join;
}

ASTNode* JoinNode::getRight() {
	return right;
}

ASTNode* JoinNode::getLeft() {
	return left;
}

std::map<std::pair<unsigned,unsigned>, std::set<std::pair<std::string,std::string>>> JoinNode::getConditions() {
	return joinconditions;
}

void JoinNode::setRight(ASTNode* r) {
	right = r;
}

void JoinNode::setLeft(ASTNode* l) {
	left = l;
}

JoinNode::JoinNode(ASTNode* left, ASTNode* right) :left(left), right(right){
}

void JoinNode::addCondition(std::pair<unsigned,unsigned> bindings, std::set<std::pair<std::string,std::string>> condition) {
	joinconditions.insert(std::make_pair(bindings, condition));
}
