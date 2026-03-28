//
// Created by PinkySmile on 27/03/2026.
//

#include "UdongeClone.hpp"
#include "../Mamizou.hpp"

void UdongeClone::update()
{
	auto m = reinterpret_cast<Mamizou *>(this->parentPlayerB);
	auto r = reinterpret_cast<SokuLib::v2::PlayerUdonge *>(m->_transformPlayer);

	this->center = this->parentPlayerB->center;
	this->direction = this->parentPlayerB->direction;
	this->position = this->parentPlayerB->position;
	this->renderInfos = this->parentPlayerB->renderInfos;
	this->frameData = this->parentPlayerB->frameData;
	this->gameData.frameData = this->parentPlayerB->gameData.frameData;
	this->refreshSprite();

	this->position.x += this->gpFloat[0];

	GameObject *src = nullptr;

	for (auto obj : r->objectList->getList()) {
		printf("%i %f %i %f\n", obj->frameState.actionId, obj->customData[1], this->frameState.actionId, this->gpFloat[0]);
		if (obj->frameState.actionId == 847 && obj->customData[1] == this->gpFloat[0]) {
			src = obj;
			break;
		}
	}
	if (!src || m->isTransformed()) {
		this->renderInfos.color.a = 0x00;
		this->gameData.frameData = &(*this->gameData.patternMap)[0]->frames.front();
		this->frameData = this->gameData.frameData;
	} else if (!m->isTransformed())
		this->renderInfos.color.a = src->renderInfos.color.a;
}

void UdongeClone::initializeAction()
{
	if (this->frameState.actionId == 1)
		this->gpFloat[0] = -200;
	else
		this->gpFloat[0] = 200;
	this->allowEntity = false;
	this->renderInfos.color = SokuLib::Color{0xFF, 0xFF, 0xFF, 0x80};
	this->renderInfos.shaderType = 1;
	this->setParent(this->parentPlayerB);
}