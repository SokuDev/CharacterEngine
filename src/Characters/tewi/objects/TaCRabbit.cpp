//
// Created by PinkySmile on 09/07/24.
//

#include <cmath>
#include "TaCRabbit.hpp"
#include "../Tewi.hpp"

#define T 15
#define disabled gpShort[0]

void TaCRabbit::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->checkTurnIntoCrystals(false, 2, 2)) {
		this->lifetime = 0;
		return;
	}

	if (this->lifetime == 1) {
		this->renderInfos.color.a -= 15;
		if (!this->renderInfos.color.a)
			this->lifetime = 0;
		return;
	}

	auto hammer = ((Tewi *)this->parentPlayerB)->getHammer();

	this->advanceFrame();
	if (!hammer || this->disabled) {
		this->disabled = 1;
		if (this->renderInfos.color.a <= 25)
			this->lifetime = 0;
		else
			this->renderInfos.color.a -= 25;
		return;
	}
	if (this->HP <= 0) {
		this->renderInfos.color.a -= 15;
		if (!this->renderInfos.color.a)
			this->lifetime = 0;
		this->renderInfos.zRotation += 5;
		this->speed.x = -2;
		this->speed.y = -2;
	} else if (this->frameState.sequenceId == 1) {
		if (this->frameState.poseId < 2) {
			if (this->position.x > this->parentPlayerB->position.x)
				this->direction = SokuLib::RIGHT;
			else
				this->direction = SokuLib::LEFT;
			hammer->renderInfos.zRotation = 0;
			hammer->position = this->position;
			hammer->position.y += 20;
			hammer->position.x += -35 * this->direction;
		} else if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
			hammer->customData[1] = 30;

			auto target = fmod(atan2(this->parentPlayerB->position.y + 75 - this->position.y, this->parentPlayerB->position.x - this->position.x) * 180 / M_PI + 360, 360);

			hammer->customData[0] = target;
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_HIT);
			this->createEffect(2, hammer->position.x, hammer->position.y, this->direction, 1);
			this->createEffect(129, (-this->direction * 80) + hammer->position.x, this->position.y, -this->direction, -1);
			hammer->collisionLimit = 1;
			hammer->collisionType = COLLISION_TYPE_NONE;
			hammer->customData[3] = 0;
			hammer->layer = 1;
		} else if (this->frameState.poseId >= 3) {
			if (this->renderInfos.color.a <= 25)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 25;
		}
	} else if (hammer->frameState.sequenceId == 8) {
		hammer->customData[1] = 0;
		hammer->direction = this->direction;
		hammer->position = this->position;
		hammer->position.x += 35 * this->direction;
		hammer->position.y += 20;
		hammer->layer = 2;
		if (this->isOnGround() && this->frameState.poseId == 0)
			this->nextSequence();
	} else if (this->_canGrabHammer(hammer)) {
		SokuLib::playSEWaveBuffer(SokuLib::SFX_SLAP_HIT);
		hammer->setSequence(8);
		hammer->skillIndex = 3;
		hammer->collisionLimit = 0;
		hammer->customData[3] = 1;
	} else if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
		if (this->position.x > hammer->position.x)
			this->direction = SokuLib::LEFT;
		else
			this->direction = SokuLib::RIGHT;
		if (std::abs(this->position.x - hammer->position.x) > 300) {
			this->speed.x = 15;
			this->speed.y = 5;
		} else {
			auto t = T;

			if (hammer->position.y - this->position.y > 600)
				t *= 4;
			else if (hammer->position.y - this->position.y > 300)
				t *= 2;
			this->speed.x = (hammer->position.x - this->position.x) / (t * 1.f) * this->direction;
			this->speed.y = (hammer->position.y - this->position.y + 0.5f * t * t / 2) / (t * 1.f);
		}
		this->parentPlayer->playSFX(3);
	}
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

	// TODO: Find a better way to do this
	if (this->parentPlayerB->frameState.poseId == 0)
		switch (this->parentPlayerB->frameState.actionId) {
		case Tewi::ACTION_d22B:
		case Tewi::ACTION_d22C:
		case Tewi::ACTION_jd22B:
		case Tewi::ACTION_jd22C:
			this->lifetime = 0;
			if (hammer->frameState.sequenceId != 8)
				break;
			if (hammer->position.x == 0 || hammer->position.x == 1280)
				hammer->setSequence(6);
			else if (hammer->isOnGround())
				hammer->setSequence(1);
			else
				hammer->setSequence(0);
			hammer->customData[3] = 0;
			hammer->customData[2] = 1;
			hammer->skillIndex = -1;
			hammer->collisionLimit = 0;
			break;
		}
}

void TaCRabbit::initializeAction()
{
	this->skillIndex = 3;
	this->HP = 1;
	this->lifetime = 2;
}

bool TaCRabbit::_canGrabHammer(const SokuLib::v2::GameObject *hammer) const
{
	if (hammer->frameState.sequenceId == 0) {
		if (std::abs(hammer->position.x - this->position.x) > 100)
			return false;
		if (hammer->position.y + 50 < this->position.y)
			return false;
		if (hammer->position.y - 50 > this->position.y)
			return false;
	} else if (hammer->renderInfos.zRotation == 0) {
		if (std::abs(hammer->position.x - this->position.x) > 75)
			return false;
		if (std::abs(hammer->position.y - this->position.y) > 50)
			return false;
	} else {
		if (hammer->position.y - this->position.y > 35)
			return false;
		if (hammer->position.y - this->position.y < -35)
			return false;
		if (std::abs(hammer->position.x - this->position.x) > 150)
			return false;
	}
	return true;
}
