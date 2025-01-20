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
		this->renderInfos.color.a = 255;
	if (this->frameState.sequenceId != mamizou->_transformStacks)
		this->setSequence(mamizou->_transformStacks);
	this->advanceFrame();
}

void StackGui::initializeAction()
{
	this->isGui = true;
}
