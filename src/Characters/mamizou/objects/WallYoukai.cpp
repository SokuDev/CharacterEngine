//
// Created by PinkySmile on 13/06/2026.
//

#include "WallYoukai.hpp"
#include "../Mamizou.hpp"

#define version gpShort[0]

void WallYoukai::update()
{
	float params[3] = {0, 0, 5};

	switch (this->frameState.sequenceId) {
	case 0:
		if (this->frameState.currentFrame == 0) {
			if (this->parentPlayerB->frameState.actionId == Mamizou::ACTION_a1_236b)
				this->version = 0;
			else if (this->parentPlayerB->frameState.actionId == Mamizou::ACTION_a1_236c)
				this->version = 1;
		}
		this->position.x = this->parentPlayerB->position.x - 40 * this->parentPlayerB->direction;
		this->position.y = this->parentPlayerB->position.y + 180;
		if (this->renderInfos.scale.x < 1) {
			this->renderInfos.scale.x += 0.1;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (
				this->parentPlayer->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
				this->parentPlayer->frameState.actionId < SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN
			) {
				this->renderInfos.scale.x = 1;
				this->renderInfos.scale.y = 1;
			}
			if (this->renderInfos.scale.x >= 1) {
				this->collisionType = COLLISION_TYPE_NONE;
				this->collisionLimit = 1;
			}
		}
		if (
			this->parentPlayer->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
			this->parentPlayer->frameState.actionId < SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN
		) {
			std::swap(this->gameData.ally, this->gameData.opponent);
			this->setSequence(3);
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = 1;
		} else if (
			this->parentPlayerB->frameState.sequenceId == 2 &&
			this->parentPlayerB->frameState.poseId == 1
		) {
			if (this->version) {
				this->speed.x = 0;
				this->speed.y = 15;
			} else {
				this->speed.x = 8;
				this->speed.y = 12;
			}
			this->nextSequence();
			this->layer = 1;
		}
		break;
	case 1:
		if (this->checkTurnIntoCrystals(false, 1, 5)) {
			this->lifetime = 0;
			return;
		}
		this->speed.y -= this->gravity.y;
		if (this->speed.y <= 0)
			this->nextSequence();
		if (this->collisionType != COLLISION_TYPE_NONE)
			this->gpShort[0] = 1;
		break;
	case 2:
		if (this->checkTurnIntoCrystals(false, 1, 5)) {
			this->lifetime = 0;
			return;
		}
	case 3:
		this->speed.y -= this->gravity.y;
		if (this->speed.y + this->gravity.y >= -5 && this->speed.y < -5 && this->gpShort[0] == 1) {
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = 1;
		}
		if (this->isOnGround()) {
			this->position.y = this->getGroundHeight();
			if (this->parentPlayerB->effectiveSkillLevel[7] >= 4) {
				this->createObject(this->frameState.actionId, 0, this->position.y, SokuLib::RIGHT, 1, params);
				SokuLib::playSEWaveBuffer(SokuLib::SFX_CATFISH_LAND);
				SokuLib::camera.shake = 15;
			} else
				this->parentPlayerB->playSFX(2);
			this->setSequence(4);
			this->resetForces();
		}
		break;
	case 4:
		this->advanceFrame();
		if (this->frameState.currentFrame > 20) {
			this->renderInfos.color.a -= 5;
			if (this->renderInfos.color.a == 0)
				this->lifetime = 0;
		}
		break;
	case 5:
		if (this->advanceFrame())
			this->lifetime = 0;
		break;
	case 6:
		if (!this->parentB || this->parentB->frameState.sequenceId != 0) {
			this->lifetime = 0;
			return;
		}
		this->position = this->parentB->position;
		this->renderInfos.scale = this->parentB->renderInfos.scale;
	}
	this->position.x += this->speed.x * this->direction;
	this->position.y += this->speed.y;
}

void WallYoukai::initializeAction()
{
	if (!this->customData) {
		float params[] = {0, 0, 6};

		this->renderInfos.scale.x = 0;
		this->renderInfos.scale.y = 0;
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
	} else
		this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 5) {
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
	}
	this->skillIndex = 7;
	this->gravity.y = 1;
}
