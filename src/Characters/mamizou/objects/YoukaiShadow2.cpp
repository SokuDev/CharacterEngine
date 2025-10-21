//
// Created by PinkySmile on 08/01/25.
//

#include "YoukaiShadow2.hpp"

#define disabled gpShort[0]

void YoukaiShadow2::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->parentPlayerB->frameState.actionId != this->customData[0])
		this->lifetime = 0;
	if (this->parentPlayerB->timeStop || this->parentPlayerB->hitStop)
		return;
	this->position = this->parentPlayer->position + this->parentPlayer->speed;
	if (this->advanceFrame() || this->frameState.currentFrame == 0) {
		this->lifetime = 0;
		return;
	}
	if (this->parentPlayerB->frameState.poseId == this->customData[1])
		this->disabled = 1;
	if (this->disabled) {
		if (this->renderInfos.color.a > 30)
			this->renderInfos.color.a -= 30;
		else
			this->lifetime = 0;
	}
}

void YoukaiShadow2::initializeAction()
{
	this->disabled = 0;
	this->setSequence(this->customData[2]);
}
