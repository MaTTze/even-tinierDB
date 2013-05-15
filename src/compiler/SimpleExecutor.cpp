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

	tablescans.push_back(dynamic_cast<Tablescan*>(s.get()));
std::cout << "Tablescan" << std::endl;
	return std::move(s);
}

std::unique_ptr<Operator> SimpleExecutor::executeSelection(SelectionNode* n) {
	std::cout << "Selection" << std::endl;
    std::unique_ptr<Operator> childOp = executeNode(n->getChild());

    std::unique_ptr<Tablescan> scan(tablescans.at(n->getBinding()));
	
    Register condition; setConstantCondition(&condition, n->getBinding(), n->getAttribute(), n->getRight());
    const Register* reg = scan->getOutput(n->getAttribute());

   	std::unique_ptr<Operator> select(new Selection(std::move(scan), reg, &condition));	

    return select;	
}

std::unique_ptr<Operator> SimpleExecutor::executeJoin(JoinNode* n) {

}

std::unique_ptr<Operator> SimpleExecutor::executeProjection(ProjectionNode* n) {
	std::cout << "Projection" << std::endl;
	return executeNode(n->getChild());
}

void SimpleExecutor::setConstantCondition (Register* condition, unsigned binding, std::string attribute, std::string conditionString) {
	std::string tableName = query.getRelations().at(binding);
	Table& t = db->getTable(tableName);
	const Attribute a = t.getAttribute(t.findAttribute(attribute));

	switch(a.getType()) {
      case Attribute::Type::Int:
        condition->setInt(atoi(conditionString.c_str()));
        break;
      case Attribute::Type::Double:
        condition->setDouble(atof(conditionString.c_str()));
        break;
      case Attribute::Type::Bool:
        if(conditionString == "true")
          condition->setBool(true);
        else if (conditionString == "false")
          condition->setBool(false);
        break;
      case Attribute::Type::String:
        condition->setString(conditionString);
        break;
    }
}
