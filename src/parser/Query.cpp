/*
 * Query.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: gollwitz
 */
#include "Query.hpp"


void Query::addProjection(std::string binding, std::string attribute)
{
	unsigned bind = getRelation(binding);
	projections[bind].insert(attribute);
}



void Query::addRelation(std::string rel, std::string binding)
{
	bindings.insert(std::make_pair(binding, relations.size()));
	relations.push_back(rel);
}



Query::Query()
{
}



void Query::addJoincondition(std::string binding1, std::string att1, std::string binding2, std::string att2)
{
	unsigned bind1 = getRelation(binding1);
	unsigned bind2 = getRelation(binding2);
	if (bind1 == bind2) {
		if (att1>att2){ // same binding -> sort attributes
				std::string swap = att2;
				att2 = att1;
				att1 = swap;
		}
	} else if (bind1<bind2){ // sort binding
		unsigned swap_unsigned = bind2;
		bind2 = bind1;
		bind1 = swap_unsigned;
		std::string swap_string = att2;
		att2 = att1;
		att1 = swap_string;
	}

	joinconditions[std::make_pair(bind1, bind2)].insert(std::make_pair(att1, att2));
}

bool Query::checkBinding(std::string& binding) {
	return bindings.find(binding)!=bindings.end();
}

void Query::addSelection(std::string binding, std::string attribute, std::string constant) {
	unsigned bind = getRelation(binding);
	selections[bind].insert(std::make_pair(attribute, constant));
}

unsigned Query::getRelation(std::string& binding) {
	return bindings.find(binding)->second;
}

std::string Query::getRelationName(std::string& binding) {
	return relations[bindings.find(binding)->second];
}

std::unordered_map<std::string, unsigned> Query::getBindings() const {
	return bindings;
}

const std::map<std::pair<unsigned, unsigned>,std::set<std::pair<std::string, std::string> > >& Query::getJoinconditions() const {
	return joinconditions;
}

std::unordered_map<unsigned, std::unordered_set<std::string> > Query::getProjections() const {
	return projections;
}

const std::vector<std::string>& Query::getRelations() const {
	return relations;
}

std::unordered_map<unsigned, std::unordered_map<std::string, std::string> > Query::getSelections() const {
	return selections;
}
