/*
 * SelectionNode.hpp
 *
 *  Created on: May 14, 2013
 *      Author: matthias
 */

#ifndef SELECTIONNODE_HPP_
#define SELECTIONNODE_HPP_

#include "ASTNode.hpp"
#include <string>

class SelectionNode: public ASTNode {
public:
	ASTNode::Type getType();
	SelectionNode(ASTNode*, unsigned, std::string, std::string, bool);
	ASTNode* getChild();
	unsigned getBinding();
	std::string getAttribute();
	std::string getRight();
	bool isConstant();
	
private:
	ASTNode* child;
	unsigned binding;
	std::string attribute;
	std::string right;
	bool constant;
};


#endif /* SELECTIONNODE_HPP_ */
