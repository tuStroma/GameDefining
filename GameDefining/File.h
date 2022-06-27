#pragma once
#include <string>
#include <fstream>
#include <iostream>

#define PAGE_SIZE 1


class File
{
private:
	std::string file_path;
	bool file_exists;

	char page_buffer[PAGE_SIZE];
	int in_buffer = 0;
	int page_number = 0;
	int page_offset = 0;
	int cursor = 0;
	bool buffer_filled = false;

	void getPage(int page_number);

public:
	File(std::string path);
	~File();

	char next();
	char seek(int character_number);

	int getCursorPosition();
	void setCursorPosition(int cursor);
};

