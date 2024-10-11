//
// Created by PinkySmile on 09/10/24.
//

#ifndef CHARACTERENGINE_GIUROLL_HPP
#define CHARACTERENGINE_GIUROLL_HPP


#include <cstdint>

namespace GiuRoll
{
	void load();
	bool isLoaded();
	void setCharDataSize(size_t size);
	void setCharDataPos(size_t id, size_t chrSize, size_t objSize);
}


#endif //CHARACTERENGINE_GIUROLL_HPP
