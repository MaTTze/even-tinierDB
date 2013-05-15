/*
 * SimpleCompiler.cpp
 *
 *  Created on: May 15, 2013
 *      Author: matthias
 */
#include "SimpleCompiler.hpp"

ASTNode* SimpleCompiler::compile(Query query) {
	q = query;
	generateTablescans();
	generateSelections();
	generateCrossproducts();
	pushdownSelections();
	generateProjections();
	return currentRoot;
}

void SimpleCompiler::generateTablescans() {
	auto relations = q.getRelations();
	for (auto it = relations.begin(); it != relations.end(); it++) {

	}
}

void SimpleCompiler::generateSelections() {
}

void SimpleCompiler::generateCrossproducts() {
}

void SimpleCompiler::pushdownSelections() {
}

void SimpleCompiler::generateProjections() {
}
