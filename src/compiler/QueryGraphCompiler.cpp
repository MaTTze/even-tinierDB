/*
 * QueryGraphCompiler.cpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */
#include "QueryGraphCompiler.hpp"
#include "querygraph/Edge.hpp"
#include "querygraph/Node.hpp"


QueryGraphCompiler::QueryGraphCompiler(Database* db):db(db) {
}

QueryGraph* QueryGraphCompiler::compile(Query query) {
	q = query;
	relations = q.getRelations().size();
	qg = new QueryGraph(db, relations);
	generateNodes();
	addSelections();
	generateEdges();
	return qg;
}

/*
	Creates a node for each relation read from the from-statement in the Query
*/
void QueryGraphCompiler::generateNodes() {
	auto rel = q.getRelations();
	for (unsigned i = 0; i < rel.size(); i++) {		//iterate over all relations
		qg->addNode(rel.at(i), i);		//add a node with its relation's name and corresponding binding
	}
}

/*
	Inserts selections into the correct node. Self-Joins will be handled as selection.
*/
void QueryGraphCompiler::addSelections() {
	auto sel = q.getSelections();
	for (auto it = sel.begin(); it != sel.end(); it++){		//iterate over all selections
		unsigned rel = it->first;		//binding of selection
		auto list = it->second;
		for (auto it2 = list.begin(); it2 != list.end(); it2++){
			qg->addSelection(rel, it2->first, it2->second, true);		//add selection to node. Parameters: binding, attribute name, right Side, is right side a constant
		}
	}
	//Get all self-joins
	joinconditions = std::map<std::pair<unsigned,unsigned>, std::set<std::pair<std::string,std::string> > >(q.getJoinconditions());
	for (unsigned i = 0; i < relations; i++) {
		auto it = joinconditions.find(std::make_pair(i,i));		//find self-joins. These will have the same binding on each side.
		if (it != joinconditions.end()){
			auto list = it->second;
			for (auto it2 = list.begin(); it2 != list.end(); it2++){
				qg->addSelection(i, it2->first, it2->second, false);	//add selection to node. Parameters: see above
			}
			joinconditions.erase(it);			//delete self-joins from list of join conditions
		}
	}

}

/*
	Adds edges for every join.
*/
void QueryGraphCompiler::generateEdges() {
	for (auto it = joinconditions.rbegin(); it != joinconditions.rend();it++){
		qg->addEdge(it->first, it->second);		//add edge with parameters: left binding, right binding
	}
}
