#include "Symbol.h"

Symbol::Symbol(SymbolType type)
{
	this->type = type;
}

SymbolType Symbol::getSymbolType()
{
	return type;
}
