#include "Database.hpp"
#include "parser/Parser.hpp"
#include "compiler/Compiler.hpp"
#include "compiler/ast/ASTPrinter.hpp"
#include "compiler/ast/ASTNode.hpp"
#include "compiler/strategies/GOOStrategy.hpp"
#include "compiler/SimpleExecutor.hpp"
#include "compiler/randomized/DyckGenerator.hpp"
#include <iostream>
#include <list>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
int main()
{
  	std::list<int> encoding = DyckGenerator::generateWord(100,7);

   	DyckGenerator::printWord(encoding);
}
//---------------------------------------------------------------------------
