//
// Created by PinkySmile on 09/07/24.
//

#include "Rabbit.hpp"
#include "Tewi.hpp"

void Rabbit::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->customData[2] == 0 && this->checkTurnIntoCrystals(false, 2, 2)) {
		this->lifetime = 0;
		return;
	}
	this->advanceFrame();
	this->checkGrazed(this->customData[2]);
	this->checkProjectileHit(this->customData[2]);
	if (0 < this->customData[2] && (this->customData[2] <= this->otherProjectileHit || this->customData[2] <= this->grazeCounter)) {
		this->renderInfos.color.a -= 15;
		if (!this->renderInfos.color.a)
			this->lifetime = 0;
	} else if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
		this->speed.x = this->customData[0];
		this->speed.y = this->customData[1];
		this->parentPlayer->playSFX(3);
	}
	if (this->frameState.poseId == 4 && this->isOnGround()) {
		this->resetForces();
		this->nextPose();
	}
	this->position.x += this->speed.x * this->direction;
	this->position.y += this->speed.y;
	if (this->isOnGround())
		this->position.y = this->getGroundHeight();
	this->speed.y -= 0.5;
	if (this->position.x < 0 || this->position.x > 1300)
		this->lifetime = 0;
	// TODO: Find a better way to do this
	if (this->parentPlayerB->frameState.poseId == 0 && this->customData[2] != 0)
		switch (this->parentPlayerB->frameState.actionId) {
		case Tewi::ACTION_d236B:
		case Tewi::ACTION_d236B_HAMMER:
		case Tewi::ACTION_d236C:
		case Tewi::ACTION_d236C_HAMMER:
			this->lifetime = 0;
			break;
		}
}

bool Rabbit::initializeAction()
{
	this->collisionLimit = 1;
	return true;
}
