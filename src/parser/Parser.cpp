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
   if (selectPos == std::string::npos || selectPos == std::string::npos || selectPos == std::string::npos || selectPos > fromPos || fromPos > wherePos){
      std::cout << "Invalid Input" << std::endl;
      exit(0);
   }
   std::string selectClause = query.substr(selectPos+6,fromPos-selectPos-6);
   std::string fromClause = query.substr(fromPos+4,wherePos-fromPos-4);
   std::string whereClause = query.substr(wherePos+5);
   parseFromClause(fromClause, q);
   parseSelectClause(selectClause, q);
   parseWhereClause(whereClause, q);
   return q;
}


size_t Parser::findKeyword(std::string& query, std::string keyword)
{
   return query.find(keyword);
}

void Parser::parseFromClause(std::string& query, Query& result)
{
   size_t pos = -1;
   size_t first = 0;
   size_t space;
   std::string tmp, rel, binding;
   do {
      first = pos+1;
      pos = query.find(",",first);
      tmp = query.substr(first, pos-first);
      tmp = trim(tmp);
      space = tmp.find(" ");
      rel = tmp.substr(0, space);
      rel = trim(rel);
      binding = tmp.substr(space+1);
      binding = trim(binding);
      if (!checkRelation(rel)){
    	  std::cout << "Relation " << rel << " doesn't exist!";
    	  exit(0);
      }
      result.addRelation(rel, binding);
   } while (pos != std::string::npos);
}

void Parser::parseSelectClause(std::string& query, Query& result)
{
	if (query.find("*") != std::string::npos){
		// empty projections-map implies "return everything" -> nothing to do
		return;
	}
	size_t pos = -1;
	size_t first = 0;
	size_t dot;
	std::string tmp, attribute, binding;
	do {
	   first = pos+1;
	   pos = query.find(",",first);
	   tmp = query.substr(first, pos-first);
	   tmp = trim(tmp);
	   dot = tmp.find(".");
	   binding = tmp.substr(0, dot);
	   binding = trim(binding);
	   attribute = tmp.substr(dot+1);
	   attribute = trim(attribute);
	   if (!(result.checkBinding(binding) && checkAttribute(binding, attribute, result))){
	 	  std::cout << "Attribute " << binding << "." << attribute << " doesn't exist!" << std::endl;
	 	  exit(0);
	   }
	   result.addProjection(binding, attribute);
	} while (pos != std::string::npos);
}

void Parser::parseWhereClause(std::string& query, Query& result)
{
	size_t pos = -5;
	size_t first = 0;
	size_t equals = 0;
	size_t dot = 0;
	std::string tmp, left, right, binding1, attribute1, binding2, attribute2;
	do {
		first = pos+5;
		pos = query.find(" and ", first);
		tmp = query.substr(first, pos-first);
		tmp = trim(tmp);
		if (tmp == ""){
			return;
		}
		equals = tmp.find("=");
		left = tmp.substr(0, equals);
		left = trim(left);
		right = tmp.substr(equals+1);
		right = trim(right);
		dot = left.find(".");
		binding1 = left.substr(0, dot);
		binding1 = trim(binding1);
		attribute1 = left.substr(dot+1);
		attribute1 = trim(attribute1);
		if (!(result.checkBinding(binding1) && checkAttribute(binding1, attribute1, result))){
			std::cout << "Attribute " << binding1 << "." << attribute1 << " doesn't exist!" << std::endl;
			exit(0);
		}
		dot = right.find(".");
		if (dot == std::string::npos){
			result.addSelection(binding1, attribute1, right);
		}else{
			if (isdigit(right[0])){
				result.addSelection(binding1, attribute1, right);
			} else {
				binding2 = right.substr(0, dot);
				binding2 = trim(binding2);
				attribute2 = right.substr(dot+1);
				attribute2 = trim(attribute2);
				if (!(result.checkBinding(binding2) && checkAttribute(binding2, attribute2, result))){
					std::cout << "Attribute " << binding2 << "." << attribute2 << " doesn't exist!" << std::endl;
					exit(0);
				}
				result.addJoincondition(binding1,attribute1, binding2, attribute2);
			}
		}
	} while (pos != std::string::npos);
}

bool Parser::checkRelation(std::string& rel) {
	return db->hasTable(rel);
}

bool Parser::checkAttribute(std::string binding, std::string& attribute, Query& result) {
	return (db->getTable(result.getRelationName(binding)).findAttribute(attribute) >= 0);
}
