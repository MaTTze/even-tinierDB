/*
 * QueryGraph.hpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */

#ifndef QUERYGRAPH_HPP_
#define QUERYGRAPH_HPP
#include "Node.hpp"
#include "Edge.hpp"
#include "Database.hpp"
#include <vector>
#include <set>
#include <string>

class QueryGraph {
public:
	QueryGraph(Database* db);

	//Simply adds a note to the Query Graph.
	void addNode(std::string, unsigned);

	//Adds an edge to the Query Graph. Estimated selectivity handled in constructor of Edge.
	void addEdge(std::pair<unsigned, unsigned>, std::set<std::pair<std::string, std::string> >);

	//Adds a selection to a sepecific Node in the Query Graph. Estimation of selectivity is done in Node.
	void addSelection(unsigned, std::string, std::string, bool);

	//Get a node by its binding.
	Node* getNode(unsigned);

	void print();

private:
	Database* db;					//pointer to database to retreive tables and domain sizes
	std::vector<Node*> nodes;		//all nodes in the Query Graph
	std::vector<Edge*> edges;		//all edges in the Query Graph
};



#endif /* QUERYGRAPH_HPP_ */
