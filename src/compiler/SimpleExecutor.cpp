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
			return std::move(executeTablescan(dynamic_cast<TablescanNode*>(node)));
			break;
		case ASTNode::Type::Selection:
			return std::move(executeSelection(dynamic_cast<SelectionNode*>(node)));
			break;
		case ASTNode::Type::Join:
			return std::move(executeJoin(dynamic_cast<JoinNode*>(node)));
			break;
		case ASTNode::Type::Projection:
			return std::move(executeProjection(dynamic_cast<ProjectionNode*>(node)));
			break;
		default:
			break;
		}
	return std::move(std::unique_ptr<Operator>(nullptr));
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

    Tablescan* scan= tablescans.at(n->getBinding());
	
    Register* condition = new Register(); setConstantCondition(condition, n->getBinding(), n->getAttribute(), n->getRight());
    const Register* reg = scan->getOutput(n->getAttribute());

   	std::unique_ptr<Operator> select(new Selection(std::move(childOp), reg, condition));
   	constantRegisters.push_back(condition);
    return std::move(select);
}

std::unique_ptr<Operator> SimpleExecutor::executeJoin(JoinNode* n) {
	executeNode(n->getLeft());
	return executeNode(n->getRight());
}

std::unique_ptr<Operator> SimpleExecutor::executeProjection(ProjectionNode* n) {
	std::cout << "Projection" << std::endl;
	std::unique_ptr<Operator> childOp(executeNode(n->getChild()));
	auto projections = n->getProjections();
	
	if(projections.size() < 1) {
		return std::move(childOp);
	} else {
		std::vector<const Register*> regs;
		for (auto it = projections.begin(); it != projections.end();it++){
			Tablescan* scan= tablescans.at(it->first);
			for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++){
				regs.push_back(scan->getOutput(*it2));
			}
		}

		std::unique_ptr<Operator> p(new Projection(std::move(childOp),regs));
		return std::move(p);
	}
	
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
