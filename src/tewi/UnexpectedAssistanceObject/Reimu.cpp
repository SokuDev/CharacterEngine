//
// Created by PinkySmile on 17/07/24.
//

#include <cmath>
#include "Reimu.hpp"

namespace UnexpectedAssistance
{
	void Reimu::update()
	{
		if (this->parentPlayerB->timeStop)
			return;

		auto old = this->frameState.sequenceId;

		if (this->advanceFrame()) {
			this->lifetime = 0;
			return;
		}
		if (this->unknown195) {
			this->unknown195--;
			if (this->unknown195 == 0)
				this->collisionType = COLLISION_TYPE_NONE;
		} else if (this->collisionType != COLLISION_TYPE_NONE)
			this->unknown195 = 2;
		if (this->frameState.sequenceId == 0) {
			this->position += this->speed;
			this->speed.y -= this->gravity.y;
			this->renderInfos.zRotation = std::fmod(this->renderInfos.zRotation + 60, 360);
			if (this->position.y <= this->getGroundHeight()) {
				if (this->collisionLimit == 0) {
					this->nextSequence();
					this->resetRenderInfo();
					this->collisionLimit = 120;
					this->unknown195 = 0;
				} else {
					this->parentPlayerB->playSFX(1);
					this->position.y = this->getGroundHeight();
					this->speed.y *= -0.6;
					this->collisionLimit--;
				}
			}
		} else if (this->frameState.sequenceId == 2) {
			if (old == 1)
				this->parentPlayerB->playSFX(4);
			if (this->frameState.currentFrame == 120) {
				this->nextSequence();
				this->collisionType = COLLISION_TYPE_NONE;
			}
		} else if (this->frameState.sequenceId == 3) {
			auto f = std::cos(this->frameState.currentFrame * 3.0 * M_PI / 180);

			this->renderInfos.scale.x += f * 0.05;
			this->renderInfos.scale.y += f * 0.05;
			if (this->renderInfos.color.a <= 10)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 10;
		}
	}

	bool Reimu::initializeAction()
	{
		unsigned total = 0;
		float temp = 0;
		float base = 1280;

		for (int i = 0; i < 4; i++) {
			while (base > 0 || temp > 0) {
				base += temp;
				temp--;
				total++;
			}
			base = 0;
			temp *= -0.6;
		}
		this->parentPlayerB->playSFX(0);
		this->position.x = SokuLib::rand(1280);
		this->position.y = 1280;
		this->speed.x = (this->gameData.opponent->position.x - this->position.x) / total;
		this->gravity.y = 1;
		this->collisionLimit = 3;
		return true;
	}
}