#pragma once

#include <string>
#include <vector>
#include <stack>

#include "GrammarTable.h"
#include "../Lexer/Lexeme.h"
#include "../Lexer/ID.h"
#include "../Tables/TableManager.h"

typedef std::vector<std::string>::iterator error_iterator;

struct variable
{
	std::string type = "";
	std::string id = "";
	bool init = false;

	bool operator==(const variable& v)
	{
		return id == v.id;
	}
};

class Parser
{
public:
	Parser(std::string grammarfile, table_manager* manager);
	void Parse(token_iterator begin, token_iterator end);

private:
	table_manager* manager;
	GrammarTable table;
	token_iterator begin, end;

	int row = 0;
	token_iterator token;
	std::stack<int> stack;

	std::vector<variable> variables;

	// For table
	void Jump();
	void Accept(std::string raw_token);
	void Stack();
	void Return();
};