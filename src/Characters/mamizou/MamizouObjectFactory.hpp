//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_MAMIZOUOBJECTFACTORY_HPP
#define CHARACTERENGINE_MAMIZOUOBJECTFACTORY_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

class MamizouObjectFactory {
public:
	static SokuLib::v2::GameObject *construct(SokuLib::v2::Player *owner, short action);
};


#endif //CHARACTERENGINE_MAMIZOUOBJECTFACTORY_HPP
