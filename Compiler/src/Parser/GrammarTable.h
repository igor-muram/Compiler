#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

struct TableRow
{
	std::vector<std::string> terminals;
	int jump = 0;
	bool accept = false;
	bool stack = false;
	bool ret = false;
	bool error = false;
};

inline std::istream& operator>>(std::istream& stream, TableRow& row)
{
	char c = 0;
	while (c != '|')
	{
		std::string terminal;
		stream.read(&c, 1);
		while (c != ' ' && c != '|' && c != '\n')
		{
			terminal.push_back(c);
			stream.read(&c, 1);
		}

		if (terminal != "")
			row.terminals.push_back(terminal);
	}

	stream >> row.jump >> row.accept >> row.stack >> row.ret >> row.error;
	row.jump--;

	return stream;
}

class GrammarTable
{
public:
	GrammarTable() = default;
	GrammarTable(std::string filename);

	TableRow& operator[](int i)
	{
		return table[i];
	}

private:
	std::vector<TableRow> table;
};