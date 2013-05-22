/*
 * Node.cpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */
#include "Node.hpp"

Node::Node(Database* database, std::string relName) {
	relationName = relName;
	db = database;

	baseCardinality = (db->getTable(relationName)).getCardinality();
	estimatedCardinality = static_cast<double>(baseCardinality);
}

void Node::addEdge(unsigned binding, Edge* edge) {
	edges.insert(std::make_pair(binding, edge));
}

void Node::addSelection(std::string attribute, std::string constant, bool isConstant) {
	selections.insert(std::make_pair(std::make_pair(attribute, constant), isConstant));

	Table& t = db->getTable(relationName);
	Attribute a1 = t.getAttribute(t.findAttribute(attribute));

	if(isConstant) {
		estimatedCardinality = estimatedCardinality / a1.getUniqueValues();
	} else {
		Attribute a2 = t.getAttribute(t.findAttribute(constant));
		estimatedCardinality = estimatedCardinality / (std::max(a1.getUniqueValues(), a2.getUniqueValues()));
	}

}

std::string Node::getName() {
	return relationName;
}
