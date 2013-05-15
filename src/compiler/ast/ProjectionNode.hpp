/*
 * SelectionNode.hpp
 *
 *  Created on: May 14, 2013
 *      Author: matthias
 */

#ifndef PROJECTIONNODE_HPP_
#define PROJECTIONNODE_HPP_

#include "ASTNode.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>

class ProjectionNode: public ASTNode {
public:
	ASTNode::Type getType();
	ProjectionNode(ASTNode*, std::unordered_map<unsigned,std::unordered_set<std::string> >);
	ASTNode* getChild();
	std::unordered_map<unsigned,std::unordered_set<std::string>> getProjections();
	void setChild(ASTNode*);
	void addProjection(unsigned,std::unordered_set<std::string>);
private:
	ASTNode* child;
	std::unordered_map<unsigned,std::unordered_set<std::string> > projections;
};


#endif /* SELECTIONNODE_HPP_ */
