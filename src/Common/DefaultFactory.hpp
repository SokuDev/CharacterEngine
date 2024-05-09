//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_DEFAULTFACTORY_HPP
#define CHARACTERENGINE_DEFAULTFACTORY_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

class DefaultFactory {
public:
	static SokuLib::v2::GameObject *construct(SokuLib::v2::Player *owner, short action);
};


#endif //CHARACTERENGINE_DEFAULTFACTORY_HPP
