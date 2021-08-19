#pragma once

#include <vector>
#include <string>

#include "../Lexer/ID.h"

using namespace std;

class const_table {

public:
	const_table(int table_no): table_no(table_no) {}

	void put(string word)
	{
		if (!contains(word))
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
		if (id.row_no == -1)
			return false; 

		if (id.row_no < size)
		{
			word = table[id.row_no];
			return true;
		}
		else
		{
			return false;
		}
	}

	bool contains(string word)
	{
		for (auto& w : table)
			if (w == word)
				return true;

		return false;
	}

	int get_table_no() { return table_no; }

private:
	vector<string> table;
	int size = 0;
	int table_no;
};