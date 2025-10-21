//
// Created by PinkySmile on 12/10/2025.
//

#include "CardSelect.hpp"
#include "../../Mamizou.hpp"

void CardSelect::update()
{
	auto mamizou = (Mamizou *)this->parentPlayerB;

	if (mamizou->frameState.actionId != SokuLib::ACTION_USING_SC_ID_204 && mamizou->frameState.actionId != SokuLib::ACTION_SC_ID_204_ALT_EFFECT) {
		this->lifetime = 0;
		return;
	}
	if (mamizou->_currentSelection >= mamizou->_opponentSkillCards.size()) {
		this->lifetime = 0;
		return;
	}
}

void CardSelect::initializeAction()
{
}

void CardSelect::onRenderEnd()
{
	auto mamizou = (Mamizou *)this->parentPlayerB;

	if (mamizou->_currentSelection >= mamizou->_opponentSkillCards.size())
		return;

	int tex = mamizou->_opponentSkillCards[mamizou->_currentSelection].second[this->customData[0]];

	if (tex) {
		this->sprite.dxHandle = tex;
		this->sprite.render();
	}
}

