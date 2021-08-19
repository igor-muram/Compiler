#pragma once

#include <string>
#include "ConstTable.h"
#include "HashTable.h"

using namespace std;
class table_manager
{
public:
	table_manager()
	{
		keywords = new const_table(0);
		operators = new const_table(1);
		delimiters = new const_table(2);
		identificators = new hash_table(3, 5);
		constants = new hash_table(4, 5);
		spaces = new const_table(6);

		keywords->put("main");
		keywords->put("return");
		keywords->put("void");
		keywords->put("int");
		keywords->put("if");
		keywords->put("else");

		operators->put("+");
		operators->put("-");
		operators->put("*");
		operators->put("==");
		operators->put("!=");
		operators->put("!");
		operators->put(">");
		operators->put("<");
		operators->put("=");

								// везде пробел, таб, перенос строки
		delimiters->put(",");	// буква, /
		delimiters->put(";");	// буква, }, /
		delimiters->put("{");	// буква, }, /
		delimiters->put("}");	// буква, }, /
		delimiters->put("(");	// буква, цифра, (, ), /
		delimiters->put(")");	// ;, запятая, ), {, /

		spaces->put("\t");
		spaces->put("\n");
		spaces->put(" ");
	}

	bool is_keyword(string word)
	{
		return keywords->contains(word);
	}

	bool is_operator(char symbol)
	{
		std::string word;
		word.push_back(symbol);
		return operators->contains(word);
	}


	bool is_operator(std::string symbol)
	{
		return operators->contains(symbol);
	}

	bool is_delimiter(char symbol)
	{
		string word;
		word.push_back(symbol);
		return delimiters->contains(word);
	}

	bool is_space(char symbol)
	{
		string word;
		word.push_back(symbol);
		return spaces->contains(word);
	}

	bool is_const(std::string word)
	{
		lexeme lex(word);
		if (isdigit(word[0]))
			lex.set_type(VARTYPE::CONST);

		return constants->contains(lex);
	}

	ID get_id(string word)
	{
		ID id;

		if (keywords->contains(word))
			keywords->get_id(word, id);
		else if (operators->contains(word))
			operators->get_id(word, id);
		else if (delimiters->contains(word))
			delimiters->get_id(word, id);
		else
			get_word_id(word, id);

		return id;
	}

	// [TODO] : Переделать возврат строки, добавить isValid
	lexeme get_by_id(ID& id)
	{
		string word;

		if (id.table_no == 0)
		{
			keywords->get_by_id(id, word);
			return lexeme(word);
		}
		
		if (id.table_no == 1)
		{
			operators->get_by_id(id, word);
			return lexeme("operator", VARTYPE::VOID, word);
		}

		if (id.table_no == 2)
		{
			delimiters->get_by_id(id, word);
			return lexeme(word);
		}

		if (id.table_no == 3)
		{
			lexeme lex;
			identificators->get_by_id(id, lex);
			lex.set_name("var");
			return lex;
		}

		if (id.table_no == 4)
		{
			lexeme lex;
			constants->get_by_id(id, lex);
			lex.set_name("const");
			return lex;
		}
	}

	void print_tables()
	{
		cout << "keywords:" << endl;
		keywords->print();
		cout << endl << endl;

		cout << "operators:" << endl;
		operators->print();
		cout << endl << endl;

		cout << "delimiters:" << endl;
		delimiters->print();
		cout << endl << endl;

		cout << "identificators:" << endl;
		identificators->print();
		cout << endl << endl;

		cout << "constants:" << endl;
		constants->print();
		cout << endl << endl;
	}

private:
	const_table* keywords;
	const_table* operators;
	const_table* delimiters;
	const_table* spaces;
	hash_table* identificators;
	hash_table* constants;

	void get_word_id(string word, ID& id)
	{
		lexeme lex(word);
		if (isdigit(word[0]))
			lex.set_type(VARTYPE::CONST);

		if (lex.get_type() != VARTYPE::CONST)
		{
			if (identificators->contains(lex))
				identificators->get_id(lex, id);
			else
			{
				identificators->put(lex);
				identificators->get_id(lex, id);
			}
		}
		else
		{
			if (constants->contains(lex))
				constants->get_id(lex, id);
			else
			{
				constants->put(lex);
				constants->get_id(lex, id);
			}
		}
	}
};