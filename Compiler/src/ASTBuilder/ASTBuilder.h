#pragma once
#include <string>
#include <vector>
#include <stack>
#include <queue>

#include "../Lexer/ID.h"
#include "../Tables/TableManager.h"

enum class OPs {
	ADD,
	MUL,
	SUB,
	EQ,
	NE,
	LT,
	ASSIGN
};

class ASTNode
{
public:
	ASTNode(std::string type, std::string value = "") : type(type), value(value) { }

	void AddChild(ASTNode* node)
	{
		Childs.push_back(node);
	}

	void SetValue(std::string value) { this->value = value; }

	std::vector<ASTNode*> Childs;
	std::string type = "";
	std::string value = "";
};

class ASTBuilder
{
public:
	ASTBuilder(table_manager* manager) : manager(manager) {	}

	void Build(token_iterator begin, token_iterator end)
	{
		this->begin = begin;
		this->end = end;

		token = begin;
		Match("int");
		Match("main");
		Match("(");
		Match(")");
		Match("{");

		tree = new ASTNode("program");
		tree->AddChild(Body());

		BuildPostfix(tree);
	}

	std::vector<std::string> postfix;
private:
	table_manager* manager;
	ASTNode* tree;
	token_iterator token, begin, end;

	std::string raw = "";
	std::string matching = "";

	

	int label = 0;

private:
	bool IsMatch(std::string type)
	{
		matching = manager->get_by_id(*token).get_name();
		return matching == type;
	}

	std::string Match(std::string type)
	{
		if (IsMatch(type))
		{
			token++;

			raw = type;
			return type;
		}
		else
		{
			return "";
		}
	}

	OPs MatchOp()
	{
		OPs op;
		std::string raw_op = manager->get_by_id(*token).get_value();

		if (raw_op == "*")
			op = OPs::MUL;

		if (raw_op == "+")
			op = OPs::ADD;

		if (raw_op == "-")
			op = OPs::SUB;

		if (raw_op == "==")
			op = OPs::EQ;

		if (raw_op == "!=")
			op = OPs::NE;

		if (raw_op == "<")
			op = OPs::LT;
		
		if (raw_op == "=")
			op = OPs::ASSIGN;

		token++;
		return op;
	}

	ASTNode* Body()
	{
		ASTNode* body = new ASTNode("body");
		while (token != end)
		{
			if (IsMatch("int"))
			{
				body->AddChild(Decl());
			}
			if (IsMatch("var") || IsMatch("const"))
			{
				body->AddChild(Expr(true));
			}
			else if (IsMatch("if"))
			{
				body->AddChild(If());
			}
			else if (IsMatch("return"))
			{
				body->AddChild(Return());
			}
			else
			{
				token++;
			}
		}

		return body;
	}

	ASTNode* Decl()
	{
		ASTNode* node = new ASTNode("decl");
		Match("int");
		node->AddChild(Expr(true));

		return node;
	}

	ASTNode* If()
	{
		std::string L1 = "L" + std::to_string(label);
		std::string L2 = "L" + std::to_string(label + 1);
		label += 2;
		Match("if");

		ASTNode* cond;
		ASTNode* CJ = new ASTNode("CJ");
		ASTNode* UJ = new ASTNode("UJ", L1 + ":");
		CJ->AddChild(Expr(true));
		CJ->AddChild(new ASTNode("label", L1));

		UJ->AddChild(new ASTNode("label", L2));

		Match("{");
		ASTNode* B = IfBody("ifbody");
		Match("}");

		ASTNode* C;
		if (IsMatch("else"))
		{
			cond = new ASTNode("if", L2 + ":");
			Match("else");
			Match("{");
			C = IfBody("elsebody");
			Match("}");

			cond->AddChild(CJ);
			cond->AddChild(B);
			cond->AddChild(UJ);
			cond->AddChild(C);
		}
		else
		{
			cond = new ASTNode("if", L1 + ":");
			cond->AddChild(CJ);
			cond->AddChild(B);
		}
		
		return cond;
	}

	ASTNode* IfBody(std::string type)
	{
		ASTNode* body = new ASTNode(type);
		while (!IsMatch("}"))
		{
			if (IsMatch("int"))
			{
				body->AddChild(Decl());
			}
			else if (IsMatch("var") || IsMatch("const"))
			{
				body->AddChild(Expr(true));
			}
			else if (IsMatch("if"))
			{
				body->AddChild(If());
			}
			else if (IsMatch("return"))
			{
				body->AddChild(Return());
			}
			else
			{
				token++;
			}
		}

		return body;
	}

