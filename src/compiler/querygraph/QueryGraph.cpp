/*
 * QueryGraph.cpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */
#include "QueryGraph.hpp"

QueryGraph::QueryGraph(Database* db, unsigned relations):db(db){
	nodes.resize(relations);
}

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
	edges.insert(std::make_pair(bindings, e));
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

std::vector<Node*> QueryGraph::getNodes() {
	return nodes;
}

std::map<std::pair<unsigned, unsigned>, Edge*> QueryGraph::getEdges() {
	return edges;
}

//Evaluates the estimated selectivity of a join from a set of relations to another set of relations
double QueryGraph::evalSelectivity(std::set<unsigned> bindings1, std::set<unsigned> bindings2) {
	double selectivity = 1.0;
	for(auto it = bindings1.begin(); it != bindings1.end(); it++) {
		for(auto it2 = bindings2.begin(); it2 != bindings2.end(); it2++) {
			auto e = edges.end();		//"init" e

			//if an edge between the current binding1 and binding2 exists, get its selectivity and multiply it by the current selectivity.
			if(*it > *it2) {		//check if binding1 is bigger than binding2, as we inserted them in descending order
				e = edges.find(std::make_pair(*it, *it2));	
			} else {
				e = edges.find(std::make_pair(*it2, *it));
			}
			if(e != edges.end())
				selectivity *= e->second->getSelectivity();
		}
	}
	return selectivity;
}

//Checks if a set of relations is somehow connected to another set of bindings
bool QueryGraph::isConnected(std::set<unsigned> bindings1, std::set<unsigned> bindings2) {
	bool ret = false;
	std::cout << "wtf" << std::endl;
	for(auto it = bindings1.begin(); it != bindings1.end(); it++) {
		for(auto it2 = bindings2.begin(); it2 != bindings2.end(); it2++) {
			auto e = edges.end();		//"init" e

			//if an edge between the current binding1 and binding2 exists, get its selectivity and multiply it by the current selectivity.
			if(*it > *it2) {		//check if binding1 is bigger than binding2, as we inserted them in descending order
				e = edges.find(std::make_pair(*it, *it2));	
			} else {
				e = edges.find(std::make_pair(*it2, *it));
			}
			if(e != edges.end()) {
				return true;
			}
		}
	}
	std::cout << "not connected" << std::endl;
	return ret;
}

//Converts a bitmap that represents a set of relations to a set of relations
std::set<unsigned> QueryGraph::convertBitmapToSet(unsigned map) {
	std::set<unsigned> set;

	for(int i = 0; i < nodes.size(); i++) {
		if(map & 1 << i)
			set.insert(i);
	}

	return set;
}

//Adds all join conditions present between two sets of nodes to a single join node (compare to ::evalSelectivity)
void QueryGraph::addConditionsToJoin(JoinNode* n, std::set<unsigned> bindings1, std::set<unsigned> bindings2) {
	for(auto it = bindings1.begin(); it != bindings1.end(); it++) {
		for(auto it2 = bindings2.begin(); it2 != bindings2.end(); it2++) {
			auto e = edges.end();
			if(*it > *it2) {
				e = edges.find(std::make_pair(*it, *it2));
			} else {
				e = edges.find(std::make_pair(*it2, *it));
			}
			if(e != edges.end()){
				std::cout << *it << " with " << *it2 << std::endl;
				n->addCondition(std::make_pair(*it, *it2), e->second->getConditions());
			}
		}
	}
}

void QueryGraph::print(){
	for(unsigned i = 0; i < nodes.size(); i++) {
		Node* node = nodes.at(i);
		std::cout << node->getName() << " is joined with: " << std::endl;
		auto edgemap = node->getEdges();

		for(auto it = edgemap.begin(); it !=  edgemap.end(); it++) {
			std::cout << "\t" << nodes.at(it->first)->getName() << " on: ";
			Edge* e = it->second;
			auto conditions = e->getConditions();

			for(auto it2 = conditions.begin(); it2 != conditions.end(); it2++) {
				std::cout << it2->first << "=" << it2->second << ", ";
			}

			std::cout << std::endl;
			std::cout << "\t\tEstimated selectivity of join: " << e->getSelectivity() << std::endl;
		}
	}
}

