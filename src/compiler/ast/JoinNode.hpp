/*
 * JoinNode.hpp
 *
 *  Created on: May 14, 2013
 *      Author: matthias
 */

#ifndef JOINNODE_HPP_
#define JOINNODE_HPP_

class JoinNode:ASTNode {
public:
	Type getType();
private:
	ASTNode* parent;
	ASTNode* left;
	ASTNode* right;
	std::map<std::pair<unsigned,unsigned>, std::set<std::pair<std::string,std::string> > > joinconditions;
};



#endif /* JOINNODE_HPP_ */
