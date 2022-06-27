#include "Interpreter.h"

Interpreter::Interpreter()
{
}

Interpreter::~Interpreter()
{
	for (RegularExpression* regex : lexical_analysis_list)
		regex->~RegularExpression();
}


// Object creation
void Interpreter::add_RegEx(RegularExpression* regex)
{
	lexical_analysis_list.push_back(regex);
}


// Analysis
bool Interpreter::Analysis(std::string file_path)
{
	LexicalAnalysis(file_path);
	return true;
}

void Interpreter::LexicalAnalysis(std::string file_path)
{
	std::list<RegularExpression*> regex_list;

	// Setup file
	File* file = new File(file_path);
	int file_counter = 0;

	// Main loop
	bool end_of_file = false;
	while (!end_of_file)
	{
		// Prepare next step of analysis
		std::string word = "";
		regex_list = lexical_analysis_list;
		RegularExpression* fit = NULL;
		int fit_length = 0;


		while (regex_list.size() > 0)
		{
			int previous_cursor = file->getCursorPosition();

			// Get next character
			char next_char = file->next();
			if (next_char == 0 && word == "")	end_of_file = true;	// Check end of file
			word += next_char;


			//std::cout << "\t\t\t\tNext char: " << next_char << '\n';
			// Pass new character to each regex and check the results
			for (std::list<RegularExpression*>::iterator iterator = regex_list.begin(); iterator != regex_list.end();) {
				RegularExpression* regex = *iterator;
				bool result = regex->passCharacter(next_char);		// Pass next character to each regex

				// Chech result
				if (result == false)
				{
					//std::cout << "Erasing\n";
					if (regex->getLastFit() > fit_length)			// Save if found longer fit
					{
						fit = regex;
						fit_length = regex->getLastFit();

						// Set new cursor position
						file_counter = previous_cursor;				
						file->setCursorPosition(file_counter);
						//std::cout << "New fit\n";
					}

					iterator = regex_list.erase(iterator);			// Erase from list
				}
				else
					iterator++;

			}
		}


		// Check found word

		// If word was found
		if (fit)	
		{
			fit->generateValue(word.substr(0, fit_length));		// Execute action
		}
		// If no pattern matches
		else
		{
			// Get first of characters and increment coursor
			char c = file->seek(file_counter);
			file_counter = file->getCursorPosition();

			if (c == 0) end_of_file = true;						// Check end of file
			else	
				std::cout << "No pattern matching character: " << c << '\n';
		}

		// Reset list
		for (RegularExpression* regex : lexical_analysis_list)
			regex->reset();

	}
}


// Others
void Interpreter::print()
{
	std::cout << "Lexical analysis:\n\n";

	for (RegularExpression* regex : lexical_analysis_list)
		regex->print();
	std::cout << "\n";
}
