/*
 * Query.hpp
 *
 *  Created on: Apr 29, 2013
 *      Author: gollwitz
 */

#ifndef QUERY_HPP_
#define QUERY_HPP_
#include <string>
#include <vector>
#include <map>

class Query {
public:
   Query();
   void addRelation(std::string, std::string);
   void addSelection(std::string, std::string);
   void addJoincondition(std::string,std::string,std::string,std::string);
   void addPredicate(std::string,std::string,std::string);
   bool checkBinding(std::string&);
   std::string getRelation(std::string&);

private:

   std::map<std::string,std::string> relations;
   std::vector<std::pair<std::string,std::string> > selections;
   std::vector<std::pair<std::pair<std::string,std::string>,std::pair<std::string,std::string> > > joinconditions;
   std::vector<std::pair<std::pair<std::string,std::string>, std::string> > predicates;
};

#endif /* QUERY_HPP_ */
