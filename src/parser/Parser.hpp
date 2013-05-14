/*
 * Parser.hpp
 *
 *  Created on: Apr 29, 2013
 *      Author: gollwitz
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_
#include "Query.hpp"
#include <string>

class Database;

class Parser {
public:
   Parser(Database* db);
   Query parse(std::string query);

private:
   size_t findKeyword(std::string&,std::string);
   void parseSelectClause(std::string&, Query&);
   void parseFromClause(std::string&, Query&);
   void parseWhereClause(std::string&, Query&);
   /// check if relation exists in DB
   bool checkRelation(std::string&);
   /// check if attribute exists in relation
   bool checkAttribute(std::string, std::string&, Query&);

   Database* db;
};

#endif /* PARSER_HPP_ */
