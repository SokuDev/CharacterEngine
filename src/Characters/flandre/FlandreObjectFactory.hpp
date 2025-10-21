//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_FLANDREOBJECTFACTORY_HPP
#define CHARACTERENGINE_FLANDREOBJECTFACTORY_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

class FlandreObjectFactory {
public:
	static SokuLib::v2::GameObject *construct(SokuLib::v2::Player *owner, short action);
};


#endif //CHARACTERENGINE_FLANDREOBJECTFACTORY_HPP
