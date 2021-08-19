#include <string>
#include <iostream>

#include "Tables/ConstTable.h"
#include "Tables/HashTable.h"
#include "Lexer/ID.h"
#include "Lexer/lexeme.h"
#include "Types.h"

using namespace std;

int main()
{
	hash_table table(0, 5);

	lexeme v(VARTYPE::FLOAT, "a");

	table.put(lexeme(VARTYPE::FLOAT, "a"));
	table.put(lexeme(VARTYPE::INT, "b"));
	table.put(lexeme(VARTYPE::CHAR, "c"));
	table.put(lexeme(VARTYPE::FLOAT, "eps"));

	table.print();
	ID id;
	if (table.contains(v))
	{
		table.get_id(v, id);
		lexeme c;
		table.get_by_id(id, c);
	}

	lexeme count;
	table.get_by_id(id, count);
	count.set_value("100");
	count.set_type(VARTYPE::INT);
	table.change(id, count);

	cout << endl;
	table.print();
	const_table const_t(1);

	const_t.put("main");
	const_t.put("int");
	const_t.put("return");
	const_t.put("double");



	cout << endl << endl;
	const_t.print();
	if (const_t.contains("int"))
	{
		ID id;
		const_t.get_id("int", id);
		string word;
		const_t.get_by_id(id, word);
	}
}