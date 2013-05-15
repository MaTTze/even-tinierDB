/*
 * ASTNode.hpp
 *
 *  Created on: May 14, 2013
 *      Author: matthias
 */

#ifndef ASTNODE_HPP_
#define ASTNODE_HPP_

class ASTNode {
public:
	typedef enum {Join, Selection, Tablescan, Projection} Type;
	Type getType() = 0;
	ASTNode* getParent();
	void setParent(ASTNode*);
private:
	ASTNode* parent;
};


#endif /* ASTNODE_HPP_ */
