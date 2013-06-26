/*
 * ASTPrinter.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "CostCalculator.hpp"

#include "ast/TablescanNode.hpp"
#include "ast/SelectionNode.hpp"
#include "ast/JoinNode.hpp"
#include "ast/ProjectionNode.hpp"
#include "iostream"
#include "memory"

double CostCalculator::getCosts(ASTNode* node, QueryGraph* qg) {
	return std::get<0>(getCostsHelper(node, qg));
}

std::tuple<double, double, std::set<unsigned> > CostCalculator::getCostsHelper(ASTNode* node, QueryGraph* qg){
	unsigned binding;
	std::tuple<double, double, std::set<unsigned> > result1, result2;
	double size;
	std::set<unsigned> bindings;
	switch (node->getType()){
	case ASTNode::Type::Tablescan:
		binding = dynamic_cast<TablescanNode*>(node)->getRelation();
		 return std::make_tuple(0, qg->getNode(binding)->getECardinality(), std::set<unsigned>({binding}));
	case ASTNode::Type::Selection:
		return getCostsHelper(dynamic_cast<SelectionNode*>(node)->getChild(), qg);
	case ASTNode::Type::Join:
		result1 = getCostsHelper(dynamic_cast<JoinNode*>(node)->getLeft(), qg);
		result2 = getCostsHelper(dynamic_cast<JoinNode*>(node)->getRight(), qg);
		size = qg->evalSelectivity(std::get<2>(result1), std::get<2>(result2))*std::get<1>(result1)*std::get<1>(result2);
		bindings = std::get<2>(result1);
		bindings.insert(std::get<2>(result2).begin(), std::get<2>(result2).end());
		return std::make_tuple(size+std::get<0>(result1)+std::get<0>(result2), size, bindings);
	case ASTNode::Type::Projection:
		return getCostsHelper(dynamic_cast<ProjectionNode*>(node)->getChild(), qg);
	default:
		break;
	}
	return std::make_tuple(0,0,std::set<unsigned>());
}


