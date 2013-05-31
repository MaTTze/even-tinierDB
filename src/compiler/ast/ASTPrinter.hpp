/*
 * AST.hpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */

#ifndef AST_HPP_
#define AST_HPP_
#include "ASTNode.hpp"

class ASTPrinter {
public:
	static void print(ASTNode*);
private:
	static void printHelper(ASTNode*, unsigned);
	static void printIndentation(unsigned);
};


#endif /* AST_HPP_ */
