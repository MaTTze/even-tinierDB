/*
 * QueryGraphCompiler.cpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */
#include "QueryGraphCompiler.hpp"
#include "querygraph/Edge.hpp"
#include "querygraph/Node.hpp"


QueryGraph* QueryGraphCompiler::compile(Query query) {
	q = query;
	qg = new QueryGraph(db);
	relations = 0;
	generateNodes();
	addSelections();
	generateEdges();
	return qg;
}

void QueryGraphCompiler::generateNodes() {
	auto rel = q.getRelations();
	for (unsigned i = 0; i < rel.size(); i++) {
		qg->addNode(rel.at(i), i);
		relations++;
	}
}

void QueryGraphCompiler::addSelections() {
	auto sel = q.getSelections();
	for (auto it = sel.begin(); it != sel.end(); it++){
		unsigned rel = it->first;
		auto list = it->second;
		for (auto it2 = list.begin(); it2 != list.end(); it2++){
			qg->addSelection(rel, it2->first, it2->second, true);
		}
	}
	joinconditions = std::map<std::pair<unsigned,unsigned>, std::set<std::pair<std::string,std::string> > >(q.getJoinconditions());
	for (unsigned i = 0; i < relations; i++) {
		auto it = joinconditions.find(std::make_pair(i,i));
		if (it != joinconditions.end()){
			auto list = it->second;
			for (auto it2 = list.begin(); it2 != list.end(); it2++){
				qg->addSelection(i, it2->first, it2->second, false);
			}
			joinconditions.erase(it);
		}
	}

}

QueryGraphCompiler::QueryGraphCompiler(Database* db):db(db) {
}

void QueryGraphCompiler::generateEdges() {
	for (auto it = joinconditions.rbegin(); it != joinconditions.rend();it++){
		qg->addEdge(it->first, it->second);
	}
}
