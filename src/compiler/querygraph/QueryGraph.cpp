/*
 * QueryGraph.cpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */
#include "QueryGraph.hpp"

QueryGraph::QueryGraph(Database* db):db(db){}

/*
	Simply adds a note to the Query Graph.
*/
void QueryGraph::addNode(std::string relName, unsigned binding) {
	Node* n = new Node(db, relName);
	nodes.at(binding) = n;
}

/*
	Adds an edge to the Query Graph. Estimated selectivity handled in constructor of Edge.
*/
void QueryGraph::addEdge(std::pair<unsigned, unsigned> bindings, std::set<std::pair<std::string, std::string>> conditions) {
	Edge* e = new Edge(db, bindings.first, bindings.second, conditions, this);
	nodes.at(bindings.first)->addEdge(bindings.second, e);
	nodes.at(bindings.second)->addEdge(bindings.first, e);
	edges.push_back(e);
}

/*
	Adds a selection to a sepecific Node in the Query Graph. Estimation of selectivity is done in Node.
*/
void QueryGraph::addSelection(unsigned binding, std::string attribute, std::string constant, bool isConstant) {
	nodes.at(binding)->addSelection(attribute, constant, isConstant);
}

/*
	Get a node by its binding.
*/
Node* QueryGraph::getNode(unsigned binding) {
	return nodes.at(binding);
} 

void QueryGraph::print(){
	for(unsigned i = 0; i < nodes.size(); i++) {
		std::cout << nodes.at(i)->getName() << std::endl;
	}
}

