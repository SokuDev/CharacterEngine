//
// Created by PinkySmile on 05/02/25.
//

#include "LanternYoukai.hpp"

void LanternYoukai::update()
{
	if (this->advanceFrame())
		this->lifetime = 0;
	if (this->frameState.sequenceId == 0 || this->frameState.sequenceId == 1) {
		if (this->frameState.poseId == 0)
			this->renderInfos.color.a = this->frameState.poseFrame * 255 / this->frameState.poseDuration;
		else if (this->frameState.poseId == this->frameState.sequenceSize - 1) {
			if (this->renderInfos.color.a <= 10)
				this->lifetime = 0;
			this->renderInfos.color.a -= 10;
		} else
			this->renderInfos.color.a = 255;
		if (this->frameState.poseId == 0 && this->frameState.poseFrame == 0)
			this->lifetime = 0;
		if (this->frameState.poseId == 1 || this->frameState.poseId == 2)
			this->renderInfos.zRotation -= 10;
	} else if (this->frameState.sequenceId == 2 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0)
		this->lifetime = 0;
}

void LanternYoukai::initializeAction()
{
	this->setSequence(this->customData[2]);
	this->renderInfos.zRotation = this->customData[0];
	if (this->frameState.sequenceId == 0 || this->frameState.sequenceId == 1) {
		this->renderInfos.color.a = 0;
	}
}
