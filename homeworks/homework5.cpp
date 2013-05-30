#include "Database.hpp"
#include "parser/Parser.hpp"
#include "compiler/Compiler.hpp"
#include "compiler/ast/ASTPrinter.hpp"
#include "compiler/ast/ASTNode.hpp"
#include "compiler/strategies/GOOStrategy.hpp"
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
int main()
{
   Database db;
   db.open("data/uni");
   Parser p = Parser(&db);
   std::string qry = "select v.titel, v.gelesenvon, p.persnr, p.name from vorlesungen v, professoren p, hoeren h where v.titel=p.persnr and v.gelesenvon=p.persnr and h.vorlnr = v.vorlnr";
   Query query = p.parse(qry);


   GOOStrategy s = GOOStrategy();
   Compiler c = Compiler(&s);
   ASTPrinter::print(c.compile(query), 0);
}
//---------------------------------------------------------------------------
