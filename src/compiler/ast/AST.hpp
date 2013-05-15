/*
 * AST.hpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */

#ifndef AST_HPP_
#define AST_HPP_
#include "ASTNode.hpp"

class AST {
	ASTNode* get Root();

private:
	ASTNode* root;
};


#endif /* AST_HPP_ */
