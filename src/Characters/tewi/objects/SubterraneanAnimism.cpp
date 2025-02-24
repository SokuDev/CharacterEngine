//
// Created by PinkySmile on 27/09/24.
//

#include "SubterraneanAnimism.hpp"

void SubterraneanAnimism::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->checkTurnIntoCrystals(false, 5, 5)) {
		this->lifetime = 0;
		return;
	}
	if (this->frameState.actionId == 858) {
		this->advanceFrame();
		if (this->frameState.poseId == this->frameState.sequenceSize - 1) {
			if (this->renderInfos.color.a < 11)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 11;
		}
		return;
	}
	this->advanceFrame();
	if (this->frameState.currentFrame > 35)
		this->lifetime = 0;
	else if (this->frameState.currentFrame > 20)
		this->renderInfos.color.a = (35 - this->frameState.currentFrame) * 255 / 15;
	this->position.x += this->speed.x;
	if (this->frameState.poseId == this->frameState.sequenceSize - 1) {
		this->position.y += this->speed.y;
		this->speed.y -= 0.75;
	}
}

void SubterraneanAnimism::initializeAction()
{
	this->collisionLimit = 1;
	if (this->frameState.actionId == 858) {
		this->setSequence(min(4, this->parentPlayerB->effectiveSkillLevel[9] - 1));
		this->parentPlayerB->playSFX(57);
		return;
	}

	this->speed.y = min(22.5, 7.5 + (this->gameData.opponent->position.y - this->position.y) / 25);
	this->speed.x = 6 * this->direction;
	this->parentPlayerB->playSFX(3);
}
