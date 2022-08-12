#pragma once
#include <string>
#include <iostream>

#include "../definition/source/symbolsEnum.h"

class Symbol
{
private:
	SymbolType type;

public:
	Symbol(SymbolType type);

	SymbolType getSymbolType();
};
