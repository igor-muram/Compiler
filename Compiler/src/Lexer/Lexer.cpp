#include "Lexer.h"

lexer::lexer()
{
	manager = table_manager();
}

void lexer::scan(string& source)
{
	pos = source.begin();
	end = source.end();

	while (pos != end)
	{
		while (*pos == ' ' || *pos == '\t' || *pos == '\n')
		{
			if (*pos == '\n')
				line++;

			pos++;
		}

		if (isalpha(*pos) || *pos == '_')
			word();
		else if (isdigit(*pos))
			number();
		else if (manager.is_operator(*pos))
			op();
		else if (manager.is_delimiter(*pos))
			delimiter();
		else if (*pos == '/')
			comment();
	}
}

void lexer::word()
{
	string word;
	while (isalnum(*pos) || *pos == '_')
	{
		word.push_back(*pos);
		pos++;
	}

	if (!manager.is_space(*pos) && !manager.is_delimiter(*pos) && !manager.is_operator(*pos) && *pos != '/')
	{
		error();
		pos++;
	}

	tokens.push_back(manager.get_id(word));
}

void lexer::number()
{
	string number;
	while (isdigit(*pos))
	{
		number.push_back(*pos);
		pos++;
	}

	if (!manager.is_space(*pos) && !manager.is_delimiter(*pos) && !manager.is_operator(*pos) && *pos != '/')
	{
		error();
		pos++;
	}

	tokens.push_back(manager.get_id(number));
}

void lexer::op()
{
	if (*pos == '=')
	{
		pos++;
		switch (*pos)
		{
		case '=':
			tokens.push_back(manager.get_id("=="));
			pos++;
			break;
		case '(':
		case '\t':
		case ' ':
		case '/':
			tokens.push_back(manager.get_id("="));
			break;
		default:
			if (!isalnum(*pos))
				error();
			break;
		}
	}
	else if (*pos == '!')
	{
		pos++;
		if (*pos == '=')
		{
			tokens.push_back(manager.get_id("!="));
			pos++;
		}
		else
			error();
	}
	else
	{
		string op;
		op.push_back(*pos);
		tokens.push_back(manager.get_id(op));
		pos++;
		if (pos != end && !manager.is_space(*pos) && !isalnum(*pos) && *pos != '(')
		{
			error();
			pos++;
		}
	}
}

void lexer::delimiter()
{
	string word;
	word.push_back(*pos);
	tokens.push_back(manager.get_id(word));

	switch (*pos)
	{
	case ';':
	case '{':
	case '}':
		pos++;
		if (pos != end && !manager.is_space(*pos) && !isalpha(*pos) && *pos != '}' && *pos != '/')
		{
			error();
			pos++;
		}
		break;
	case ',':
		pos++;
		if (!manager.is_space(*pos) && !isalpha(*pos) && *pos != '/')
		{
			error();
			pos++;
		}
		break;
	case '(':
		pos++;
		if (!manager.is_space(*pos) && !isalnum(*pos) && *pos != '(' && *pos != ')' && *pos != '/')
		{
			error();
			pos++;
		}
		break;
	case ')':
		pos++;
		if (!manager.is_space(*pos) && *pos != ';' && *pos != ',' && *pos != ')' && *pos != '{' && *pos != '/')
		{
			error();
			pos++;
		}
		break;
	}
}

void lexer::comment()
{
	pos++;
	if (pos != end && *pos != '*' && *pos != '/')
	{
		error();
		pos++;
	}

	else if (*pos == '/')
	{
		while (*pos != '\n')
			pos++;
	}
	else if (*pos == '*')
	{
		pos++;
		while (pos + 1 != end  && *pos != '*' && *(pos + 1) != '/')
			pos++;

		if (pos + 1 != end)
			pos += 2;
		else
			pos++;
	}
}

void lexer::error()
{
	stringstream error_buffer;
	error_buffer << "Error on line: " << line << ". " << "'" << *pos << "'" << " is not allowed!";
	errors.push_back(error_buffer.str());
}