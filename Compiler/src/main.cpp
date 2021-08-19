#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Lexer/Lexer.h"
#include "Lexer/ID.h"

#include "Parser/Parser.h"
#include "ASTBuilder/ASTBuilder.h"

#include "CodeGenerator/CodeGenerator.h"

using namespace std;

int main()
{
	ifstream in("input/program.txt");
	stringstream buffer;
	buffer << in.rdbuf();
	string source = buffer.str();
	in.close();

	table_manager manager;
	lexer lex(&manager);
	lex.scan(source);

	token_iterator begin = lex.tokens_begin();
	token_iterator end = lex.tokens_end();

	Parser parser("input/grammar_table.txt", &manager);
	parser.Parse(begin, end);

	ASTBuilder ast(&manager);
	ast.Build(begin, end);

	for (auto lex : ast.postfix)
	{
		std::cout << lex << " ";
	}
	
	std::cout << std::endl;

	CodeGenerator generator(&manager);
	generator.Generate(ast.postfix, parser.variables);

	for (auto iterator = generator.HeadBegin(); iterator != generator.HeadEnd(); iterator++)
	{
		std::cout << *iterator << std::endl;
	}

	for (auto iterator = generator.BodyBegin(); iterator != generator.BodyEnd(); iterator++)
	{
		std::cout << *iterator << std::endl;
	}
	cin.get();
}