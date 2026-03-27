//
// Created by PinkySmile on 25/03/2026.
//

#include "../Mamizou.hpp"
#include "YoumuClone.hpp"

void YoumuClone::update()
{
	auto m = reinterpret_cast<Mamizou *>(this->parentPlayerB);
	auto y = reinterpret_cast<SokuLib::v2::PlayerYoumu *>(m->_transformPlayer);
	auto &current = m->getCurrentCloneFrame();

	this->center = current.center;
	this->direction = current.dir;
	this->position = current.pos;
	this->renderInfos = current.infos;
	this->frameData = current.frameData;
	this->gameData.frameData = current.frameData;
	this->renderInfos.shaderType = 1;
	this->refreshSprite();

	if (!y->youmuCloneTimeLeft) {
		this->renderInfos.color.a = 0x00;
		this->collisionLimit = 0;
	} else if (!m->isTransformed() || !current.transformed) {
		if (current.resetHit) {
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = current.hitCount;
		}
		this->renderInfos.color.a = 0x80;
		y->objectList->getList().front()->renderInfos.color.a = 0;
	} else {
		this->renderInfos.color.a = 0x00;
		this->collisionLimit = 0;
		y->objectList->getList().front()->renderInfos.color.a = 0x80;
	}
}

void YoumuClone::initializeAction()
{
	this->renderInfos.color = SokuLib::Color{0xFF, 0xFF, 0xFF, 0x80};
	this->renderInfos.shaderType = 1;
}
