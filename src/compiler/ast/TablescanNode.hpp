/*
 * TablescanNode.hpp
 *
 *  Created on: May 14, 2013
 *      Author: matthias
 */

#ifndef TABLESCANNODE_HPP_
#define TABLESCANNODE_HPP_
#include "ASTNode.hpp"
#include <string>

class TablescanNode: public ASTNode {
public:
	ASTNode::Type getType();
	TablescanNode(std::string tablename, unsigned relation);
	std::string getTableName();
	unsigned getRelation();
private:
	unsigned relation;
	std::string tableName;
};


#endif /* TABLESCANNODE_HPP_ */
