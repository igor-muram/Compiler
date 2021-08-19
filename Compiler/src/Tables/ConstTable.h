#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "../Lexer/ID.h"

using namespace std;


// [TODO] : Const table with lexeme instead of string
class const_table {

public:
	const_table(int table_no): table_no(table_no) {}

	void put(string word)
	{
		table.push_back(word);
		size++;
	}

	bool get_id(string word, ID& id)
	{
		bool isFound = false;
		int pos = 0;
		for (int i = 0; i < size && !isFound; i++)
		{
			if (table[i] == word)
			{
				pos = i;
				isFound = true;
			}
		}

		if (isFound)
		{
			id = ID(table_no, pos, 0);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool get_by_id(ID id, string& word)
	{
		if (id.chain_no == -1)
			return false; 

		if (id.chain_no < size)
		{
			word = table[id.chain_no];
			return true;
		}
		else return false;
	}

	bool contains(string word)
	{
		for (auto& w : table)
			if (w == word)
				return true;

		return false;
	}

	int get_table_no() { return table_no; }

	void print() 
	{
		for (auto element : table)
			cout << element << endl;
	}

private:
	vector<string> table;
	int size = 0;
	int table_no;
};