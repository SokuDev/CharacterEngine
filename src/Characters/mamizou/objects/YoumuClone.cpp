//
// Created by PinkySmile on 25/03/2026.
//

#include "../Mamizou.hpp"
#include "YoumuClone.hpp"

void YoumuClone::update()
{
	auto &next = this->_data[this->_dataPointer];
	auto m = reinterpret_cast<Mamizou *>(this->parentPlayerB);
	auto y = reinterpret_cast<SokuLib::v2::PlayerYoumu *>(m->_transformPlayer);

	this->_dataPointer++;
	this->_dataPointer %= CLONE_BUFFERING_DELAY + 1;

	auto &current = this->_data[this->_dataPointer];

	this->sprite = current.sprite;
	this->center = current.center;
	this->direction = current.dir;
	this->frameData = current.img;
	this->position = current.pos;
	this->renderInfos = current.infos;
	this->frameData = current.img;
	this->gameData.frameData = &current.data;
	this->renderInfos.shaderType = 1;

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

	next.transformed = m->isTransformed();
	next.sprite = m->sprite;
	next.center = m->center;
	next.resetHit = m->frameState.currentFrame == 0 && m->frameState.sequenceId == 0;
	next.dir = m->direction;
	next.hitCount = m->collisionLimit;
	next.img = m->frameData;
	memcpy(&next.data, m->gameData.frameData, sizeof(*m->gameData.frameData));
	next.data.onHitPStun = 0;
	next.pos = m->position;
	next.infos = m->renderInfos;
}

void YoumuClone::initializeAction()
{
	auto m = reinterpret_cast<Mamizou *>(this->parentPlayerB);

	for (auto &dat : this->_data) {
		dat.transformed = m->isTransformed();
		dat.sprite = m->sprite;
		dat.center = m->center;
		dat.resetHit = m->frameState.currentFrame == 0 && m->frameState.sequenceId == 0;
		dat.dir = m->direction;
		dat.hitCount = m->collisionLimit;
		dat.img = m->frameData;
		memcpy(&dat.data, m->gameData.frameData, sizeof(*m->gameData.frameData));
		dat.data.onHitPStun = 0;
		dat.pos = m->position;
		dat.infos = m->renderInfos;
	}
	this->renderInfos.color = SokuLib::Color{0xFF, 0xFF, 0xFF, 0x80};
	this->renderInfos.shaderType = 1;
}
