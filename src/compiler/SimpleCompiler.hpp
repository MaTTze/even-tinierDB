/*
 * SimpleCompiler.hpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */

#ifndef SIMPLECOMPILER_HPP_
#define SIMPLECOMPILER_HPP_
#include "ast/ASTNode.hpp"
#include "../parser/Query.hpp"

class SimpleCompiler {
public:
	ASTNode* compile(Query);
private:
	void generateTablescans();
	void generateSelections();
	void generateCrossproducts();
	void pushdownSelections();
	void generateProjections();

	std::vector<ASTNode*> relations;
	std::map<std::pair<unsigned,unsigned>, std::set<std::pair<std::string,std::string> > > joinconditions;
	ASTNode* currentRoot;
	Query q;
};


#endif /* SIMPLECOMPILER_HPP_ */
