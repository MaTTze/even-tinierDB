/*
 * QueryGraph.hpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */

#ifndef QUERYGRAPH_HPP_
#define QUERYGRAPH_HPP_
#include "Node.hpp"
#include "Edge.hpp"
#include "Database.hpp"
#include "../ast/JoinNode.hpp"
#include <vector>
#include <set>
#include <string>

class QueryGraph {
public:
	QueryGraph(Database* db, unsigned);

	//Simply adds a note to the Query Graph.
	void addNode(std::string, unsigned);

	//Adds an edge to the Query Graph. Estimated selectivity handled in constructor of Edge.
	void addEdge(std::pair<unsigned, unsigned>, std::set<std::pair<std::string, std::string> >);

	//Adds a selection to a sepecific Node in the Query Graph. Estimation of selectivity is done in Node.
	void addSelection(unsigned, std::string, std::string, bool);

	//Get a node by its binding.
	Node* getNode(unsigned);

	std::vector<Node*> getNodes();

	std::map<std::pair<unsigned, unsigned>, Edge*> getEdges();

	double evalSelectivity(std::set<unsigned>, std::set<unsigned>);

	void addConditionsToJoin(JoinNode*, std::set<unsigned>, std::set<unsigned>);

	void print();

private:
	Database* db;					//pointer to database to retreive tables and domain sizes
	std::vector<Node*> nodes;		//all nodes in the Query Graph
	std::map<std::pair<unsigned, unsigned>, Edge*> edges;		//all edges in the Query Graph
};



#endif /* QUERYGRAPH_HPP_ */
