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
#include <vector>
#include <set>

class QueryGraph {
public:
	QueryGrap();
	void addNode(std::string, unsigned);
	void addEdge(unsigned, unsigned);
	void addSelection(unsigned, std::string, std::string, bool);
	Node* getNode(unsigned);

private:
	Database* db;
	std::vector<Node*> nodes;
	std::vector<Edge*> edges;
};



#endif /* QUERYGRAPH_HPP_ */
