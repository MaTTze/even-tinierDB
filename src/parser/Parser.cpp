/*
 * Parser.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: gollwitz
 */
#include "Parser.hpp"
#include "Database.hpp"

inline std::string trim(std::string& str)
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
   Query q();
   //remove leading and trailing whitespaces
   trim(query);
   //find select
   int selectPos = findKeyword(query, std::string("select"));
   int fromPos = findKeyword(query, std::string("from"));
   int wherePos = findKeyword(query, std::string("where"));
   if (selectPos == std::string::npos || selectPos == std::string::npos || selectPos == std::string::npos){
      std::cout << "Invalid Input";
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


int Parser::findKeyword(std::string& query, std::string keyword)
{
   return query.find(keyword);
}

void Parser::parseRelations(std::string& query, Query& result)
{
   unsigned pos = -1;
   unsigned first = 0;
   unsigned space;
   std::string tmp, rel, binding;
   do {
      first = pos+1;
      pos = query.find(",");
      tmp = query.substr(first, pos-first);
      space = tmp.find(" ");
      rel = tmp.substr(first, space-first);
      binding = tmp.substr(space+1, pos-space-1);
      checkRelation(tmp);
      result.addRelation(rel, binding);
   } while (!pos == std::string::npos);
}

void Parser::parseSelections(std::string& query, Query& result)
{
   if (query.find("*") != std::string::npos){
      return;
   }
   unsigned pos = -1;
   unsigned first = 0;
   std::string tmp;
   do {
      first = pos+1;
      pos = query.find(",");
      tmp = query.substr(first, pos-first);
      checkAttribute(tmp);
      result.addSelection(tmp);
   } while (!pos == std::string::npos);
}

void Parser::parseJoinconditions(std::string& query, Query& result)
{
   unsigned pos = -1;
      unsigned first = 0;
      std::string tmp, att1, att2;
      do {
         first = pos+5;
         pos = query.find(" and ");
         tmp = query.substr(first, pos-first);
         equals = tmp.find("=");
         att1 = tmp.substr(first, equals-first);
         att2 = tmp.substr(equals+1, pos-equals-1);
         checkAttribute(tmp);
         result.addRelation(rel, binding);
      } while (!pos == std::string::npos);
}





