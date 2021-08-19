#pragma once

#include <string>
#include <iostream>

#include "../Lexer/ID.h"
#include "../Lexer/Lexeme.h"
#include "../Types.h"

using namespace std;

class hash_node
{
public:
	hash_node(lexeme value)
	{
		this->value = value;
		next = nullptr;
	}

	lexeme get_value() { return value; }
	hash_node* get_next() { return next; }

	void set_next(hash_node* node) { next = node; }
	void set_value(lexeme value) { this->value = value;}

private:
	hash_node* next;
	lexeme value;
};

class hash_table
{
public:
	hash_table(int table_no, int size)
	{
		this->table_no = table_no;
		this->size = size;
		table = new hash_node*[size];

		for (int i = 0; i < size; i++)
			table[i] = nullptr;
	}

	~hash_table()
	{
		for (int i = 0; i < size; i++)
		{
			hash_node* node = table[i];

			while(node != nullptr)
			{ 
				hash_node* prev = node;
				node = node->get_next();
				delete prev;
			}

			table[i] = nullptr;
		}

		delete[] table;
	}

	bool contains(lexeme value)
	{
		int h = hash(value.get_name());
		if (table[h] == nullptr)
			return false;

		hash_node* node = table[h];
		while (node->get_next() != nullptr && node->get_value() != value)
			node = node->get_next();

		if (node->get_value() != value)
			return false;

		return true;
	}

	void put(lexeme value)
	{
		int h = hash(value.get_name());
		hash_node* new_node = new hash_node(value);
		
		if (table[h] != nullptr)
		{
			hash_node* node = table[h];
			while (node->get_next() != nullptr)
				node = node->get_next();

			node->set_next(new_node);	
		}
		else
		{
			table[h] = new_node;
		}
	}
	
	bool change(ID id, lexeme value)
	{
		if (id.node_no == -1 && id.chain_no == -1)
			return false;

		hash_node* node = table[id.chain_no];

		for (int i = 0; i < id.node_no; i++)
			node = node->get_next();

		node->set_value(value);
		return true;
	}

	bool get_id(lexeme value, ID& id)
	{
		id.table_no = table_no;
		id.chain_no = hash(value.get_name());
		hash_node* node = table[id.chain_no];

		int pos = 0;
		while (node->get_next() != nullptr && node->get_value() != value)
		{
			node = node->get_next();
			pos++;
		}

		if (node->get_value() == value)
		{
			id.node_no = pos;
			return true;
		}
		else
		{
			id.node_no = id.chain_no = -1;
			return false;
		}
	}

	bool get_by_id(ID id, lexeme& value)
	{
		if (id.node_no == -1 && id.chain_no == -1)
			return false;

		hash_node* node = table[id.chain_no];

		for (int i = 0; i < id.node_no && node != nullptr; i++)
			node = node->get_next();

		if (node != nullptr)
		{
			value = node->get_value();
			return true;
		}
		else return false;
	}

	int get_table_no() { return table_no; }

	void print() 
	{
		for (int i = 0; i < size; i++)
		{
			cout << i << " -> ";
			hash_node* node = table[i];

			while (node != nullptr)
			{
				cout << node->get_value().to_string() << " -> ";
				node = node->get_next();
			}

			cout << "null" << endl;
		}
	}

private:
	hash_node** table;
	int size;
	int table_no;

	int hash(string& value)
	{
		int hash = 7;
		for (int i = 0; i < value.size(); i++)
		{
			hash = hash * 31 + abs(static_cast<int>(value[i]));
		}

		return hash % size;
	}
};
