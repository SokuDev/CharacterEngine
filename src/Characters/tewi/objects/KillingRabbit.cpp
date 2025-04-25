//
// Created by PinkySmile on 04/09/24.
//

#include "KillingRabbit.hpp"
#include "../Tewi.hpp"

#define T 10

void KillingRabbit::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->checkTurnIntoCrystals(false, 5, 15)) {
		this->lifetime = 0;
		return;
	}

	if (this->lifetime == 1) {
		this->renderInfos.color.a -= 15;
		if (!this->renderInfos.color.a)
			this->lifetime = 0;
		return;
	}

	auto opponent = this->gameData.opponent;

	this->checkGrazed(0);
	this->advanceFrame();
	if (this->hp <= 0) {
		this->renderInfos.color.a -= 15;
		if (!this->renderInfos.color.a)
			this->lifetime = 0;
		this->renderInfos.zRotation += 5;
		this->speed.x = -2;
		this->speed.y = -2;
	} else if (this->frameState.sequenceId == 2) {
		if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed = {0, 25};
		}
		if (this->frameState.poseId >= 3) {
			if (this->position.y >= 1000 || this->speed.y <= 0)
				this->lifetime = 0;
		} else {
			this->position = opponent->position;
			this->speed = {0, 0};
		}
	} else if (this->frameState.sequenceId == 1) {
		this->speed = {0, 0};
		this->position = opponent->position;
		this->direction = -opponent->direction;
		if (
			this->parentPlayerB->frameState.actionId >= Tewi::ACTION_STAND_PICKUP_HAMMER_FROM_AIR &&
			this->parentPlayerB->frameState.actionId <= Tewi::ACTION_AIR_PICKUP_HAMMER_FROM_GROUND
		) {
			this->nextSequence();
			this->collisionType = COLLISION_TYPE_NONE;
		}
	} else if (this->frameState.sequenceId == 0 && (
		this->collisionType == COLLISION_TYPE_HIT ||
		this->collisionType == COLLISION_TYPE_ARMORED ||
		this->collisionType == COLLISION_TYPE_BLOCKED
	)) {
		this->createEffect(1, this->position.x, this->position.y, this->direction, 1);
		this->nextSequence();
	} else if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
		if (this->position.x > opponent->position.x)
			this->direction = SokuLib::LEFT;
		else
			this->direction = SokuLib::RIGHT;
		if (std::abs(this->position.x - opponent->position.x) > 300) {
			this->speed.x = 25;
			this->speed.y = 7;
		} else {
			auto t = T;

			if (opponent->position.y - this->position.y > 600)
				t *= 4;
			else if (opponent->position.y - this->position.y > 300)
				t *= 2;
			this->speed.x = (opponent->position.x - this->position.x) / (t * 1.f) * this->direction;
			this->speed.y = (opponent->position.y - this->position.y + 0.5f * t * t / 2) / (t * 1.f);
		}
		this->parentPlayer->playSFX(3);
	}
	if (this->frameState.sequenceId == 0)
		this->parentPlayerB->spellBgTimer = 5;
	if (this->frameState.sequenceId == 0 && this->frameState.poseId == 4 && this->isOnGround()) {
		this->resetForces();
		this->nextPose();
	}
	this->position.x += this->speed.x * this->direction;
	this->position.y += this->speed.y;
	if (this->isOnGround())
		this->position.y = this->getGroundHeight();
	this->speed.y -= 0.5;
	if (this->position.x < 40)
		this->position.x = 40;
	if (this->position.x > 1240)
		this->position.x = 1240;
}

void KillingRabbit::initializeAction()
{
	this->skillIndex = 3;
	this->hp = 500;
	this->lifetime = 2;
	this->collisionLimit = 2;
}
