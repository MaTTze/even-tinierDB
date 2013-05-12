#include "Database.hpp"
#include "operator/Tablescan.hpp"
#include "operator/CrossProduct.hpp"
#include "operator/Selection.hpp"
#include "operator/Projection.hpp"
#include "operator/Printer.hpp"
#include "operator/Chi.hpp"
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
int main()
{
   Database db;
   db.open("data/uni");
   Table& studenten=db.getTable("studenten");
   Table& attends=db.getTable("hoeren");

   unique_ptr<Tablescan> s1(new Tablescan(studenten));
   const Register* s1id=s1->getOutput("matrnr");
   const Register* s1name=s1->getOutput("name");

   unique_ptr<Tablescan> s2(new Tablescan(studenten));
   const Register* s2id=s2->getOutput("matrnr");
   const Register* s2name=s2->getOutput("name");

   unique_ptr<Tablescan> a1(new Tablescan(attends));
   const Register* a1sid=a1->getOutput("matrnr");
   const Register* a1lid=a1->getOutput("vorlnr");

   unique_ptr<Tablescan> a2(new Tablescan(attends));
   const Register* a2sid=a2->getOutput("matrnr");
   const Register* a2lid=a2->getOutput("vorlnr");

   // lectures attended by students
   unique_ptr<CrossProduct> cp1(new CrossProduct(move(s1),move(a1)));
   unique_ptr<Selection> select1(new Selection(move(cp1),s1id,a1sid));
   unique_ptr<Projection> project1(new Projection(move(select1),{s1id,s1name,a1lid}));

   // find student Schopenhauer
   Register schopenhauer; schopenhauer.setString("Schopenhauer");
   unique_ptr<Selection> select_s2name(new Selection(move(s2),s2name,&schopenhauer));
   unique_ptr<Projection> project_s2(new Projection(move(select_s2name),{s2id}));
   // lectures attended by Schopenhauer
   unique_ptr<CrossProduct> cp2(new CrossProduct(move(project_s2),move(a2)));
   unique_ptr<Selection> select2(new Selection(move(cp2),s2id,a2sid));
   unique_ptr<Projection> project2(new Projection(move(select2),{s2id,a2lid}));

   unique_ptr<CrossProduct> cp3(new CrossProduct(move(project1),move(project2)));
   unique_ptr<Selection> select3lid(new Selection(move(cp3),a1lid,a2lid));
   unique_ptr<Chi> chi(new Chi(move(select3lid),Chi::NotEqual,s1id,s2id));
   const Register* chiResult=chi->getResult();
   unique_ptr<Selection> select3sid(new Selection(move(chi),chiResult));
   unique_ptr<Projection> project3(new Projection(move(select3sid),{s1name}));

   Printer out(move(project3));

   out.open();
   while (out.next());
   out.close();

}
//---------------------------------------------------------------------------
