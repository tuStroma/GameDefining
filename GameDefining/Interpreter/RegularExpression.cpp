#include "RegularExpression.h"


// Functions for function class

RegularExpression::CharAcceptFunction::CharAcceptFunction(bool (*function)(char), std::string name)
{
	this->function = function;
	this->name = name;
}

bool RegularExpression::CharAcceptFunction::Check(char c)
{
	return function(c);
}

std::string RegularExpression::CharAcceptFunction::getName()
{
	return name;
}


// Functions for RegEx

RegularExpression::RegularExpression(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;

	// Alloc state table
	state_table = (int**)malloc(rows * sizeof(int*));
	if (state_table == NULL) { std::cout << "Memory allocation error\n"; exit(0); }

	for (int i = 0; i < rows; i++)
	{
		state_table[i] = (int*)malloc(cols * sizeof(int));
		if (state_table[i] == NULL) { std::cout << "Memory allocation error\n"; exit(0); }
	}

	// Fill state_table
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			state_table[i][j] = -1;


	// Alloc End States array
	end_states = (bool*)malloc(rows * sizeof(bool));
	if (end_states == NULL) { std::cout << "Memory allocation error\n"; exit(0); }
	for (int i = 0; i < rows; i++)
		end_states[i] = false;


	// Alloc Accept Function array
	accept_functions = (CharAcceptFunction**)malloc(cols * sizeof(CharAcceptFunction*));
	if (accept_functions == NULL) { std::cout << "Memory allocation error\n"; exit(0); }
	for (int i = 0; i < cols; i++)
		accept_functions[i] = new CharAcceptFunction([](char c) { return false; });
}

RegularExpression::RegularExpression(std::string const_string)
{
	constant = true;
	this->const_string = const_string;
	const_length = (int)const_string.length();
}

RegularExpression::RegularExpression(std::string const_string, void(*generate_value)(std::string))
{
	constant = true;
	this->const_string = const_string;
	const_length = (int)const_string.length();

	setGenerateFunction(generate_value);
}

RegularExpression::~RegularExpression()
{
	if (!constant)
		for (int i = 0; i < rows; i++)
			free(state_table[i]);

	free(state_table);
	free(end_states);

	for (int i = 0; i < cols; i++)
		accept_functions[i]->~CharAcceptFunction();
	free(accept_functions);


	std::cout << "Deleting RegEx object\n";
}


// Setup functions
void RegularExpression::setTablePosition(int row, int col, int transition)
{
	// Check row range
	if (row < 0 || row >= this->rows)
	{
		std::cout << "Row number ( " << row << " ) out of range. Should be between 0 and " << this->rows - 1 << '\n';
		return;
	}

	// Check col range
	if (col < 0 || col >= this->cols)
	{
		std::cout << "Column number ( " << col << " ) out of range. Should be between 0 and " << this->cols - 1 << '\n';
		return;
	}

	// Check state transition range
	if (transition < 0 || transition >= this->rows)
	{
		std::cout << "State transition ( " << transition << " ) out of range. Should be between 0 and " << this->rows - 1 << '\n';
		return;
	}

	state_table[row][col] = transition;
}

void RegularExpression::setEndState(int state)
{
	// Check state range
	if (state < 0 || state >= this->rows)
	{
		std::cout << "End state number ( " << state << " ) out of range. Should be between 0 and " << this->rows - 1 << '\n';
		return;
	}

	end_states[state] = true;
}

void RegularExpression::setAcceptFunction(int column, bool (*function)(char), std::string name)
{
	// Check column range
	if (column < 0 || column >= this->cols)
	{
		std::cout << "Column number(AcceptFunction) ( " << column << " ) out of range. Should be between 0 and " << this->cols - 1 << '\n';
		return;
	}

	accept_functions[column]->~CharAcceptFunction();
	accept_functions[column] = new CharAcceptFunction(function, name);
}

void RegularExpression::setGenerateFunction(void (*generate_value)(std::string))
{
	this->generate_value = generate_value;
}


// Action functions
bool RegularExpression::passCharacter(char c)
{
	if (c == 0)		// Check if end of file
		return false;

	if (constant)	// If pattern is constant string
	{
		if (const_string[char_num] == c)
		{
			char_num++;

			if (char_num == const_length)
			{
				correct = true;
				last_fit = const_length;
			}

			return true;
		}

		return false;
	}

	else			// If pattern is table expression
	{
		if (state < 0)					// Check if current state is legal
			return false;

		for (int j = 0; j < cols; j++)	// For every column check if char fits there
		{
			if (accept_functions[j]->Check(c))	// If char fits in column
			{
				state = state_table[state][j];		// Set new state
				char_num++;							// Increments number of passed characters

				if (state < 0) 						// Check if further recognition is still possible
					return false;

				if (end_states[state])				// Check if current state is an end state
					last_fit = char_num;

				if (DEBUG_REGEX)
					std::cout << "Character: \t" << c << " \tNew state: \t" << state << '\n';

				return true;
			}
		}

		// If character doesn't fit in any column
		state = -1;
		return false;
	}
}
int RegularExpression::getLastFit()
{
	return last_fit;
}
void RegularExpression::generateValue(std::string found) 
{
	generate_value(found);
}
void RegularExpression::reset()
{
	// Table expression reset
	state = 0;

	// Constant expression reset
	correct = false;

	// Analysis reset
	char_num = 0;
	last_fit = 0;
}


void RegularExpression::print()
{
	std::cout << "Rows: " << rows << " \tColumns: " << cols << "\n\n";

	std::cout << "\t";
	for (int j = 0; j < cols; j++)
		std::cout << "| " << accept_functions[j]->getName() << '\t';
	std::cout << '\n';

	for (int i = 0; i < rows; i++)
	{
		if (end_states[i]) std::cout << '*';
		else std::cout << ' ';
		std::cout << ' ' << i << ":\t"; 

		for (int j = 0; j < cols; j++)
		{
			std::cout << "| ";
			if(state_table[i][j] >= 0)
				std::cout << state_table[i][j];
			std::cout << '\t';
		}
		std::cout << '\n';
	}
}