#pragma once

#include <string>
#include <iostream>

using namespace std;

struct ID
{	
	ID(int table_no = -1, int chain_no = -1, int node_no = -1) : table_no(table_no), chain_no(chain_no), node_no(node_no) {}

	bool IsValid()
	{
		return table_no != -1 && chain_no != -1 && node_no != -1;
	}

	int table_no;
	int chain_no;
	int node_no;
};

inline ostream& operator<<(ostream& stream, const ID& id)
{
	stream << id.table_no << ' ' << id.chain_no << ' ' << id.node_no;
	return stream;
}
 
inline istream& operator>>(istream& stream, const ID& id)
{
	stream >> id.table_no >> id.chain_no >> id.node_no;
	return stream;
}

typedef vector<ID>::iterator token_iterator;