/*
 * Node.hpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */

#ifndef NODE_HPP_
#define NODE_HPP_
#include "Edge.hpp"
#include <map>
#include <string>

class Node {
public:
	Node(Database*, std::string);
	void addEdge(unsigned, Edge*);
	void addSelection(std::string, std::string, bool);

	std::string getName();


private:
	Database* db;
	std::string relationName;
	std::map<unsigned, Edge*> edges;
	std::map<std::pair<std::string, std::string>, bool> selections;
	unsigned baseCardinality;
	double estimatedCardinality;
};




#endif /* NODE_HPP_ */
