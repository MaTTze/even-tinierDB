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
   bool checkBinding(std::string&);
   std::string getRelation(std::string&);

private:

   std::unordered_map<std::string,std::string> relations;
   std::unordered_map<std::string,std::unordered_set<std::string> > projections;
   //std::vector<std::pair<std::pair<std::string,std::string>,std::pair<std::string,std::string> > > joinconditions;
   std::map<std::pair<std::string,std::string>, std::set<std::pair<std::string,std::string> > > joinconditions; //not unordered to avoid hash<pair> problematic
   //std::vector<std::pair<std::pair<std::string,std::string>, std::string> > selections;
   std::unordered_map<std::string,std::unordered_map<std::string,std::string> > selections;
};

#endif /* QUERY_HPP_ */
