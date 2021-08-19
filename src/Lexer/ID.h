#pragma once

#include <string>
#include <sstream>

using namespace std;

struct ID
{	
	ID(int table_no = -1, int row_no = -1, int node_no = -1) : table_no(table_no), row_no(row_no), node_no(node_no) {}

	string to_string()
	{
		stringstream idBuilder;
		idBuilder << '(' << table_no << ',' << row_no << ',' << node_no << ')';
		return idBuilder.str();
	}

	int table_no;
	int row_no;
	int node_no;
};