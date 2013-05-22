/*
 * Edge.hpp
 *
 *  Created on: May 22, 2013
 *      Author: matthias
 */

#ifndef EDGE_HPP_
#define EDGE_HPP_
#include "Node.hpp"
#include <set>
#include <string>

class Edge {

public:
	Edge(Database*, unsigned, unsigned, std::set<std::pair<std::string, std::string>>, QueryGraph*);

	double getSelectivity();


private:
	std::set<std::pair<std::string,std::string>> conditions;
	double selectivity;
};




#endif /* EDGE_HPP_ */
