//
// Created by PinkySmile on 08/05/24.
//

#include <cmath>
#include "Hammer.hpp"

void Hammer::update()
{
	if (this->frameState.sequenceId == 6) {
		this->renderInfos.zRotation = fmod(this->renderInfos.zRotation + 30, 360);
		this->position += this->speed;
		this->speed.x += (this->parentPlayer->position.x - this->position.x) / 1000;
		this->speed.y += (this->parentPlayer->position.y + 75 - this->position.y) / 1000;
	} else if (this->frameState.sequenceId == 0) {
		if (this->collisionLimit && this->checkTurnIntoCrystals(false, 1, 5)) {
			this->collisionLimit = 0;
			this->customData[2] = 0;
			return;
		}
		this->renderInfos.zRotation = fmod(this->renderInfos.zRotation + 30, 360);
		this->position += this->speed;
		this->speed.y -= 1;
	} else
		this->renderInfos.zRotation = 0;
	this->advanceFrame();
	if (this->frameState.sequenceId != 0)
		return;
	if (this->position.x <= 40 && this->speed.x < 0) {
		this->position.x = 40;
		this->speed.x = -this->speed.x * 0.8f;
	}
	if (this->position.x >= 1240 && this->speed.x > 0) {
		this->position.x = 1240;
		this->speed.x = -this->speed.x * 0.8f;
		this->speed.y = this->speed.y * 0.8f;
	}
	if (this->position.y <= 0 && this->speed.y < 0) {
		if (this->customData[2] == 0) {
			this->nextSequence();
			this->speed.x = 0;
			this->speed.y = 0;
			this->position.y = 0;
		} else {
			this->customData[2]--;
			this->position.y = 0;
			this->speed.x = this->speed.x * 0.8f;
			this->speed.y = -this->speed.y * 0.8f;
		}
	}
}

bool Hammer::initializeAction()
{
	this->collisionLimit = 1;
	this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
	this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
	return true;
}
