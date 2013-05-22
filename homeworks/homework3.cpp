#include "Database.hpp"
#include "parser/Parser.hpp"
#include "compiler/SimpleCompiler.hpp"
#include "compiler/SimpleExecutor.hpp"
#include "compiler/ast/ASTPrinter.hpp"
#include "compiler/ast/ASTNode.hpp"
#include "compiler/QueryGraphCompiler.hpp"
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
int main()
{
   Database db;
   db.open("data/uni");
   Parser p = Parser(&db);
   std::string qry = "select v.titel, v.gelesenvon, p.persnr, p.name from vorlesungen v, professoren p where v.titel=Ethik and v.gelesenvon=p.persnr";
   Query query = p.parse(qry);
   /*
   SimpleCompiler c = SimpleCompiler();
   ASTNode* ASTRoot = c.compile(query);
   ASTPrinter::print(ASTRoot,0);
   SimpleExecutor e = SimpleExecutor(ASTRoot, query ,&db);
   */
   QueryGraphCompiler qgc = QueryGraphCompiler(&db);
   QueryGraph* qg = qgc.compile(query);
   //qg->print();
   //e.execute();
}
//---------------------------------------------------------------------------
