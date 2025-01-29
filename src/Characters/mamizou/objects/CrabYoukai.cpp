//
// Created by PinkySmile on 09/01/25.
//

#include "CrabYoukai.hpp"
#include "../Mamizou.hpp"

#define disjointed gpShort[0]

void CrabYoukai::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->parentPlayerB->hitStop && this->parentPlayerB->frameState.actionId == SokuLib::ACTION_j2A)
		return;
	if (this->frameState.sequenceId == 0) {
		if (this->renderInfos.scale.x == 1)
			return static_cast<void>(this->nextSequence());
		this->renderInfos.color.a += 15;
		this->renderInfos.scale.x = this->renderInfos.color.a / 255.f;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (
			this->parentPlayerB->frameState.actionId != Mamizou::ACTION_d214b &&
			this->parentPlayerB->frameState.actionId != Mamizou::ACTION_jd214b
		)
			this->lifetime = 0;
	}
	if (this->frameState.sequenceId == 1) {
		if (this->parentPlayerB->speed.y < 0)
			this->nextSequence();
	}

	if (this->frameState.sequenceId == 3) {
		this->renderInfos.color.a -= 15;
		if (this->renderInfos.color.a == 0)
			this->lifetime = 0;
		return;
	}
	if (this->disjointed == 0) {
		if (
			this->parentPlayerB->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
			this->parentPlayerB->frameState.actionId < SokuLib::ACTION_FORWARD_DASH
		) {
			this->disjointed = 1;
			return;
		}
		this->speed = this->parentPlayerB->speed;
		this->position.x = this->parentPlayerB->position.x + this->parentPlayerB->speed.x + (this->direction * 50);
		this->position.y = this->parentPlayerB->position.y + this->parentPlayerB->speed.y + 80;
		this->gravity = this->parentPlayerB->gravity;
		if (this->parentPlayerB->frameState.sequenceId == 2) {
			this->setSequence(3);
			this->disjointed = 1;
			this->resetForces();
		} else if (
			this->parentPlayerB->frameState.actionId != Mamizou::ACTION_d214b &&
			this->parentPlayerB->frameState.actionId != Mamizou::ACTION_jd214b
		)
			this->lifetime = 0;
	} else {
		this->position.x += this->speed.x * this->direction;
		this->position.y += this->speed.y;
		this->speed.y -= this->gravity.y;
		if (this->position.x > 1240)
			this->position.x = 1240;
		else if (this->position.x < 40)
			this->position.x = 40;
		if (this->isOnGround()) {
			this->setSequence(3);
			this->resetForces();
		}
	}
}

void CrabYoukai::initializeAction()
{
	this->renderInfos.scale.x = 0;
	this->renderInfos.scale.y = 0;
	this->renderInfos.color.a = 0;
}
