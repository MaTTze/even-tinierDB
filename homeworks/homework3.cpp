#include "Database.hpp"
#include "parser/Parser.hpp"
#include "compiler/SimpleCompiler.hpp"
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
   c.compile(p.parse(std::string("select prof.name, vorl.name from professoren prof, vorlesungen vorl where prof.persnr=12 and vorl.gelesenvon=prof.persnr")));
}
//---------------------------------------------------------------------------
