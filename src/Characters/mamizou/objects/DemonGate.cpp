//
// Created by PinkySmile on 18/10/2025.
//

#include "DemonGate.hpp"
#include "../Mamizou.hpp"

#define disappearing gpShort[0]
#define appearTimer gpShort[1]
#define spawnCount gpShort[2]

static const std::vector<unsigned char> sequences[4] = {
	{5, 9, 17},
	{4, 6, 15, 16},
	{1, 2, 3, 7, 10, 11, 18},
	{8, 12, 13, 14}
};

void DemonGate::update()
{
	float params[3] = {0, 0, 0};

	if (this->checkTurnIntoCrystals(true, 2, 5)) {
		this->lifetime = 0;
		return;
	}
	switch (this->frameState.sequenceId) {
	case 0:
		if (this->frameState.currentFrame == 0) {
			if (this->parentPlayerB->frameState.actionId == Mamizou::ACTION_a1_214b)
				this->appearTimer = 45;
			else if (this->parentPlayerB->frameState.actionId == Mamizou::ACTION_a1_214c)
				this->appearTimer = 105;
		}
		this->advanceFrame();
		if (this->disappearing == 0) {
			if (
				this->parentPlayerB->frameState.actionId == Mamizou::ACTION_a1_214b ||
				this->parentPlayerB->frameState.actionId == Mamizou::ACTION_a1_214c
			)
				this->spawnCount = this->parentPlayerB->gpShort[5];
			if (this->renderInfos.scale.x < 1) {
				this->renderInfos.scale.x += 0.05;
				this->renderInfos.scale.y += 0.05;
				if (this->frameState.currentFrame % 4 == 0)
					this->createObject(810, this->position.x + 75 - SokuLib::rand(150), this->position.y, this->direction, 1, params);
			} else if (this->appearTimer != 0)
				this->appearTimer--;
			else
				this->disappearing = 1;
		} else if (this->disappearing == 1) {
			if (this->appearTimer == 0) {
				if (this->spawnCount == 0) {
					this->disappearing = 2;
					break;
				}

				auto &arr = sequences[max(1, min(this->parentPlayerB->effectiveSkillLevel[6], 4)) - 1];
				auto r = SokuLib::rand(arr.size());

				if (r >= arr.size()) r = arr.size() - 1;
				params[0] = arr[r];
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				this->appearTimer = 30;
				this->spawnCount--;
			} else
				this->appearTimer--;
		} else {
			this->renderInfos.scale.x -= 0.1;
			this->renderInfos.scale.y -= 0.1;
			if (this->frameState.currentFrame % 4 == 0)
				this->createObject(810, this->position.x + 75 - SokuLib::rand(150), this->position.y, this->direction, 1, params);
			if (this->renderInfos.scale.x <= 0)
				this->lifetime = 0;
		}
		break;
	default:
		this->advanceFrame();
		if (this->position.x < -160 || this->position.x > 1480) {
			this->lifetime = 0;
			break;
		}
		if (
			this->collisionType == COLLISION_TYPE_BULLET_COLLIDE_HIGH_DENSITY ||
			this->collisionType == COLLISION_TYPE_BULLET_COLLIDE_SAME_DENSITY ||
			this->collisionType == COLLISION_TYPE_GRAZED
		) {
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = 1;
		}
		if (this->renderInfos.scale.x < 1) {
			this->renderInfos.scale.x += 0.05;
			this->renderInfos.scale.y += 0.05;
		} else {
			this->renderInfos.scale.x = 1;
			this->renderInfos.scale.y = 1;
		}
		this->position.x += 10 * this->direction;
		if (this->frameState.currentFrame % 12 == 0)
			this->createObject(810, this->position.x + 50 - SokuLib::rand(100), this->position.y, this->direction, 1, params);
		break;
	}
}

void DemonGate::initializeAction()
{
	if (this->customData)
		this->setSequence(this->customData[0]);
	this->skillIndex = 6;
	switch (this->frameState.sequenceId) {
	case 0:
		this->disappearing = 0;
		this->appearTimer = 0;
		this->renderInfos.scale.x = 0;
		this->renderInfos.scale.y = 0;
		break;
	case 19:
		break;
	default:
		this->renderInfos.scale.x = 0.5;
		this->renderInfos.scale.y = 0.5;
		this->collisionLimit = 1;
		break;
	}
}
