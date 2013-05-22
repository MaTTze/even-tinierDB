/*
 * QueryGraph.cpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */


QueryGraph::QueryGrap(){}

void QueryGraph::addNode(std::string relName, unsigned binding) {
	Node* n = new Node(db, relName);
	nodes[binding] = n;
}

void QueryGraph::addEdge(std::pair<unsigned, unsigned> bindings, std::set<std::pair<std::string, std::string>> conditions) {
	Edge* e = new Edge(db, bindings.first, bindings.second, conditions, this);
	nodes.at(bindings.first).addEdge(bindings.second, e);
	nodes.at(bindings.second).addEdge(bindings.first, e);
	edges.push_back(e);
}

void QueryGraph::addSelection(unsigned binding, std::string attribute, std::string constant, bool isConstant) {
	nodes.at(binding).addSelection(attribute, constant, isConstant);
}

Node* getNode(unsigned binding) {
	return nodes.at(binding);
} 

