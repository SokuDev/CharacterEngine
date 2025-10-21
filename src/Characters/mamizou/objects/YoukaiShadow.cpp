//
// Created by PinkySmile on 08/01/25.
//

#include "YoukaiShadow.hpp"

#define disabled gpShort[0]

void YoukaiShadow::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (
		this->parentPlayerB->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
		this->parentPlayerB->frameState.actionId < SokuLib::ACTION_FORWARD_DASH
	)
		this->disabled = 1;
	if (this->parentPlayerB->timeStop)
		return;
	if (this->parentPlayerB->hitStop && !this->disabled)
		return;
	if (this->advanceFrame() || this->frameState.currentFrame == 0) {
		this->lifetime = 0;
		return;
	}
	if (this->disabled) {
		if (this->renderInfos.color.a > 30)
			this->renderInfos.color.a -= 30;
		else
			this->lifetime = 0;
	}
}

void YoukaiShadow::initializeAction()
{
	this->disabled = 0;
	switch (this->parentPlayerB->frameState.actionId) {
	case SokuLib::ACTION_3A:
		this->setSequence(0);
		if (this->parentPlayerB->frameState.sequenceId == 1) {
			this->renderInfos.scale.x = 2;
			this->renderInfos.scale.y = 2;
		}
		break;
	case SokuLib::ACTION_5AAAA:
		this->setSequence(0);
		this->renderInfos.scale.x = 2;
		this->renderInfos.scale.y = 2;
		break;
	}
}
