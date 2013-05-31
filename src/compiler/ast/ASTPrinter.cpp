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

void ASTPrinter::print(ASTNode* node) {
	printHelper(node, 0);
}

void ASTPrinter::printHelper(ASTNode* node, unsigned indentation) {
	printIndentation(indentation);
	
	switch (node->getType()){
	case ASTNode::Type::Tablescan:
		 std::cout << dynamic_cast<TablescanNode*>(node)->getTableName() << std::endl;
		 break;
	case ASTNode::Type::Selection:
		std::cout << "Selection" << std::endl;
		printHelper(dynamic_cast<SelectionNode*>(node)->getChild(), indentation+1);
		break;
	case ASTNode::Type::Join:
		if(dynamic_cast<JoinNode*>(node)->getConditions().size() < 1)
			std::cout << "Crossproduct" << std::endl;
		else
			std::cout << "Join" << std::endl;
		printHelper(dynamic_cast<JoinNode*>(node)->getLeft(), indentation+1);
		printHelper(dynamic_cast<JoinNode*>(node)->getRight(), indentation+1);
		break;
	case ASTNode::Type::Projection:
		std::cout << "Projection" << std::endl;
		printHelper(dynamic_cast<ProjectionNode*>(node)->getChild(), indentation+1);
		break;
	default:
		break;
	}
}

void ASTPrinter::printIndentation(unsigned indentation) {
	for (unsigned i = 0; i < indentation; i++){
		std::cout << "    ";
	}
}

