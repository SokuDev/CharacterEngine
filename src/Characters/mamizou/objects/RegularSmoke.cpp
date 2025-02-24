//
// Created by PinkySmile on 08/02/25.
//

#include "RegularSmoke.hpp"

void RegularSmoke::update()
{
	this->updateSpeedAngled(this->customData[0], this->customData[1]);
	this->customData[1] -= 0.5;
	if (this->customData[1] < 0.5)
		this->customData[1] = 0.5;
	this->position.x = this->direction * this->speed.x + this->position.x;
	this->position.y = this->speed.y * 0.15 + this->position.y;
	this->renderInfos.zRotation += this->gpFloat[0];
	this->renderInfos.scale.x += 0.02;
	this->renderInfos.scale.y = this->renderInfos.scale.x;
	if ((int)this->frameState.currentFrame >= 0) {
		if (this->renderInfos.color.a < 6) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 5;
	}
	this->advanceFrame();
}

void RegularSmoke::initializeAction()
{
	if (SokuLib::rand(100) < 50)
		this->gpFloat[0] = 1.0;
	else
		this->gpFloat[0] = -1.0;
}
