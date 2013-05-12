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
   Table& professors=db.getTable("professoren");
   Table& lectures=db.getTable("vorlesungen");
   Table& attends=db.getTable("hoeren");

   unique_ptr<Tablescan> a1(new Tablescan(attends));
   const Register* a1sid=a1->getOutput("matrnr");
   const Register* a1lid=a1->getOutput("vorlnr");

   unique_ptr<Tablescan> a2(new Tablescan(attends));
   const Register* a2sid=a2->getOutput("matrnr");
   const Register* a2lid=a2->getOutput("vorlnr");

   unique_ptr<Tablescan> l1(new Tablescan(lectures));
   const Register* l1pid=l1->getOutput("gelesenvon");
   const Register* l1lid=l1->getOutput("vorlnr");

   unique_ptr<Tablescan> l2(new Tablescan(lectures));
   const Register* l2pid=l2->getOutput("gelesenvon");
   const Register* l2lid=l2->getOutput("vorlnr");

   unique_ptr<Tablescan> p(new Tablescan(professors));
   const Register* pname=p->getOutput("name");
   const Register* pid=p->getOutput("persnr");

   // attends1 join with attends2 (sid not equal)
   unique_ptr<CrossProduct> cp1(new CrossProduct(move(a1),move(a2)));
   unique_ptr<Chi> chi(new Chi(move(cp1),Chi::NotEqual,a1sid,a2sid));
   const Register* chiResult=chi->getResult();
   unique_ptr<Selection> select1(new Selection(move(chi),chiResult));
   unique_ptr<Projection> project1(new Projection(move(select1),{a1lid,a2lid}));

   // professors join with lectures1
   unique_ptr<CrossProduct> cp2(new CrossProduct(move(p),move(l1)));
   unique_ptr<Selection> select2(new Selection(move(cp2),pid,l1pid));
   unique_ptr<Projection> project2(new Projection(move(select2),{pname,pid,l1lid}));

   //  project2 join with lectures2
   unique_ptr<CrossProduct> cp3(new CrossProduct(move(project2),move(l2)));
   unique_ptr<Selection> select3(new Selection(move(cp3),pid,l2pid));
   unique_ptr<Projection> project3(new Projection(move(select3),{pname,l1lid,l2lid}));

   //project3 join with project1
   unique_ptr<CrossProduct> cp4(new CrossProduct(move(project3),move(project1)));
   unique_ptr<Selection> select4_1(new Selection(move(cp4),l1lid,a1lid));
   unique_ptr<Selection> select4_2(new Selection(move(select4_1),l2lid,a2lid));
   unique_ptr<Projection> project4(new Projection(move(select4_2),{pname}));

   Printer out(move(project4));

   out.open();
   while (out.next());
   out.close();

}
//---------------------------------------------------------------------------
