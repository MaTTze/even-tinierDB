/*
 * SimpleExecutor.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "SimpleExecutor.hpp"
#include <sys/time.h>

SimpleExecutor::SimpleExecutor(ASTNode* root, Query query, Database* db): tree(root), query(query), db(db) {
}

void SimpleExecutor::execute() {
	tablescans = std::vector<Tablescan*>(query.getRelations().size());

	Printer out(std::move(executeNode(tree)));
	std::cout << "Executing..." << std::endl;
	unsigned long int start = time(NULL);
	out.open();
	while (out.next());
	out.close();

	std::cout << "Finished. Execution took ~" << time(NULL)-start << " seconds." << std::endl;

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

	tablescans[n->getRelation()] = dynamic_cast<Tablescan*>(s.get());
	std::cout << "Build Tablescan " << n->getTableName() << ". " << n->getRelation() << std::endl;
	return std::move(s);
}

std::unique_ptr<Operator> SimpleExecutor::executeSelection(SelectionNode* n) {
    std::unique_ptr<Operator> childOp = executeNode(n->getChild());
	std::cout << "Build Selection" << std::endl;
    Tablescan* scan= tablescans.at(n->getBinding());
	
    Register* condition = new Register(); setConstantCondition(condition, n->getBinding(), n->getAttribute(), n->getRight());
    const Register* reg = scan->getOutput(n->getAttribute());

   	std::unique_ptr<Operator> select(new Selection(std::move(childOp), reg, condition));
   	constantRegisters.push_back(condition);
    return std::move(select);
}

std::unique_ptr<Operator> SimpleExecutor::executeJoin(JoinNode* n) {
	std::unique_ptr<Operator> leftOp(executeNode(n->getLeft()));
	std::unique_ptr<Operator> rightOp(executeNode(n->getRight()));
	std::cout << "Build Join" << std::endl;

	auto joins = n->getConditions();

	std::unique_ptr<Operator> op;

	if (joins.empty()){
		std::unique_ptr<Operator> op(new CrossProduct(std::move(leftOp),std::move(rightOp)));
		return std::move(op);
	}

	bool first = true;
	for (auto it = joins.begin(); it != joins.end();it++){
		Tablescan* scanLeft = tablescans.at(it->first.first);
		Tablescan* scanRight = tablescans.at(it->first.second);
		auto conditions = it->second;

		for(auto it2 = conditions.begin(); it2 != conditions.end(); it2++) {
			const Register* regLeft;
			const Register* regRight;

			if(scanLeft->getTable().findAttribute(it2->first) != -1) {
				regLeft = scanLeft->getOutput(it2->first);
				regRight = scanRight->getOutput(it2->second);
			} else {
				regLeft = scanLeft->getOutput(it2->second);
				regRight = scanRight->getOutput(it2->first);
			}

			std::unique_ptr<Operator> op2;

			if(first) {
				std::unique_ptr<Operator> hj(new HashJoin(std::move(rightOp), std::move(leftOp), regRight, regLeft));
				op2 = std::move(hj);
			}
			else {
				std::unique_ptr<Operator> sel(new Selection(std::move(op), regLeft, regRight));
				op2 = std::move(sel);
			}
			op = (std::move(op2));
			first = false;
		}
	}
	return std::move(op);
}

std::unique_ptr<Operator> SimpleExecutor::executeProjection(ProjectionNode* n) {
	std::unique_ptr<Operator> childOp(executeNode(n->getChild()));
	std::cout << "Build Projection" << std::endl;
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
