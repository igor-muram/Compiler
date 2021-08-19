#include <string>
#include <iostream>

#include "Tables/ConstTable.h"
#include "Tables/HashTable.h"
#include "Lexer/ID.h"

using namespace std;

int main()
{

	hash_table table(0, 5);

	variable v(VARTYPE::FLOAT, "a");

	table.put(variable(VARTYPE::FLOAT, "a"));
	table.put(variable(VARTYPE::INT, "b"));
	table.put(variable(VARTYPE::CHAR, "c"));
	table.put(variable(VARTYPE::FLOAT, "eps"));


	if (table.contains(v))
	{
		ID id;
		table.get_id(v, id);
		variable c;
		table.get_by_id(id, c);
	}

	if (table.contains(variable()))
	{
		cout << "ERROR" << endl;
	}

	const_table const_t(1);

	const_t.put("main");
	const_t.put("int");
	const_t.put("return");
	const_t.put("double");

	if (const_t.contains("int"))
	{
		ID id;
		const_t.get_id("int", id);
		string word;
		const_t.get_by_id(id, word);
	}
}