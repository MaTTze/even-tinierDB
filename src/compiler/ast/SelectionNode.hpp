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
	SelectionNode(ASTNode*, std::string, std::string, bool);
	ASTNode* getChild();
	std::string getFirst();
	std::string getSecond();
	bool isConstant();
	
private:
	ASTNode* child;
	std::string first;
	bool constant;
	std::string second;
};


#endif /* SELECTIONNODE_HPP_ */
