//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_TEWIOBJECTFACTORY_HPP
#define CHARACTERENGINE_TEWIOBJECTFACTORY_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

class TewiObjectFactory {
public:
	static SokuLib::v2::GameObject *construct(SokuLib::v2::Player *owner, short action);
};


#endif //CHARACTERENGINE_TEWIOBJECTFACTORY_HPP
