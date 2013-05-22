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
	void addNode(std::string, unsigned);
	void addEdge(std::pair<unsigned, unsigned>, std::set<std::pair<std::string, std::string> >);
	void addSelection(unsigned, std::string, std::string, bool);
	Node* getNode(unsigned);

private:
	Database* db;
	std::vector<Node*> nodes;
	std::vector<Edge*> edges;
};



#endif /* QUERYGRAPH_HPP_ */
