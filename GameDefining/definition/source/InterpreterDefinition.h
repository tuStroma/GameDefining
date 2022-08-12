#pragma once

#include "symbolsEnum.h"

#include "../../Interpreter/Interpreter.h"
#include "../../Interpreter/Symbol.h"

Interpreter* createInterpreter()
{
	Interpreter* inter = new Interpreter();

	RegularExpression* multiline_comment = new RegularExpression(5, 3);
	int* (*f2)(std::string) = [](std::string) { return (int*)NULL; };
	{
		multiline_comment->setTablePosition(0, 0, 1);
		multiline_comment->setTablePosition(1, 1, 2);
		multiline_comment->setTablePosition(2, 1, 3);
		multiline_comment->setTablePosition(2, 2, 2);
		multiline_comment->setTablePosition(3, 0, 4);
		multiline_comment->setTablePosition(3, 1, 3);
		multiline_comment->setTablePosition(3, 2, 2);

		multiline_comment->setEndState(4);

		multiline_comment->setAcceptFunction(0,
			[](char c) { return (c == '/'); },
			"/");

		multiline_comment->setAcceptFunction(1,
			[](char c) { return (c == '*'); },
			"*");

		multiline_comment->setAcceptFunction(2,
			[](char c) { return (c != '*' && c != '/'); },
			"['/*]");


		multiline_comment->setGenerateFunction([](std::string found) {
			std::cout << "Multiline comment:\t" << found << '\n';
			return (Symbol*)NULL;
			});
	}

	RegularExpression* identifier = new RegularExpression(2, 2);
	{
		identifier->setTablePosition(0, 0, 1);
		identifier->setTablePosition(1, 0, 1);
		identifier->setTablePosition(1, 1, 1);

		identifier->setEndState(1);

		identifier->setAcceptFunction(0,
			[](char c) { return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')); },
			"[a-zA-Z]");

		identifier->setAcceptFunction(1,
			[](char c) { return (('0' <= c && c <= '9') || c == '_'); },
			"[0-9_]");



		identifier->setGenerateFunction([](std::string found) {
			std::cout << "Identifier:\t" << found << '\n';
			return new Symbol(SymbolType::IDENTIFIER);
			});
	}

	RegularExpression* white_spaces = new RegularExpression(1, 1);
	{
		white_spaces->setTablePosition(0, 0, 0);

		white_spaces->setEndState(0);

		white_spaces->setAcceptFunction(0,
			[](char c) { return (c == ' ' || c == '\t' || c == '\n'); },
			"[ \t\n]");


		white_spaces->setGenerateFunction([](std::string found) {return (Symbol*)NULL; });
	}

	RegularExpression* kw_int = new RegularExpression("int", [](std::string found) { std::cout << "KW: \tint\n"; return new Symbol(SymbolType::KW_INT); });
	RegularExpression* kw_float = new RegularExpression("float", [](std::string found) { std::cout << "KW: \tfloat\n"; return new Symbol(SymbolType::KW_FLOAT); });

	RegularExpression* operator_assign = new RegularExpression("=", [](std::string found) { std::cout << "Operator: \t=\n"; return new Symbol(SymbolType::OPERATOR_ASSIGN); });
	RegularExpression* operator_add = new RegularExpression("+", [](std::string found) { std::cout << "Operator: \t+\n"; return new Symbol(SymbolType::OPERATOR_ADD); });
	RegularExpression* operator_sub = new RegularExpression("-", [](std::string found) { std::cout << "Operator: \t-\n"; return new Symbol(SymbolType::OPERATOR_SUB); });

	inter->add_RegEx(kw_int);
	inter->add_RegEx(kw_float);

	inter->add_RegEx(operator_assign);
	inter->add_RegEx(operator_add);
	inter->add_RegEx(operator_sub);

	inter->add_RegEx(multiline_comment);
	inter->add_RegEx(identifier);
	inter->add_RegEx(white_spaces);
	
	return inter;
}
