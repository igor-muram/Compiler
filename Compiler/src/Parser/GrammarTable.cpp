#include "GrammarTable.h"

GrammarTable::GrammarTable(std::string filename)
{
	std::ifstream in(filename);

	if (in.is_open())
	{

		int count;
		in >> count;
		for (int i = 0; i < count; i++)
		{
			TableRow row;
			in >> row;
			table.push_back(row);
		}
	}
	else
	{
		std::cout << "Can't open grammar table file" << std::endl;
	}
}