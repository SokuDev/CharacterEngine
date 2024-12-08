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
	if (this->frameState.sequenceId == 0 && this->checkProjectileHit(4))
		this->nextSequence();
	if (this->frameState.sequenceId == 1) {
		if (this->renderInfos.color.a < 11) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 11;
		this->renderInfos.zRotation += 2;
	}
}

void Bamboo::initializeAction()
{
	this->collisionLimit = 1;
	this->renderInfos.zRotation = 22;
}
