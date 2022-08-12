#pragma once
#include <string>
#include <iostream>

#include "Symbol.h"

#define	DEBUG_REGEX false

class RegularExpression
{
	// Class to define subset of characters to describe a column
	// Contains function to determine if given character falls into this set
	class CharAcceptFunction
	{
	private:
		bool (*function)(char);						// Core of the class - Acceptation function
		std::string name = "";						// Description of function

	public:
		CharAcceptFunction(bool (*function)(char), std::string name = "");

		bool Check(char c);							// Use of Accepttion function

		std::string getName();

	};

private:

	bool constant = false;							// Determines if expression is defined by Table or String constant

	// Table expression
	int rows = 0;
	int cols = 0;

	int** state_table = NULL;						// Table of state transitions depending on current state and next character
	bool* end_states = NULL;						// States in which string is accepted
	CharAcceptFunction** accept_functions = NULL;	// Conditions on which char is accepted in column

	int state = 0;									// Current state	(reset)

	// Constant expression
	std::string const_string = "";					// String constant
	int const_length = 0;							// String length

	bool correct = false;							// Informs if given string matches String constant	(reset)

	// Analysis
	int char_num = 0;								// Number of checked chars	(reset)
	int last_fit = 0;								// Length of the longest fitting string	(reset)


	// Finalisation
	Symbol* (*generate_value)(std::string);			// Generate value of found string

public:
	RegularExpression(int rows, int cols);
	RegularExpression(std::string const_string);
	RegularExpression(std::string const_string, Symbol*(*generate_value)(std::string));
	~RegularExpression();

	// Setup functions
	void setTablePosition(int row, int col, int transition);
	void setEndState(int state);
	void setAcceptFunction(int column, bool (*function)(char), std::string name = "");
	void setGenerateFunction(Symbol*(*generate_value)(std::string));

	// Action functions
	bool passCharacter(char c);						// Gets next character from string, checks if if matches the pattern and returns result (true - by default, false - if falls into unknown state); 0 - value of EOF
	int getLastFit();								// Returns length of the longest fitting string
	Symbol* generateValue(std::string found);			// Generate value of found string
	void reset();									// Resets searching

	// Other
	void print();
};

