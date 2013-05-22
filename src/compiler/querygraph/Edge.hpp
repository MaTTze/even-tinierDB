/*
 * Edge.hpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */

#ifndef EDGE_HPP_
#define EDGE_HPP_
#include "Database.hpp"
#include <set>
#include <string>

class QueryGraph;

class Edge {

public:
	//Creates a new edge and sets correct estimated selectivity. 
	Edge(Database*, unsigned, unsigned, std::set<std::pair<std::string, std::string> >, QueryGraph*);

	//Returns the estimated selectivity of this edge.
	double getSelectivity();


private:
	std::set<std::pair<std::string,std::string>> conditions;	//the joins this edge is representing in the query graph
	double selectivity;		//the resulting estimated selectivity of all joins
};	




#endif /* EDGE_HPP_ */
