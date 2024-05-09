//
// Created by PinkySmile on 09/05/24.
//

#include <SokuLib.hpp>
#include "Memory.hpp"

void *operator new(size_t t)
{
	return SokuLib::NewFct(t);
}

void operator delete(void *p)
{
	SokuLib::DeleteFct(p);
}