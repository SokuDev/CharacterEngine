//
// Created by PinkySmile on 09/05/24.
//

#include "BasicObject.hpp"

SokuLib::v2::GameObject* BasicObject::createObject(short action, float x, float y, SokuLib::Direction direction, char layer, float *extraData, unsigned int extraDataSize)
{
	return this->parentPlayerB->objectList->createObject(nullptr, this->gameData.ally, action, x, y, direction, layer, extraData, extraDataSize);
}

SokuLib::v2::GameObject* BasicObject::createChild(short action, float x, float y, SokuLib::Direction direction, char layer, float *extraData, unsigned int extraDataSize)
{
	return this->parentPlayerB->objectList->createObject(this, this->gameData.ally, action, x, y, direction, layer, extraData, extraDataSize);
}
