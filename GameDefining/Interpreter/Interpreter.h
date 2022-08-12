#pragma once

#include <iostream>
#include <string>
#include <list>

#include "RegularExpression.h"
#include "../File/File.h"

class Interpreter
{
private:
	std::list<RegularExpression*> lexical_analysis_list;

	std::list<Symbol*> LexicalAnalysis(std::string file_path);

public:
	Interpreter();
	~Interpreter();

	void add_RegEx(RegularExpression* regex);

	bool Analysis(std::string file_path);

	void print();
};

