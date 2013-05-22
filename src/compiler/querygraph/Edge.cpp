/*
 * Edge.cpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */


Edge(Database* db, unsigned binding1, unsigned binding2, std::set<std::pair<std::string, std::string>> joinconditions, QueryGraph* graph) {
	Table& t1 = db->getTable(graph->getNode(binding1)->getName());
	Table& t2 = db->getTable(graph->getNode(binding2)->getName());

	selectivity = 1.0;


	for(auto it = joinconditions.begin(); it != joinconditions.end(); it++) {
		Attribute a1 = t1.getAttribute(t1.findAttribute(it->first));
		Attribute a2 = t2.getAttribute(t2.findAttribute(it->second));

		selectivity = selectivity / (std::max(a1.getUniqueValues()), a2.getUniqueValues()));
	}
}


double getSelectivity() {
	return selectivity;
}
