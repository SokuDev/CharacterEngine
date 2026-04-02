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
	if (this->frameState.actionId == 980)
		this->updateWeatherDebuffEffect();
	if (this->frameState.actionId >= 1000)
		return this->updateDefaultObject();

	if (this->advanceFrame())
		this->lifetime = 0;
}

void BasicObject::initializeAction()
{
	if (this->frameState.actionId == 980)
		return this->initWeatherDebuffEffect();
	this->initDefaultObject();
}