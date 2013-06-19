#include "Database.hpp"
#include "parser/Parser.hpp"
#include "compiler/Compiler.hpp"
#include "compiler/ast/ASTPrinter.hpp"
#include "compiler/ast/ASTNode.hpp"
#include "compiler/strategies/QuickPickStrategy.hpp"
#include "compiler/SimpleExecutor.hpp"
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
int main()
{
   Database db;
   db.open("data/tpch/tpch");
   Parser p = Parser(&db);
   std::string qry = "select * from orders o, lineitem l, customer c, customer c2, orders k where ";
   std::cout << "Parsing query: " << qry << std::endl;
   Query query = p.parse(qry);
   std::cout << "Parsed." << std::endl;
   QuickPickStrategy s = QuickPickStrategy();
   Compiler c = Compiler(&s);
   ASTNode* tree = c.compile(query);
   std::cout << qry << std::endl;
   std::cout << "Query results in this AST: " << std::endl; 
   ASTPrinter::print(tree);
  /* std::cout << "---------------------------" << std::endl; 
   SimpleExecutor se = SimpleExecutor(tree, query, &db);
   std::cout << "Executing query:" << std::endl;
   se.execute();
   */
}
//---------------------------------------------------------------------------
