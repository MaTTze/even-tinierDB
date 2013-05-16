#include "Database.hpp"
#include "parser/Parser.hpp"
#include "compiler/SimpleCompiler.hpp"
#include "compiler/SimpleExecutor.hpp"
#include "compiler/ast/ASTPrinter.hpp"
#include "compiler/ast/ASTNode.hpp"
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
int main()
{
   Database db;
   db.open("data/uni");
   Parser p = Parser(&db);
   SimpleCompiler c = SimpleCompiler();
   std::string qry = "select s.name, s.matrnr from studenten s where s.matrnr=24002";
   Query query = p.parse(qry);
   ASTNode* ASTRoot = c.compile(query);
   ASTPrinter::print(ASTRoot,0);
   SimpleExecutor e = SimpleExecutor(ASTRoot, query ,&db);
   e.execute();
}
//---------------------------------------------------------------------------
