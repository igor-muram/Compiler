#pragma once
#include <vector>
#include <string>

#include "../Tables/TableManager.h"
#include "ID.h"

using namespace std;

typedef vector<string>::iterator error_iterator;

class lexer
{
public:
	lexer(table_manager* manager);
	void scan(string& source);

	vector<ID>::iterator tokens_begin() { return tokens.begin(); }
	vector<string>::iterator errors_begin() { return errors.begin(); }

	vector<ID>::iterator tokens_end() { return tokens.end(); }
	vector<string>::iterator errors_end() { return errors.end(); }



private:
	table_manager* manager;
	string::iterator pos;
	string::iterator end;

	int line = 0;

	bool need = false;

	vector<ID> tokens;
	void word();
	void number();
	void op();
	void delimiter();
	void comment();

	vector<string> errors;
	void error();
};