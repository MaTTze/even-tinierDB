/*
 * Query.hpp
 *
 *  Created on: Apr 29, 2013
 *      Author: gollwitz
 */

#ifndef QUERY_HPP_
#define QUERY_HPP_
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>
class Query {
public:
   Query();
   void addRelation(std::string, std::string);
   void addProjection(std::string, std::string);
   void addJoincondition(std::string,std::string,std::string,std::string);
   void addSelection(std::string,std::string,std::string);
   /// check existence of binding
   bool checkBinding(std::string&);
   /// return the unsigned representation for binding
   unsigned getRelation(std::string&);
   /// return the table name for binding
   std::string getRelationName(std::string&);

private:

   /// bindings and their unsigned representation
   std::unordered_map<std::string,unsigned> bindings;
   /// corresponding tablenames
   std::vector<std::string> relations;
   /// binding.attribute from select clause
   std::unordered_map<unsigned,std::unordered_set<std::string> > projections;
   /// bind1.att1=bind2.att2 from where clause
   std::map<std::pair<unsigned,unsigned>, std::set<std::pair<std::string,std::string> > > joinconditions; //not unordered to avoid hash<pair> problematic
   /// binding.attribute=constant from where clause
   std::unordered_map<unsigned,std::unordered_map<std::string,std::string> > selections;
};

#endif /* QUERY_HPP_ */
