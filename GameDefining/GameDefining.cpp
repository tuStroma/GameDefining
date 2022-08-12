#include <iostream>
#include <string>
#include <list>

#include "Interpreter/Interpreter.h"
#include "File/File.h"

#include "definition/source/InterpreterDefinition.h"

void file_test()
{
	std::string test_path = "source/test.txt";

	File* test_file = new File(test_path);

	char c = test_file->next();

	while (c)
	{
		if (c == '\n') std::cout << "\\n";
		else if (c == '\t') std::cout << "\\t";
		else std::cout << c;

		std::cout << ' ' << int(c) << '\n';
		//c = test_file->next();
		c = test_file->seek(test_file->getCursorPosition());
	}
	std::cout << "\n\n\n";

	std::cout << test_file->getCursorPosition() << '\n';
}

void regex_test()
{
	/*/
	RegularExpression* regex = new RegularExpression(3, 4);

	//regex->print();

	regex->setTablePosition(2, 3, 2);
	regex->setTablePosition(0, 0, 1);
	regex->setTablePosition(1, 3, 0);
	regex->setTablePosition(2, 0, 1);

	regex->setEndState(2);

	regex->setAcceptFunction(0, 
		[](char c) { 
			return (c >= 'a' && c <= 'z'); 
		}, 
		"a-z");

	regex->setGenerateFunction (Symbol[](std::string found) {});


	regex->print();



	RegularExpression* multiline_comment = new RegularExpression(5, 3);

	
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
		std::cout << "Multiline comment:\n" << found << '\n';
		});

	multiline_comment->print();

	std::string str = "/*sjdfsdmdsfshdf*sdfsdf**sdfsdf \n* sdkfhksdfsdfsdf";
	int i = 0;
	while (str[i])
	{
		bool b = multiline_comment->passCharacter(str[i]);
		if (!b)
		{
			int fit = multiline_comment->getLastFit();
			multiline_comment->generateValue(str.substr(0, fit));
			break;
		}

		i++;
	}

	regex->~RegularExpression();
	multiline_comment->~RegularExpression();
	//*/
}



void func1()
{
	std::cout << "Hello\n";
}

void func2(int a)
{
	std::cout << "Hello " << a << "\n";
}

bool func3(char c)
{
	bool b = (c == '3');
	//std::cout << b << '\n';
	return b;
}

struct function_pointer
{
	bool (*function)(char);
};

void f_pointer_test()
{
	void (*f1)() = func1;
	void (*f2)(int) = func2;

	f1();
	f2(2);
	std::cout << "\n\n\n";


	bool (*f3[3])(char);
	f3[0] = func3;

	f3[0]('3');
	f3[0]('2');

	std::cout << "\n\n\n";


	std::cout << "Size of function pointer: \t" << sizeof(f1) << '\n';
	std::cout << "Size of function pointer: \t" << sizeof(f2) << '\n';
	function_pointer* fp = (function_pointer*)malloc(3 * sizeof(function_pointer));
	fp[0].function = func3;
	fp[0].function('3');
	//fp = (bool(**fp)(char)) 

	std::cout << "\n\n\n";


	//RegularExpression::CharAcceptFunction* caf1 = new RegularExpression::CharAcceptFunction(func3, "3");
	//RegularExpression::CharAcceptFunction* caf2 = new RegularExpression::CharAcceptFunction([](char c)
	//	{ return (c >= 'a' && c <= 'z'); },
	//	"a-z");
	//
	//std::cout << caf1->getName() << ": " << caf1->Check('3') << caf1->Check('4') << '\n';
	//std::cout << caf2->getName() << ": " << caf2->Check('3') << caf2->Check('f') << caf2->Check('F') << '\n';
}


void list_test()
{
	std::list<int> l = { 1, 3, 5, 7, 9 };
	std::list<int> l2 = { 2, 4, 6, 8 };


	for (int e : l) 
		std::cout << e << ' ';
	std::cout << '\n';

	for (std::list<int>::iterator it = l.begin(); it != l.end();) {
		if (*it % 3 == 0)
		{
			std::cout << "Usuwam: " << *it << '\n';
			it = l.erase(it);
		}
		else
			it++;
	}

	for (int e : l)
		std::cout << e << ' ';
	std::cout << '\n';

	l = l2;

	l.push_back(69);

	for (int e : l)
		std::cout << e << ' ';
	std::cout << '\n';



}

void interpreter_test()
{
	Interpreter* inter = createInterpreter();

	inter->Analysis("source/test.txt");
}

int main()
{
	//file_test();

	//f_pointer_test();
	
	//regex_test();

	//list_test();

	interpreter_test();
	
}