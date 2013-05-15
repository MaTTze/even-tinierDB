/*
 * TablescanNode.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */


Type TablescanNode::getType() {

}

TablescanNode::TablescanNode(std::string t, unsigned r) {
	tableName = t;
	relation = r;
}

std::string TablescanNode::getTableName() {
	return tableName;
}

unsigned TablescanNode::getRelation() {
	return relation;
}

