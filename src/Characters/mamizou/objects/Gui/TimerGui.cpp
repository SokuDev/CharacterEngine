//
// Created by PinkySmile on 13/01/25.
//

#include "TimerGui.hpp"
#include "../../Mamizou.hpp"

void TimerGui::update()
{
	auto mamizou = (Mamizou *)this->parentPlayerB;

	if (mamizou->_transformKind != Mamizou::KIND_TIMER && (
		mamizou->_transformKind == Mamizou::KIND_DEBUFF_TIMER ||
		mamizou->_transformKind == Mamizou::KIND_SPELL_TIMER ||
		mamizou->_transformKind == Mamizou::KIND_STACK ||
		mamizou->skilledSkillLevel[1] < 0
	)) {
		this->renderInfos.color.a = 0;
		return;
	}
	switch (this->frameState.sequenceId) {
	case 0:
		this->renderInfos.shaderType = mamizou->_transformedCooldown ? 1 : 0;
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
		this->gpFloat[0] = ((float)mamizou->_transformTimer / Mamizou::TIMER_MAX) * 79.0;
		if (this->gpFloat[0] < 0)
			this->gpFloat[0] = 0;
		if (this->gpFloat[0] > 79)
			this->gpFloat[0] = 79;
		if (mamizou->_transformedCooldown) {
			this->setSequence(2);
			break;
		}
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
		this->gpFloat[0] = (((float)Mamizou::TIMER_COOLDOWN_MAX - mamizou->_transformTimer) / Mamizou::TIMER_COOLDOWN_MAX) * 79.0;
		if (this->gpFloat[0] < 0)
			this->gpFloat[0] = 0;
		if (this->gpFloat[0] > 79)
			this->gpFloat[0] = 79;
		if (!mamizou->_transformedCooldown) {
			this->setSequence(1);
			break;
		}
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
	case 3:
		if (SokuLib::v2::InfoManagerBase::instance->unknown494 == 0)
			this->renderInfos.color.a = mamizou->_transformedCooldown ? 255 : 0;
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

void TimerGui::initializeAction()
{
	this->isGui = true;
	this->setSequence(this->customData[2]);
}
