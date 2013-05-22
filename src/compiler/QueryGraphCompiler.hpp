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

class QueryGraphCompiler {
public:
	QueryGraph compile(Query);
private:
	void generateNodes();
	void addSelections();
	void generateEdges();

	QueryGraph qg;
	Query q;
};



#endif /* QUERYGRAPHCOMPILER_HPP_ */
