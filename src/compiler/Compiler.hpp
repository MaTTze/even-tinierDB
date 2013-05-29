/*
 * Compiler.hpp
 *
 *  Created on: May 29, 2013
 *      Author: matthias
 */

#ifndef COMPILER_HPP_
#define COMPILER_HPP_
#include "ast/ASTNode.hpp"
#include "../parser/Query.hpp"
#include "strategies/OrderStrategy.hpp"

class Compiler {
public:
	ASTNode* compile(Query);
	Compiler(OrderStrategy*);
private:
	void generateTablescans();
	void generateSelections();
	void generateJoinTree();
	void generateProjections();

	std::vector<ASTNode*> relations;
	std::map<std::pair<unsigned,unsigned>, std::set<std::pair<std::string,std::string> > > joinconditions;
	ASTNode* currentRoot;
	OrderStrategy* s;
	Query q;
};


#endif /* COMPILER_HPP_ */
