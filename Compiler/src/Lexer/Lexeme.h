#pragma once

#include <string>
#include <sstream>

#include "../Types.h"
#include "ID.h"

using namespace std;

typedef vector<ID>::iterator token_iterator;

class lexeme 
{
public:
	lexeme() : name(""), value("") {}
	lexeme(string name, VARTYPE type = VARTYPE::VOID, string value = "") : type(type), name(name), value(value) {}
	lexeme(const lexeme& var) : type(var.type), name(var.name), value(var.value) {}

	void set_type(VARTYPE type) { this->type = type; }
	void set_name(string name) { this->name = name; }
	void set_value(string value) { this->value = value; }

	VARTYPE get_type() { return type; }
	string get_name() { return name; }
	string get_value() { return value; }

	bool operator==(lexeme& a)
	{
		return name == a.name;
	}

	bool operator!=(lexeme& a)
	{
		return !(name == a.name);
	}

	friend ostream& operator<<(ostream& stream, const lexeme& lexeme);

private:
	VARTYPE type = VARTYPE::VOID;
	string name;
	string value;
};

inline ostream& operator<<(ostream& stream, const lexeme& lexeme)
{
	stream << '(' << lexeme.name << ", ";
	switch (lexeme.type)
	{
	case VARTYPE::VOID:
		stream << "void, ";
		break;
	case VARTYPE::INT:
		stream << "int, ";
		break;
	default:
		break;
	}

	if (lexeme.value == "")
		stream << "empty";
	else
		stream << lexeme.value;

	stream << ")";

	return stream;
}