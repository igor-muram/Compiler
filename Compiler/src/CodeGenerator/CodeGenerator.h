#pragma once

#include "../Tables/TableManager.h"
#include "../Parser/Parser.h"

class CodeGenerator
{
public:
	CodeGenerator(table_manager* manager): manager(manager) {}
	void Generate(std::vector<std::string> postfix);

private:
	table_manager* manager;
	int var_count = 0;
	int const_count = 0;
};