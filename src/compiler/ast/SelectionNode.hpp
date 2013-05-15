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

class SelectionNode:ASTNode {
public:
	Type getType();
private:
	TablescanNode* child;
	ASTNode* parent;
	std::string first;
	bool constant;
	std::string second;
};


#endif /* SELECTIONNODE_HPP_ */
