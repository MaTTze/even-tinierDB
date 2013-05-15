/*
 * SimpleExecutor.hpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */

#ifndef SIMPLEEXECUTOR_HPP_
#define SIMPLEEXECUTOR_HPP_

class SimpleExecutor {
public:
	SimpleExecutor(ASTNode*, Query, Database*);
	void execute();

private:
	Operator* executeNode(ASTNode*);

	std::vector<Tablescan*> tablescans;
	Query query;
	ASTNode* tree;
	Database* db;
};


#endif /* SIMPLEEXECUTOR_HPP_ */
