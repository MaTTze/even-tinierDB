/*
 * Edge.cpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */
#include "Edge.hpp"
#include "QueryGraph.hpp"

/*
	Creates a new edge and sets correct estimated selectivity. 
*/
Edge::Edge(Database* db, unsigned binding1, unsigned binding2, std::set<std::pair<std::string, std::string>> joinconditions, QueryGraph* graph) {
	Table& t1 = db->getTable(graph->getNode(binding1)->getName());
	Table& t2 = db->getTable(graph->getNode(binding2)->getName());		//get both tables in order to obtain their attribute's domain sizes

	selectivity = 1.0;	//No contition? => selectivity of 1.

	for(auto it = joinconditions.begin(); it != joinconditions.end(); it++) {
		Attribute a1 = t1.getAttribute(t1.findAttribute(it->first));
		Attribute a2 = t2.getAttribute(t2.findAttribute(it->second));

		selectivity = selectivity / (std::max(a1.getUniqueValues(), a2.getUniqueValues()));	//sets estimated selectivity as prior selectivity * selectivity of the join
	}
}

/*
	Returns the estimated selectivity of this edge.
*/
double Edge::getSelectivity() {
	return selectivity;
}
