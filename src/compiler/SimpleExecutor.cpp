/*
 * SimpleExecutor.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "SimpleExecutor.hpp"

SimpleExecutor::SimpleExecutor(ASTNode* root, Query query, Database* db): tree(root), query(query), db(db) {
}

void SimpleExecutor::execute() {
	Printer out(move(executeNode(root)));

	out.open();
	while (out.next());
	out.close();

}

Operator* SimpleExecutor::executeNode(ASTNode* node) {
	switch (node->getType()){
		case ASTNode::Type::Tablescan:
			return executeTablescan(dynamic_cast<TablescanNode*>(node));
			break;
		case ASTNode::Type::Selection:
			return executeSelection(dynamic_cast<SelectionNode*>(node));
			break;
		case ASTNode::Type::Join:
			return executeJoin(dynamic_cast<JoinNode*>(node));
			break;
		case ASTNode::Type::Projection:
			return executeProjection(dynamic_cast<ProjectionNode*>(node));
			break;
		default:
			break;
		}
}
