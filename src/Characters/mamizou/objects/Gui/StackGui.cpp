//
// Created by PinkySmile on 13/01/25.
//

#include "StackGui.hpp"
#include "../../Mamizou.hpp"

void StackGui::update()
{
	auto mamizou = (Mamizou *)this->parentPlayerB;

	if (mamizou->_transformKind == Mamizou::KIND_TIMER || mamizou->skilledSkillLevel[9] < 0)
		this->renderInfos.color.a = 0;
	else
		this->renderInfos.color.a = SokuLib::v2::InfoManagerBase::instance->unknown494 == 0 ? 255 : 0;
	if (this->frameState.sequenceId == Mamizou::MAX_STACKS + 1) {
		this->gpFloat[0] = mamizou->_transformStackCharge  * 39.f / Mamizou::METER_PER_STACK;
		if (this->gpFloat[0] < 0)
			this->gpFloat[0] = 0;
		if (this->gpFloat[0] > 39)
			this->gpFloat[0] = 39;
		this->position.x = this->parentB->position.x + 3 + (22 * (3 - mamizou->_transformStacks % 4));
		this->position.y = this->parentB->position.y + 3 + 39 - this->gpFloat[0];
	} else if (this->frameState.sequenceId != mamizou->_transformStacks)
		this->setSequence(mamizou->_transformStacks);
	this->advanceFrame();
	if (this->frameState.sequenceId == Mamizou::MAX_STACKS + 1)
		this->setTextureBounds(0, 39 - this->gpFloat[0], 30, this->gpFloat[0]);
}

void StackGui::initializeAction()
{
	this->isGui = true;
	if (!this->parentB)
		this->createChild(this->frameState.actionId, this->position.x + 3, this->position.y - 3, this->direction, this->layer);
	else
		this->setSequence(9);
}
