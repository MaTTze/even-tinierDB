/*
 * Query.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: gollwitz
 */
#include "Query.hpp"


void Query::addProjection(std::string binding, std::string attribute)
{
	projections[binding].insert(attribute);
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
	if (bind1 == bind2) {
		if (att1>att2){ // same binding -> sort attributes
				std::string swap = att2;
				att2 = att1;
				att1 = swap;
		}
	} else if (bind1>bind2){ // sort bindings
		std::string swap = bind2;
		bind2 = bind1;
		bind1 = swap;
		swap = att2;
		att2 = att1;
		att1 = swap;
	}

	joinconditions[std::make_pair(bind1, bind2)].insert(std::make_pair(att1, att2));
}

bool Query::checkBinding(std::string& binding) {
	return relations.find(binding)!=relations.end();
}

void Query::addSelection(std::string binding, std::string attribute, std::string constant) {
	selections[binding].insert(std::make_pair(attribute, constant));
}

std::string Query::getRelation(std::string& binding) {
	std::string tmp = relations.find(binding)->second;
	return tmp;
}
