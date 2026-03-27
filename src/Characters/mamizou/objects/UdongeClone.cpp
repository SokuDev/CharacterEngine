//
// Created by PinkySmile on 27/03/2026.
//

#include "UdongeClone.hpp"
#include "../Mamizou.hpp"

void UdongeClone::update()
{
	auto m = reinterpret_cast<Mamizou *>(this->parentPlayerB);
	auto r = reinterpret_cast<SokuLib::v2::PlayerUdonge *>(m->_transformPlayer);
	auto &current = m->getCurrentCloneFrame();

	this->center = this->parentPlayerB->center;
	this->direction = this->parentPlayerB->direction;
	this->position = this->parentPlayerB->position;
	this->renderInfos = this->parentPlayerB->renderInfos;
	this->frameData = this->parentPlayerB->frameData;
	this->gameData.frameData = this->parentPlayerB->gameData.frameData;
	this->refreshSprite();

	if (this->frameState.actionId == 1)
		this->position.x += 200 * this->direction;
	else
		this->position.x -= 200 * this->direction;

	GameObject *src = nullptr;

	for (auto obj : r->objectList->getList()) {
		if (obj->gpShort[0] == this->frameState.actionId) {
			src = obj;
			break;
		}
	}
	if (!src) {
		this->renderInfos.color.a = 0x00;
		this->collisionLimit = 0;
	} else if (m->isTransformed()) {
		if (this->renderInfos.color.a != 0) {
			src->renderInfos.color.a = this->renderInfos.color.a;
			this->renderInfos.color.a = 0;
		}
	} else if (m->isTransformed()) {
		if (this->renderInfos.color.a == 0) {
			this->renderInfos.color.a = src->renderInfos.color.a;
			src->renderInfos.color.a = 0;
		}
	}
}

void UdongeClone::initializeAction()
{
	this->renderInfos.color = SokuLib::Color{0xFF, 0xFF, 0xFF, 0x80};
	this->renderInfos.shaderType = 1;
}