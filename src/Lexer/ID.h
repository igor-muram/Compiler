#pragma once

#include <string>
#include <sstream>

using namespace std;

struct ID
{	
	ID(int table_no = -1, int chain_no = -1, int node_no = -1) : table_no(table_no), chain_no(chain_no), node_no(node_no) {}

	string to_string()
	{
		stringstream idBuilder;
		idBuilder << '(' << table_no << ',' << chain_no << ',' << node_no << ')';
		return idBuilder.str();
	}

	int table_no;
	int chain_no;
	int node_no;
};