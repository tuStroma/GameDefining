#include "File.h"


File::File(std::string path)
{
	file_path = path;

	std::fstream file_stream;

	file_stream.open(file_path);

	if (!file_stream.is_open())
	{
		std::cout << "File does not exist\n";
		file_exists = false;
		return;
	}

	file_exists = true;
}

File::~File()
{
	std::cout << "Deleting file object\n";
}

void File::getPage(int page_number)
{
	// Binary input is used to filter out '\r' before '\n' character
	std::ifstream file_stream(file_path, std::ios_base::binary);


	if (!file_stream.is_open())
	{
		std::cout << "File can't be opened\n";
		return;
	}

	file_stream.seekg(page_number * PAGE_SIZE);
	file_stream.read(page_buffer, PAGE_SIZE);
	in_buffer = (int)file_stream.gcount();
	file_stream.close();

	this->page_number = page_number;
	page_offset = 0;
	cursor = page_number * PAGE_SIZE;
	buffer_filled = true;
}

char File::next()
{
	//page_offset++;

	if (!buffer_filled)
	{
		getPage(0);
	}

	if (page_offset >= PAGE_SIZE)
		getPage(page_number + 1);

	if (page_offset >= in_buffer)
		return char(0);

	char next_char = page_buffer[page_offset];

	cursor++;
	page_offset++;

	if (next_char == '\r')
		return next();

	else
		return next_char;
}

char File::seek(int character_number)
{
	page_number = character_number / PAGE_SIZE;
	getPage(page_number);
	page_offset = character_number % PAGE_SIZE;
	
	char c = 0;
	if (in_buffer > page_offset)
	{
		c = page_buffer[page_offset];

		cursor++;
		page_offset++;

		if (c == '\r')
			return next();

		return c;
	}

	
	return char(0);
}

int File::getCursorPosition()
{
	return page_number * PAGE_SIZE + page_offset;
}

void File::setCursorPosition(int cursor)
{
	page_number = cursor / PAGE_SIZE;
	getPage(page_number);
	page_offset = cursor % PAGE_SIZE;
}

