//
// Created by PinkySmile on 28/09/24.
//

#include <cmath>
#include "Bamboo.hpp"

void Bamboo::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	this->advanceFrame();
	this->checkGrazed(0);
	if (this->frameState.sequenceId == 0) {
		if (this->checkProjectileHit(4))
			this->nextSequence();
	} else if (this->frameState.sequenceId == 1) {
		if (this->renderInfos.color.a < 11) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 11;
		this->renderInfos.zRotation += 2;
	} else if (this->frameState.sequenceId == 2) {
		if (this->renderInfos.color.a <= 15) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.zRotation += this->direction * 2;
		this->renderInfos.color.a -= 15;
		this->position.x += this->customData[0];
		this->position.y += this->customData[1];
		this->customData[1] -= 0.5;
	}
}

void Bamboo::initializeAction()
{
	if (this->customData) {
		this->setSequence(this->customData[2]);
		return;
	}
	this->collisionLimit = 1;
	this->renderInfos.zRotation = 22;
}