	ASTNode* Return()
	{
		ASTNode* ret = new ASTNode(Match("return"));
		ret->AddChild(Expr(false));

		return ret;
	}

	ASTNode* Expr(bool with_semi)
	{
		std::stack<std::string> stack;
		std::stack<ASTNode*> node_stack;

		while (!IsMatch(";") && !IsMatch("{"))
		{
			if (IsMatch("var"))
			{
				Match("var");
				node_stack.push(new ASTNode("var", manager->get_by_id(*(token - 1)).get_value()));
			}

			if (IsMatch("const"))
			{
				Match("const");
				node_stack.push(new ASTNode("const", manager->get_by_id(*(token - 1)).get_value()));
			}

			if (IsMatch("operator"))
			{
				OPs op = MatchOp();

				switch (op)
				{
				case OPs::MUL:
				{
					if (!stack.empty())
					{
						std::string top = stack.top();
						while (top == "*")
						{
							FreeStack(stack, node_stack);
							top = stack.top();
						}
					}
					stack.push("*");
				}
				break;
				case OPs::ADD:
				{
					if (!stack.empty())
					{
						std::string top = stack.top();
						while (top == "*" || top == "+" || top == "-")
						{
							FreeStack(stack, node_stack);
							top = stack.top();
						}
					}
					stack.push("+");
				}
				break;
				case OPs::SUB:
				{
					if (!stack.empty())
					{
						std::string top = stack.top();
						while (top == "*" || top == "+" || top == "-")
						{
							FreeStack(stack, node_stack);
							top = stack.top();
						}
					}
					stack.push("-");
				}
				break;
				case OPs::EQ:
				{
					if (!stack.empty())
					{
						std::string top = stack.top();
						while (top == "*" || top == "+" || top == "-" || top == "==" || top == "!=" || top == "!=")
						{
							FreeStack(stack, node_stack);
							top = stack.top();
						}
					}
					stack.push("==");
				}
				break;
				case OPs::NE:
				{
					if (!stack.empty())
					{
						std::string top = stack.top();
						while (top == "*" || top == "+" || top == "-" || top == "==" || top == "!=" || top == "!=")
						{
							FreeStack(stack, node_stack);
							top = stack.top();
						}
					}
					stack.push("!=");
				}
				break;
				case OPs::LT:
				{
					if (!stack.empty())
					{
						std::string top = stack.top();
						while (top == "*" || top == "+" || top == "-" || top == "==" || top == "!=" || top == "!=")
						{
							FreeStack(stack, node_stack);
							top = stack.top();
						}
					}
					stack.push("<");
				}
				break;
				case OPs::ASSIGN:
				{
					if (!stack.empty())
					{
						std::string top = stack.top();
						while (top == "*" || top == "+" || top == "-" || top == "==" || top == "!=" || top == "!=" || top == "=")
						{
							FreeStack(stack, node_stack);
							top = stack.top();
						}
					}
					stack.push("=");
				}
				default:
					break;
				}
			}

			if (IsMatch("("))
			{
				stack.push(Match("("));
			}

			if (IsMatch(")"))
			{
				std::string top = stack.top();
				while (top != "(")
				{
					FreeStack(stack, node_stack);
					top = stack.top();
				}
				stack.pop();
				
				Match(")");
			}
		}

		while (!stack.empty())
		{
			FreeStack(stack, node_stack);
		}

		ASTNode* expr = new ASTNode("expr");
		expr->AddChild(node_stack.top());

		if (IsMatch(";") && with_semi)
		{
			expr->AddChild(new ASTNode(";"));
		}
		return expr;
	}

	void FreeStack(std::stack<std::string>& stack, std::stack<ASTNode*>& node_stack)
	{
		std::string op = stack.top();
		stack.pop();
		ASTNode* op_node = new ASTNode(op);
		ASTNode* op1 = node_stack.top();
		node_stack.pop();
		ASTNode* op2 = node_stack.top();
		node_stack.pop();

		op_node->AddChild(op2);
		op_node->AddChild(op1);
		node_stack.push(op_node);
	}

	void BuildPostfix(ASTNode* root)
	{
		for (auto child : root->Childs)
		{
			BuildPostfix(child);
		}

		if (root->type != "program" && root->type != "body" && root->type != "decl" && root->type != "expr" && root->type != "ifbody")
		{
			if (root->type == "if" || root->type == "var" || root->type == "const" || root->type == "label" || root->type == "elsebody")
			{
				if (root->value != "")
					postfix.push_back(root->value);
			}
			else
			{
				postfix.push_back(root->type);

				if (root->value != "")
					postfix.push_back(root->value);
			}
		}
	}
};