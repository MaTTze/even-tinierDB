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

class Query {
public:
   Query();
   void addRelation(std::string);
   void addSelection(std::string);
   void addJoincondition(std::string,std::string);

private:

   std::vector<std::pair<std::string,std::string> > relations;
   std::vector<std::string> selections;
   std::vector<std::pair<std::string,std::string> > joinconditions;
};

#endif /* QUERY_HPP_ */
