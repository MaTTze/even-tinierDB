/*
 * Node.hpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */

#ifndef NODE_HPP_
#define NODE_HPP_
#include "Edge.hpp"
#include "Database.hpp"
#include <map>
#include <string>

class Node {
public:
	
	//Creates a new Node. Also gets the cardinality of the relation from the data base. 
	Node(Database*, std::string);

	//Adds pointers to edges connected with this node to the map of edges
	void addEdge(unsigned, Edge*);

	//Adds a selection to the node. Updates estimated cardinality. Caution: real selections and self-joins differ here!
	void addSelection(std::string, std::string, bool);

	//Get the name of the relation represented by this node.
	std::string getName();

	//Get all edges
	std::map<unsigned, Edge*> getEdges();


private:
	Database* db;					//Pointer to DB, in order to get cardinalities and domain sizes
	std::string relationName;		//Name of the relation this node is representing as plain string
	std::map<unsigned, Edge*> edges;	//map of all connected edges. The unsigned value represents the binding(sort of an id) of the other node the edge is connected to.
	std::map<std::pair<std::string, std::string>, bool> selections;	//map of all selections. First string is attribute, second the constant. bool depicts whether this is a self-join, in which case the second string is interpreted as second join attribute
	unsigned baseCardinality;		//untouched cardinality of the relation
	double estimatedCardinality;	//estimated cardinality after selections have been added
};




#endif /* NODE_HPP_ */
