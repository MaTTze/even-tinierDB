#include "Database.hpp"
#include "parser/Parser.hpp"
#include "compiler/Compiler.hpp"
#include "compiler/ast/ASTPrinter.hpp"
#include "compiler/ast/ASTNode.hpp"
#include "compiler/strategies/QuickPickStrategy.hpp"
#include "compiler/strategies/DPsubStrategy.hpp"
#include "compiler/strategies/RepeatedRandomStrategy.hpp"
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

   std::string select = "*";
   std::string from = "orders o, lineitem l, customer c, customer c2, orders k";
   std::string where = "o.o_orderkey=l.l_orderkey and k.o_custkey=c2.c_custkey and o.o_custkey=c.c_custkey and c2.c_nationkey=c.c_nationkey and c.c_name=Customer#000014993";
   std::string qry = "select " + select + " from " + from + " where " + where;

   std::cout << "Parsing query: " << qry << std::endl;
   Query query = p.parse(qry);
   std::cout << "Parsed." << std::endl;

   RepeatedRandomStrategy s = RepeatedRandomStrategy();
   
   Compiler c = Compiler(&s);
   ASTNode* tree = c.compile(query);
   ASTPrinter::print(tree);

   std::cout << "######### Comparison to DP Algorithm: ##########" << std::endl;
   DPsubStrategy s2 = DPsubStrategy();

   Compiler c2 = Compiler(&s2);
   ASTNode* tree2 = c2.compile(query);
   ASTPrinter::print(tree2);

   /*
   std::cout << "---------------------------" << std::endl; 
   SimpleExecutor se = SimpleExecutor(tree, query, &db);
   std::cout << "Executing query:" << std::endl;
   se.execute();
   */
}
//---------------------------------------------------------------------------
