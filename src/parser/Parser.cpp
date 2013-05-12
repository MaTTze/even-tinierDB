/*
 * Parser.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: gollwitz
 */
#include "Parser.hpp"
#include "Database.hpp"
#include <ctype.h>

inline std::string& trim(std::string& str)
{
str.erase(0, str.find_first_not_of(' '));       //leading spaces
str.erase(str.find_last_not_of(' ')+1);         //trailing spaces
return str;
}

Parser::Parser(Database *db):db(db)
{
}

Query Parser::parse(std::string query)
{
   Query q;
   //remove leading and trailing whitespaces
   trim(query);
   //find select
   size_t selectPos = findKeyword(query, std::string("select"));
   size_t fromPos = findKeyword(query, std::string("from"));
   size_t wherePos = findKeyword(query, std::string("where"));
   if (selectPos == std::string::npos || selectPos == std::string::npos || selectPos == std::string::npos){
      std::cout << "Invalid Input" << std::endl;
      exit(0);
   }
   std::string selections = query.substr(selectPos+6,fromPos-selectPos-6);
   std::string relations = query.substr(fromPos+4,wherePos-fromPos-4);
   std::string joinconditions = query.substr(wherePos+5);
   parseRelations(relations, q);
   parseSelections(selections, q);
   parseJoinconditions(joinconditions, q);
   return q;
}


size_t Parser::findKeyword(std::string& query, std::string keyword)
{
   return query.find(keyword);
}

void Parser::parseRelations(std::string& query, Query& result)
{
   size_t pos = -1;
   size_t first = 0;
   size_t space;
   std::string tmp, rel, binding;
   do {
      first = pos+1;
      pos = query.find(",");
      tmp = query.substr(first, pos-first);
      tmp = trim(tmp);
      space = tmp.find(" ");
      rel = tmp.substr(first, space-first);
      rel = trim(rel);
      binding = tmp.substr(space+1, pos-space-1);
      binding = trim(binding);
      if (!checkRelation(rel)){
    	  std::cout << "Relation " << rel << " doesn't exist!";
    	  exit(0);
      }
      result.addRelation(rel, binding);
   } while (!pos == std::string::npos);
}

void Parser::parseSelections(std::string& query, Query& result)
{
   //if (query.find("*") != std::string::npos){
   //   return;
   //}
   //unsigned pos = -1;
   //unsigned first = 0;
   //std::string tmp;
   //do {
   //   first = pos+1;
   //   pos = query.find(",");
   //   tmp = query.substr(first, pos-first);
   //   checkAttribute(tmp);
   //   result.addSelection(tmp);
   //} while (!pos == std::string::npos);
	if (query.find("*") != std::string::npos){
	   return;
	}
	size_t pos = -1;
	size_t first = 0;
	size_t dot;
	std::string tmp, attribute, binding;
	do {
	   first = pos+1;
	   pos = query.find(",");
	   tmp = query.substr(first, pos-first);
	   tmp = trim(tmp);
	   dot = tmp.find(".");
	   binding = tmp.substr(first, dot-first);
	   binding = trim(binding);
	   attribute = tmp.substr(dot+1, pos-dot-1);
	   attribute = trim(attribute);
	   if (!(result.checkBinding(binding) && checkAttribute(binding, attribute, result))){
	 	  std::cout << "Attribute " << binding << "." << attribute << " doesn't exist!" << std::endl;
	 	  exit(0);
	   }
	   result.addSelection(binding, attribute);
	} while (!pos == std::string::npos);
}

void Parser::parseJoinconditions(std::string& query, Query& result)
{
	size_t pos = -1;
	size_t first = 0;
	size_t equals = 0;
	size_t dot = 0;
	std::string tmp, left, right, binding1, attribute1, binding2, attribute2;
	do {
		first = pos+1;
		pos = query.find(" and ");
		tmp = query.substr(first, pos-first);
		tmp = trim(tmp);
		equals = tmp.find("=");
		left = tmp.substr(first, equals-first);
		left = trim(left);
		right = tmp.substr(equals+1, pos-equals-1);
		right = trim(right);
		dot = left.find(".");
		binding1 = left.substr(first, dot-first);
		binding1 = trim(binding1);
		attribute1 = left.substr(dot+1, pos-dot-1);
		attribute1 = trim(attribute1);
		if (!(result.checkBinding(binding1) && checkAttribute(binding1, attribute1, result))){
			std::cout << "Attribute " << binding1 << "." << attribute1 << " doesn't exist!" << std::endl;
			exit(0);
		}
		dot = right.find(".");
		if (dot == std::string::npos){
			result.addPredicate(binding1, attribute1, right);
		}else{
			if (isdigit(right[0])){
				result.addPredicate(binding1, attribute1, right);
			} else {
				binding2 = right.substr(first, dot-first);
				binding2 = trim(binding1);
				attribute2 = right.substr(dot+1, pos-dot-1);
				attribute2 = trim(attribute2);
				if (!(result.checkBinding(binding2) && checkAttribute(result.getRelation(binding2), attribute2, result))){
					std::cout << "Attribute " << binding2 << "." << attribute2 << " doesn't exist!" << std::endl;
					exit(0);
				}
				result.addJoincondition(binding1,attribute1, binding2, attribute2);
			}
		}
	} while (!pos == std::string::npos);
}

bool Parser::checkRelation(std::string& rel) {
	return db->hasTable(rel);
}

bool Parser::checkAttribute(std::string binding, std::string& attribute, Query& result) {
	return (db->getTable(result.getRelation(binding)).findAttribute(attribute) >= 0);
}
