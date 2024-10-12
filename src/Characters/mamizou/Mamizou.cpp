//
// Created by PinkySmile on 11/10/24.
//

#include "Mamizou.hpp"
#include "MamizouObjectFactory.hpp"
#include "GameObjectList.hpp"

Mamizou::Mamizou(SokuLib::PlayerInfo &info) :
	SokuLib::v2::Player(info)
{
	this->objectList = new GameObjectList<MamizouObjectFactory>(this);
}

void Mamizou::render()
{
	AnimationObject::render();
}

void Mamizou::update()
{
	int old = this->frameState.poseFrame;

	this->updateDefaultBehavior();
	// Fail-safe to prevent softlocks
	if (old == this->frameState.poseFrame && this->advanceFrame())
		this->setAction(SokuLib::ACTION_IDLE);
}

void Mamizou::initializeAction()
{
	Player::initializeAction();
}

void Mamizou::handleInputs()
{
}

void Mamizou::VUnknown58()
{
}

void Mamizou::VUnknown5C()
{
}

bool Mamizou::VUnknown60(int a)
{
	return false;
}

bool Mamizou::setAction(short action)
{
	return GameObjectBase::setAction(action);
}

void Mamizou::hook()
{
}

void Mamizou::unhook()
{
}
