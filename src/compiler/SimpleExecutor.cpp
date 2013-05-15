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
	Printer out(std::move(executeNode(tree)));

	out.open();
	while (out.next());
	out.close();

}

std::unique_ptr<Operator> SimpleExecutor::executeNode(ASTNode* node) {
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

std::unique_ptr<Operator> SimpleExecutor::executeTablescan(TablescanNode* n) {
	Table& t = db->getTable(n->getTableName());
	std::unique_ptr<Operator> s(new Tablescan(t));

	tablescans.push_back(s);

   	return s;
}

std::unique_ptr<Operator> SimpleExecutor::executeSelection(SelectionNode* n) {
	return new unique_ptr<Operator>();
}

std::unique_ptr<Operator> SimpleExecutor::executeJoin(JoinNode* n) {

}

std::unique_ptr<Operator> SimpleExecutor::executeProjection(ProjectionNode* n) {

   	return new Operator();
}
