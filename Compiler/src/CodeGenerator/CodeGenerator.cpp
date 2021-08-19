#include "CodeGenerator.h"

void CodeGenerator::Generate(std::vector<std::string>& postfix, std::vector<variable>& variables)
{
	begin = postfix.begin();
	end = postfix.end();

	pos = begin;
	while (pos != end)
	{
		if (manager->is_const(*pos))
		{
			if (consts.find(*pos) == consts.end())
			{
				consts[*pos] = "C" + std::to_string(const_count);
				const_count++;
			}

			stack.push(consts[*pos]);
		}
		else if (manager->is_identificator(*pos))
		{
			stack.push(*pos);
		}
		else if ((*pos)[0] == 'L')
		{
			if ((*pos).back() != ':')
				stack.push(*pos);
			else
				code.push_back(*pos);
		}


		if (*pos == "=")
			AssignCode();
		else if (*pos == "+")
			PlusCode();
		else if (*pos == "-")
			MinusCode();
		else if (*pos == "*")
			MulCode();
		else if (*pos == "==")
			EqCode();
		else if (*pos == "!=")
			NeCode();
		else if (*pos == "<")
			LtCode();
		else if (*pos == "CJ")
			CJCode();
		else if (*pos == "UJ")
			UJCode();
		else if (*pos == "return")
			RetCode();

		pos++;
	}

	for (auto pair: consts)
	{
		head.push_back(pair.second + " DD " + pair.first);
	}

	for (int i = 0; i < var_count; i++)
	{
		head.push_back("T" + std::to_string(i) + " DD 0");
	}

	for (auto v : variables)
	{
		head.push_back(v.id + " DD 0");
	}
}

void CodeGenerator::AssignCode()
{
	std::string op2 = stack.top();
	stack.pop();
	std::string op1 = stack.top();
	stack.pop();

	code.push_back("mov eax, " + op2);
	code.push_back("mov " + op1 + ", eax");
}

void CodeGenerator::PlusCode()
{
	std::string op2 = stack.top();
	stack.pop();
	std::string op1 = stack.top();
	stack.pop();

	std::string t = getT(op1, op2);


	code.push_back("mov eax, " + op1);
	code.push_back("add eax, " + op2);
	code.push_back("mov " + t + ", eax");

	stack.push(t);
}

void CodeGenerator::MinusCode()
{
	std::string op2 = stack.top();
	stack.pop();
	std::string op1 = stack.top();
	stack.pop();

	std::string t = getT(op1, op2);
	
	code.push_back("mov eax, " + op1);
	code.push_back("sub eax, " + op2);
	code.push_back("mov " + t + ", eax");

	stack.push(t);
}

void CodeGenerator::MulCode()
{
	std::string op2 = stack.top();
	stack.pop();
	std::string op1 = stack.top();
	stack.pop();

	std::string t = getT(op1, op2);
	
	code.push_back("mov eax, " + op1);
	code.push_back("imul eax, " + op2);
	code.push_back("mov " + t + ", eax");

	stack.push(t);
}

void CodeGenerator::EqCode()
{
	std::string op2 = stack.top();
	stack.pop();
	std::string op1 = stack.top();
	stack.pop();

	std::string t = getT(op1, op2);
	std::string firstLL = "LL" + std::to_string(label_count++);
	std::string secondLL = "LL" + std::to_string(label_count++);

	code.push_back("mov eax, " + op1);
	code.push_back("cmp " + op1 + ", eax");
	code.push_back("jne " + firstLL);
	code.push_back("mov " + t + ", 1");
	code.push_back("jmp " + secondLL);
	code.push_back(firstLL + ":");
	code.push_back("mov " + t + ", 0");
	code.push_back(secondLL + ":");

	stack.push(t);
}

void CodeGenerator::NeCode()
{
	std::string op2 = stack.top();
	stack.pop();
	std::string op1 = stack.top();
	stack.pop();

	std::string t = getT(op1, op2);

	std::string firstLL = "LL" + std::to_string(label_count++);
	std::string secondLL = "LL" + std::to_string(label_count++);

	code.push_back("mov eax, " + op1);
	code.push_back("cmp " + op1 + ", eax");
	code.push_back("je " + firstLL);
	code.push_back("mov " + t + ", 1");
	code.push_back("jmp " + secondLL);
	code.push_back(firstLL + ":");
	code.push_back("mov " + t + ", 0");
	code.push_back(secondLL + ":");

	stack.push(t);
}

void CodeGenerator::LtCode()
{
	std::string op2 = stack.top();
	stack.pop();
	std::string op1 = stack.top();
	stack.pop();

	std::string t = getT(op1, op2);

	std::string firstLL = "LL" + std::to_string(label_count++);
	std::string secondLL = "LL" + std::to_string(label_count++);

	code.push_back("mov eax, " + op2);
	code.push_back("cmp " + op1 + ", eax");
	code.push_back("ja " + firstLL);
	code.push_back("mov " + t + ", 1");
	code.push_back("jmp " + secondLL);
	code.push_back(firstLL + ":");
	code.push_back("mov " + t + ", 0");
	code.push_back(secondLL + ":");

	stack.push(t);
}

void CodeGenerator::CJCode()
{
	std::string label = stack.top();
	stack.pop();
	std::string op = stack.top();
	stack.pop();

	code.push_back("cmp " + op + ", 0");
	code.push_back("je " + label);
}

void CodeGenerator::UJCode()
{
	std::string label = stack.top();
	stack.pop();

	code.push_back("jmp " + label);
}

void CodeGenerator::RetCode()
{
	std::string op = stack.top();
	stack.pop();

	code.push_back("mov eax, " + op);
}

std::string CodeGenerator::getT(std::string& op1, std::string& op2)
{
	std::string t = "";
	if (op1[0] == 'T')
		t = op1;
	else if (op2[0] == 'T')
		t = op2;

	if (t == "")
		t = "T" + std::to_string(var_count++);

	return t;
}
