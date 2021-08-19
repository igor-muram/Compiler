#pragma once

#include "../Tables/TableManager.h"
#include "../Parser/Parser.h"

#include <stack>
#include <map>

class CodeGenerator
{
public:
	CodeGenerator(table_manager* manager): manager(manager) {}
	void Generate(std::vector<std::string>& postfix, std::vector<variable>& variables);

	std::vector<std::string>::iterator BodyBegin() { return code.begin(); }
	std::vector<std::string>::iterator BodyEnd() { return code.end(); }

	std::vector<std::string>::iterator HeadBegin() { return head.begin(); }
	std::vector<std::string>::iterator HeadEnd() { return head.end(); }

private:
	table_manager* manager;
	int var_count = 0;
	int const_count = 0;
	int label_count = 0;

	std::stack<std::string> stack;
	std::map<std::string, std::string> consts;

	std::vector<std::string>::iterator begin, end, pos;

	std::vector<std::string> code, head;

private:
	void AssignCode();
	void PlusCode();
	void MinusCode();
	void MulCode();
	void EqCode();
	void NeCode();
	void LtCode();
	void CJCode();
	void UJCode();
	void RetCode();

	std::string getT(std::string& op1, std::string& op2);
};