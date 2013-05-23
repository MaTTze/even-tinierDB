/*
 * Node.cpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */
#include "Node.hpp"

/*
	Creates a new Node. Also gets the cardinality of the relation from the data base. 
*/
Node::Node(Database* database, std::string relName) {
	relationName = relName;
	db = database;

	baseCardinality = (db->getTable(relationName)).getCardinality();
	estimatedCardinality = static_cast<double>(baseCardinality);
}

/*
	Adds pointers to edges connected with this node to the node's map.
*/
void Node::addEdge(unsigned binding, Edge* edge) {
	edges.insert(std::make_pair(binding, edge));
}

/*
	Adds a selection to the node. Updates estimated cardinality. Caution: real selections and self-joins differ here!
*/
void Node::addSelection(std::string attribute, std::string constant, bool isConstant) {
	selections.insert(std::make_pair(std::make_pair(attribute, constant), isConstant));

	Table& t = db->getTable(relationName);
	Attribute a1 = t.getAttribute(t.findAttribute(attribute));		//Get attribute in order to obtain domain size

	if(isConstant) {		//if this is a "real" selection (selects a constant)
		estimatedCardinality = estimatedCardinality / a1.getUniqueValues();		//estimated cardinality is current estimated cardinality * selectivity of next selection
	} else {
		Attribute a2 = t.getAttribute(t.findAttribute(constant));	//this is a self-join pushed down to the base relation. Get second join attribute
		estimatedCardinality = estimatedCardinality / (std::max(a1.getUniqueValues(), a2.getUniqueValues()));	//estimated cardinality is current estimated cardinality * selectivity of join
	}

}

/*
	Get the name of the relation represented by this node.
*/
std::string Node::getName() {
	return relationName;
}

/*
	Get all edges
*/
std::map<unsigned, Edge*> Node::getEdges() {
	return edges;
}
