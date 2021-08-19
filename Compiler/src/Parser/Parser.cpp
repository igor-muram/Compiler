#include "Parser.h"

Parser::Parser(std::string grammarfile, table_manager* manager) : manager(manager)
{
	table = GrammarTable(grammarfile);
}

void Parser::Parse(token_iterator begin, token_iterator end)
{
	stack.push(-1);
	this->begin = begin;
	this->end = end;
	token = begin;

	while (token != end && row != -1)
	{
		std::string raw_token;
		if (manager->get_by_id(*token).get_name() == "operator")
		{
			raw_token = manager->get_by_id(*token).get_value();
		}
		else
		{
			raw_token = manager->get_by_id(*token).get_name();
		}

		if (std::find(table[row].terminals.begin(), table[row].terminals.end(), raw_token) != table[row].terminals.end())
		{
			int prev_row = row;
			if (table[row].accept)
				Accept(raw_token);

			if (table[row].stack)
				Stack();

			if (table[row].ret)
				Return();
			else
				Jump();
		}
		else if (!table[row].error)
		{
			row++;
		}
		else if (table[row].ret)
		{
			Return();
		}
		else
		{
			cout << "ERROR!" << raw_token << '[' << manager->get_by_id(*token).get_value() << ']' << " is not allowed!" << std::endl;
			token = end;
		}
	}
}

void Parser::Jump()
{
	row = table[row].jump;
}

void Parser::Accept(std::string raw_token)
{
	if (raw_token == "var")
	{
		if (token != begin && manager->get_by_id(*(token - 1)).get_name() == "int")
		{
			variable v;
			v.id = manager->get_by_id(*token).get_value();
			v.type = "int";

			if (std::find(variables.begin(), variables.end(), v) != variables.end())
			{
				std::cout << "ERROR! variable[" << v.id << "]" << " is already declared!" << std::endl;
			}
			else
			{
				std::string next = manager->get_by_id(*(token + 1)).get_name();
				if (manager->get_by_id(*(token + 1)).get_value() == "=")
					v.init = true;

				variables.push_back(v);
			}
		}
		else if (token + 1 != end && manager->get_by_id(*(token + 1)).get_value() == "=")
		{
			variable v;
			v.id = manager->get_by_id(*token).get_value();

			auto found = std::find(variables.begin(), variables.end(), v);
			if (found != variables.end())
			{
				found->init = true;
			}
			else
			{
				std::cout << "ERROR! variable[" << v.id << "]" << " is not declared!" << std::endl;
				token = end - 1;
			}
		}
		else
		{
			variable v;
			v.id = manager->get_by_id(*token).get_value();
			auto found = std::find(variables.begin(), variables.end(), v);
			if (found == variables.end())
			{
				std::cout << "ERROR! variable[" << v.id << "]" << " is not declared!" << std::endl;
				token = end - 1;
			}
			else if (!found->init)
			{
				std::cout << "ERROR! variable[" << v.id << "]" << " is not initialized!" << std::endl;
			}
		}
	}

	token++;
}

void Parser::Stack()
{
	stack.push(row + 1);
}

void Parser::Return()
{
	row = stack.top();
	stack.pop();
}