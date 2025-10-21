//
// Created by PinkySmile on 13/01/25.
//

#include "TimerDebuffGui.hpp"
#include "../../Mamizou.hpp"

void TimerDebuffGui::update()
{
	auto mamizou = (Mamizou *)this->parentPlayerB;

	if (mamizou->_transformKind != Mamizou::KIND_DEBUFF_TIMER) {
		this->renderInfos.color.a = 0;
		return;
	}
	switch (this->frameState.sequenceId) {
	case 0:
		this->position.y = 375;
		if (this->parentPlayerB->teamId == 0) {
			this->position.x = 57;
			this->direction = SokuLib::RIGHT;
		} else {
			this->position.x = 583;
			this->direction = SokuLib::LEFT;
		}
		this->renderInfos.color.a = SokuLib::v2::InfoManagerBase::instance->unknown494 == 0 ? 255 : 0;
		break;
	case 1:
		this->gpFloat[0] = ((float)mamizou->_transformTimer / Mamizou::DEBUFF_TIMER_MAX) * 79.0;
		if (this->gpFloat[0] < 0)
			this->gpFloat[0] = 0;
		if (this->gpFloat[0] > 79)
			this->gpFloat[0] = 79;
		this->position.y = 422;
		if (this->parentPlayerB->teamId == 0) {
			this->position.x = 82;
			this->direction = SokuLib::RIGHT;
		} else {
			this->position.x = 558;
			this->direction = SokuLib::LEFT;
		}
		this->renderInfos.color.a = SokuLib::v2::InfoManagerBase::instance->unknown494 == 0 ? 255 : 0;
		break;
	}
	if (this->advanceFrame())
		this->lifetime = 0;
	if (this->frameState.sequenceId == 1 || this->frameState.sequenceId == 2) {
		this->setTextureBounds(0, 0, this->gpFloat[0], 4);
		return;
	}
}

void TimerDebuffGui::initializeAction()
{
	this->isGui = true;
	this->setSequence(this->customData[2]);
}
