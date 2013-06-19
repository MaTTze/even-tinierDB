#include "Database.hpp"
#include "parser/Parser.hpp"
#include "compiler/Compiler.hpp"
#include "compiler/ast/ASTPrinter.hpp"
#include "compiler/ast/ASTNode.hpp"
#include "compiler/strategies/GOOStrategy.hpp"
#include "compiler/SimpleExecutor.hpp"
#include "compiler/randomized/DyckGenerator.hpp"
#include <iostream>
#include <vector>
#include <sys/time.h>


#define TREESIZE 4		// only works up to 33, as C(34) > ULL_MAX

int main()
{
	struct timeval time; 
    gettimeofday(&time,NULL);
    std::srand((time.tv_sec) + (time.tv_usec));

	unsigned long long c = DyckGenerator::getCatalanNumber(TREESIZE);
	unsigned long long treeNum = std::rand() % c;

	std::cout << "Generating tree " << treeNum << " with " << TREESIZE << " inner nodes:" << std::endl;

	//Generates the list representation of the Dyck word. 
  	//std::vector<int> encoding = DyckGenerator::generateWord(TREESIZE,treeNum);
  	  	std::vector<int> encoding = DyckGenerator::generateWord(5,6);

   	DyckGenerator::printWord(encoding);
}
//---------------------------------------------------------------------------
