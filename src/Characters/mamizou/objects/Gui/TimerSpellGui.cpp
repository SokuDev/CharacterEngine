//
// Created by PinkySmile on 13/01/25.
//

#include "TimerSpellGui.hpp"
#include "../../Mamizou.hpp"

void TimerSpellGui::update()
{
	auto mamizou = (Mamizou *)this->parentPlayerB;

	if (mamizou->_transformKind != Mamizou::KIND_SPELL_TIMER) {
		this->renderInfos.color.a = 0;
		return;
	}
	switch (this->frameState.sequenceId) {
	case 0:
		this->renderInfos.shaderType = mamizou->_transformTimer == 0 ? 1 : 0;
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
		this->gpFloat[0] = ((float)mamizou->_transformTimer / Mamizou::SPELL_TIMER_MAX) * 79.0;
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
	case 2:
		break;
	case 3:
		if (SokuLib::v2::InfoManagerBase::instance->unknown494 == 0)
			this->renderInfos.color.a = mamizou->_transformTimer == 0 ? 255 : 0;
		else
			this->renderInfos.color.a = 0;
		this->position.y = 422;
		if (this->parentPlayerB->teamId == 0) {
			this->position.x = 82;
			this->direction = SokuLib::RIGHT;
		} else {
			this->position.x = 558;
			this->direction = SokuLib::LEFT;
		}
		break;
	}
	if (this->advanceFrame())
		this->lifetime = 0;
	if (this->frameState.sequenceId == 1 || this->frameState.sequenceId == 2) {
		this->setTextureBounds(0, 0, this->gpFloat[0], 4);
		return;
	}
}

void TimerSpellGui::initializeAction()
{
	this->isGui = true;
	this->setSequence(this->customData[2]);
}
