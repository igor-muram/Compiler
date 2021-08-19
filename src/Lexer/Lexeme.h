#pragma once

#include <string>
#include <sstream>

#include "../Types.h"

using namespace std;

class lexeme 
{
public:
	lexeme() : type(VARTYPE::VOID), name(""), value("") {}
	lexeme(VARTYPE type, string name, string value = "") : type(type), name(name), value(value) {}
	lexeme(const lexeme& var) : type(var.type), name(var.name), value(var.value) {}

	void set_type(VARTYPE type) { this->type = type; }
	void set_name(string name) { this->name = name; }
	void set_value(string value) { this->value = value; }

	VARTYPE get_type() { return type; }
	string get_name() { return name; }
	string get_value() { return value; }

	string to_string()
	{
		stringstream stream;
		stream << '(' << name << ", ";
		switch (type)
		{
		case VARTYPE::VOID:
			stream << "void, ";
			break;
		case VARTYPE::INT:
			stream << "int, ";
			break;
		case VARTYPE::FLOAT:
			stream << "float, ";
			break;
		case VARTYPE::CHAR:
			stream << "char, ";
			break;
		default:
			break;
		}

		if (value == "")
			stream << "empty";
		else
			stream << value;

		stream << ")";

		return stream.str();
	}

	bool operator==(lexeme& a)
	{
		return type == a.type && name == a.name && value == a.value;
	}

	bool operator!=(lexeme& a)
	{
		return !(type == a.type && name == a.name && value == a.value);
	}

private:
	VARTYPE type;
	string name;
	string value;
};