/*
 * SimpleExecutor.hpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */

#ifndef SIMPLEEXECUTOR_HPP_
#define SIMPLEEXECUTOR_HPP_
#include "ast/ASTNode.hpp"
#include "ast/TablescanNode.hpp"
#include "ast/JoinNode.hpp"
#include "ast/SelectionNode.hpp"
#include "ast/ProjectionNode.hpp"
#include "../parser/Query.hpp"
#include "Database.hpp"
#include "operator/Operator.hpp"
#include "operator/Tablescan.hpp"
#include "operator/CrossProduct.hpp"
#include "operator/Selection.hpp"
#include "operator/Projection.hpp"
#include "operator/HashJoin.hpp"
#include "operator/Printer.hpp"
#include <iostream>
#include <memory>

class SimpleExecutor {
public:
	SimpleExecutor(ASTNode*, Query, Database*);
	void execute();

private:
	std::vector<Tablescan*> tablescans;
	std::vector<Register*> constantRegisters;
	ASTNode* tree;
	Query query;
	Database* db;

	std::unique_ptr<Operator> executeNode(ASTNode*);
	std::unique_ptr<Operator> executeTablescan(TablescanNode*);
	std::unique_ptr<Operator> executeSelection(SelectionNode*);
	std::unique_ptr<Operator> executeJoin(JoinNode*);
	std::unique_ptr<Operator> executeProjection(ProjectionNode*);
	void setConstantCondition(Register*, unsigned, std::string, std::string);
};


#endif /* SIMPLEEXECUTOR_HPP_ */
