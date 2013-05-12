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
   int findKeyword(std::string&,std::string);
   void parseSelections(std::string&, Query&);
   void parseRelations(std::string&, Query&);
   void parseJoinconditions(std::string&, Query&);

   Database* db;
};

#endif /* PARSER_HPP_ */
