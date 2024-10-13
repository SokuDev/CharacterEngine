//
// Created by PinkySmile on 09/05/24.
//

#include "BasicObject.hpp"

SokuLib::v2::GameObject* BasicObject::createObject(short action, float x, float y, char direction, char layer, float *extraData, unsigned int extraDataSize)
{
	return this->parentPlayerB->objectList->createObject(nullptr, this->gameData.ally, action, x, y, direction, layer, extraData, extraDataSize);
}

SokuLib::v2::GameObject* BasicObject::createChild(short action, float x, float y, char direction, char layer, float *extraData, unsigned int extraDataSize)
{
	return this->parentPlayerB->objectList->createObject(this, this->gameData.ally, action, x, y, direction, layer, extraData, extraDataSize);
}

void BasicObject::update()
{
	// Update common objects
	if (this->frameState.actionId >= 1000)
		return ((void (__thiscall *)(SokuLib::v2::GameObject *))0x48DB20)(this);
	if (this->advanceFrame())
		this->lifetime = 0;
}

void BasicObject::initializeAction()
{
	// Init common objects
	((void (__thiscall *)(SokuLib::v2::GameObject *))0x48D000)(this);
}