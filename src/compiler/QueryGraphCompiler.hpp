/*
 * QueryGraphCompiler.hpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */

#ifndef QUERYGRAPHCOMPILER_HPP_
#define QUERYGRAPHCOMPILER_HPP_

#include "querygraph/QueryGraph.hpp"
#include "../parser/Query.hpp"
#include "Database.hpp"

class QueryGraphCompiler {
public:
	QueryGraph* compile(Query);
	QueryGraphCompiler(Database*);
private:
	void generateNodes();
	void addSelections();
	void generateEdges();

	QueryGraph* qg;
	Query q;
	Database* db;
	unsigned relations;
	std::map<std::pair<unsigned,unsigned>, std::set<std::pair<std::string,std::string> > > joinconditions;
};



#endif /* QUERYGRAPHCOMPILER_HPP_ */
