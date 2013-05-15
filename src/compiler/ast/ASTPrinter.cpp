/*
 * ASTPrinter.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "ASTPrinter.hpp"

#include "TablescanNode.hpp"
#include "SelectionNode.hpp"
#include "JoinNode.hpp"
#include "ProjectionNode.hpp"
#include "iostream"

void ASTPrinter::print(ASTNode* node, unsigned indentation) {
	for (unsigned i = 0; i < indentation; i++){
		std::cout << "    ";
	}
	switch (node->getType()){
	case ASTNode::Type::Tablescan:
		 std::cout << dynamic_cast<TablescanNode*>(node)->getTableName() << std::endl;
		 break;
	case ASTNode::Type::Selection:
		std::cout << "Selection" << std::endl;
		print(dynamic_cast<SelectionNode*>(node)->getChild(), indentation+1);
		break;
	case ASTNode::Type::Join:
		std::cout << "Join" << std::endl;
		print(dynamic_cast<JoinNode*>(node)->getLeft(), indentation+1);
		print(dynamic_cast<JoinNode*>(node)->getRight(), indentation+1);
		break;
	case ASTNode::Type::Projection:
		std::cout << "Projection" << std::endl;
		print(dynamic_cast<ProjectionNode*>(node)->getChild(), indentation+1);
		break;
	default:
		break;
	}
}



