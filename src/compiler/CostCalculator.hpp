/*
 * AST.hpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */

#ifndef COSTCALCULATOR_HPP_
#define COSTCALCULATOR_HPP_
#include "ast/ASTNode.hpp"
#include "querygraph/QueryGraph.hpp"

class CostCalculator {
public:
	static double getCosts(ASTNode*, QueryGraph*);
private:
	static std::tuple<double, double, std::set<unsigned> > getCostsHelper(ASTNode*, QueryGraph*);
};


#endif /* AST_HPP_ */
