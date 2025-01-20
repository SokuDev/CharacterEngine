//
// Created by PinkySmile on 09/01/25.
//

#include "FrogShadow.hpp"

#define disjointed gpShort[0]

void FrogShadow::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->parentPlayerB->hitStop && this->parentPlayerB->frameState.actionId == SokuLib::ACTION_j2A)
		return;
	this->advanceFrame();
	if (this->frameState.sequenceId == 2) {
		if (this->frameState.poseId == 4) {
			this->renderInfos.color.a -= 15;
			if (this->renderInfos.color.a == 0)
				this->lifetime = 0;
		}
	} else if (this->frameState.sequenceId != 2) {
		if (this->disjointed == 0) {
			if (
				this->parentPlayerB->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
				this->parentPlayerB->frameState.actionId < SokuLib::ACTION_FORWARD_DASH
			) {
				this->disjointed = 1;
				return;
			}
			this->speed = this->parentPlayerB->speed;
			this->position.x = this->parentPlayerB->position.x + this->parentPlayerB->speed.x;
			this->position.y = this->parentPlayerB->position.y + this->parentPlayerB->speed.y;
			this->gravity = this->parentPlayerB->gravity;
			if (this->parentPlayerB->frameState.sequenceId == 2) {
				this->setSequence(2);
				this->disjointed = 1;
				this->resetForces();
			} else if (this->parentPlayerB->frameState.actionId != SokuLib::ACTION_j2A)
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
				this->setSequence(2);
				this->resetForces();
			}
		}
	}
}

void FrogShadow::initializeAction()
{

}
