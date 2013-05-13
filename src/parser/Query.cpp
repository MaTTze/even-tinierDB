/*
 * Query.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: gollwitz
 */
#include "Query.hpp"


void Query::addProjection(std::string binding, std::string attribute)
{
	projections.push_back(std::make_pair(binding ,attribute));
}



void Query::addRelation(std::string rel, std::string binding)
{
	relations.insert(std::make_pair(binding, rel));
}



Query::Query()
{
}



void Query::addJoincondition(std::string bind1, std::string att1, std::string bind2, std::string att2)
{
	joinconditions.push_back(std::make_pair(std::make_pair(bind1, att1), std::make_pair(bind2, att2)));
}

bool Query::checkBinding(std::string& binding) {
	return relations.find(binding)!=relations.end();
}

void Query::addSelection(std::string binding, std::string attribute, std::string constant) {
	selections.push_back(std::make_pair(std::make_pair(binding, attribute), constant));
}

std::string Query::getRelation(std::string& binding) {
	std::string tmp = relations.find(binding)->second;
	return tmp;
}
