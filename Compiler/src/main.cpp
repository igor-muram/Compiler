#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Lexer/Lexer.h"
#include "Lexer/ID.h"

using namespace std;

int main()
{
	ifstream in("input/program.txt");
	stringstream buffer;
	buffer << in.rdbuf();
	string source = buffer.str();

	in.close();
	lexer lex;
	lex.scan(source);

	token_iterator begin = lex.tokens_begin();
	token_iterator end = lex.tokens_end();

	error_iterator errors_begin = lex.errors_begin();
	error_iterator errors_end = lex.errors_end();

	while (begin != end)
	{
		
		cout << *begin << "\t" << lex.get_table_manager()->get_by_id(*begin) << endl;
		begin++;
	}

	cout << endl;

	while (errors_begin != errors_end)
	{
		cout << *errors_begin << endl;
		errors_begin++;
	}

	cin.get();
}