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
	tablescans = std::vector<Tablescan*>(query.getRelations().size());		//initialize vector of tablescans (see ::executeTablescan)

	Printer out(std::move(executeNode(tree)));		//start building of tinyDB Operators - and make a Printer
	std::cout << "Executing..." << std::endl;
	unsigned long int start = time(NULL);
	out.open();
	while (out.next());		//Execute Query.
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

//Handles a base relation within the AS tree.
std::unique_ptr<Operator> SimpleExecutor::executeTablescan(TablescanNode* n) {
	Table& t = db->getTable(n->getTableName());
	std::unique_ptr<Operator> s(new Tablescan(t));		//build tablescan of the relation this node is associated with.

	tablescans[n->getRelation()] = dynamic_cast<Tablescan*>(s.get());		//insert tablescan into a vector of tablescans at the specific position
																			//of this relations binding. Used to access the tablescans for Register build up. 
	std::cout << "Build Tablescan " << n->getTableName() << ". " << n->getRelation() << std::endl;
	return std::move(s);
}

//Handles a selection within the AS tree.
std::unique_ptr<Operator> SimpleExecutor::executeSelection(SelectionNode* n) {
    std::unique_ptr<Operator> childOp = executeNode(n->getChild());		//execute child nodes first (either tablescan, or another selection)
	std::cout << "Build Selection" << std::endl;
    Tablescan* scan= tablescans.at(n->getBinding());		//access tablescan to build correct registers
	
    Register* condition = new Register(); 		//register for right hand side of selection (eg. a.b=c, with c as a constant)
    setConstantCondition(condition, n->getBinding(), n->getAttribute(), n->getRight());	//assign correct value and datatype to this register
    
    const Register* reg = scan->getOutput(n->getAttribute());

   	std::unique_ptr<Operator> select(new Selection(std::move(childOp), reg, condition));	//select this constant within the prior built child operation
   	constantRegisters.push_back(condition);		/* HACK!: The constant registers would otherwise be void after leaving the scope of this function. */
    return std::move(select);
}

//Handles a Join within the AS tree.
std::unique_ptr<Operator> SimpleExecutor::executeJoin(JoinNode* n) {
	std::unique_ptr<Operator> leftOp(executeNode(n->getLeft()));		//evaluate left subtree
	std::unique_ptr<Operator> rightOp(executeNode(n->getRight()));		//evaluate right subtree

	auto joins = n->getConditions();		//get all join conditions associated with this join node (<unsigned,unsigned> mapped to set<string, string>)

	std::unique_ptr<Operator> op;

	if (joins.empty()){			//if there are no join conditions, this evaluates as a cross product
		std::unique_ptr<Operator> op(new CrossProduct(std::move(leftOp),std::move(rightOp)));
		std::cout << "Build Crossproduct" << std::endl;
		return std::move(op);
	}

	std::cout << "Build Join" << std::endl;
	
	bool first = true;		//flag for first iteration.
	for (auto it = joins.begin(); it != joins.end();it++){			//iterate over the map of all joins of this node
		Tablescan* scanLeft = tablescans.at(it->first.first);			//Get tablescans for the key-pair of this entry of the map
		Tablescan* scanRight = tablescans.at(it->first.second);
		auto conditions = it->second;	//get all conditions for the join denoted by the current key-pair

		for(auto it2 = conditions.begin(); it2 != conditions.end(); it2++) {		//iterate over the set of all join conditions of this join
			const Register* regLeft;
			const Register* regRight;

			if(scanLeft->getTable().findAttribute(it2->first) != -1) {		//find out in which of the two tables the attributes of this condition are.
				regLeft = scanLeft->getOutput(it2->first);						//set registers accordingly
				regRight = scanRight->getOutput(it2->second);
			} else {
				regLeft = scanLeft->getOutput(it2->second);
				regRight = scanRight->getOutput(it2->first);
			}

			if(first) { 	//First iteration will be a HashJoin.
				//construct hashjoin of child nodes and first registers.
				std::unique_ptr<Operator> hj(new HashJoin(std::move(leftOp), std::move(rightOp), regLeft, regRight));
				op = std::move(hj);
				first = false;
			}
			else {			//All following conditions will be converted to selections.
				//add selection on top of prior constructed HashJoin.
				std::unique_ptr<Operator> sel(new Selection(std::move(op), regLeft, regRight));
				op = std::move(sel);
			}
		}
	}
	return std::move(op);
}

//Handles projections within the AS tree.
std::unique_ptr<Operator> SimpleExecutor::executeProjection(ProjectionNode* n) {
	std::unique_ptr<Operator> childOp(executeNode(n->getChild()));	//build children first
	std::cout << "Build Projection" << std::endl;
	auto projections = n->getProjections();		//get projections associated with this node
	
	if(projections.empty()) {		//our algorithms can generate ASTs with an empty ProjectionNode as root. Therefore we check this.
		return std::move(childOp);		//return the unaltered child operator
	} else {
		std::vector<const Register*> regs;
		for (auto it = projections.begin(); it != projections.end();it++){		//build registers from projection rules.
			Tablescan* scan= tablescans.at(it->first);
			for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++){
				regs.push_back(scan->getOutput(*it2));
			}
		}

		std::unique_ptr<Operator> p(new Projection(std::move(childOp),regs));
		return std::move(p);
	}
	
}

//Sets Registers with constant value
void SimpleExecutor::setConstantCondition (Register* condition, unsigned binding, std::string attribute, std::string conditionString) {
	std::string tableName = query.getRelations().at(binding);	//resolve name of the relation associated with binding "binding"
	Table& t = db->getTable(tableName);		//get Table representation of relation
	const Attribute a = t.getAttribute(t.findAttribute(attribute));	//get the Attribute denoted by "attribute"

	//According to which data type the attribute has, set the correct constant.
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
