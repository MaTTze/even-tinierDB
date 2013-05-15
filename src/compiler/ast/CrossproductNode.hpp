/*
 * CrossproductNode.hpp
 *
 *  Created on: May 14, 2013
 *      Author: matthias
 */

#ifndef CROSSPRODUCTNODE_HPP_
#define CROSSPRODUCTNODE_HPP_

class CrossproductNode:ASTNode {
public:
	Type getType();
private:
	ASTNode* parent;
	ASTNode* left;
	ASTNode* right;
};



#endif /* CROSSPRODUCTNODE_HPP_ */
