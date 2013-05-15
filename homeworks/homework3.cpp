#include "Database.hpp"
#include "parser/Parser.hpp"
#include "compiler/SimpleCompiler.hpp"
#include "compiler/SimpleExecutor.hpp"
#include "compiler/ast/ASTPrinter.hpp"
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
   std::string qry = "select * from studenten s where s.name=Schopenhauer";
   ASTPrinter::print(c.compile(p.parse(qry)),0);
   SimpleExecutor e = SimpleExecutor(c.compile(p.parse(qry)), p.parse(qry) ,&db);
   e.execute();
}
//---------------------------------------------------------------------------
