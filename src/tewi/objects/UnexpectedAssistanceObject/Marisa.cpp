//
// Created by PinkySmile on 18/07/24.
//

#include <cmath>
#include "Marisa.hpp"

namespace UnexpectedAssistance
{
	void Marisa::update()
	{
		if (this->parentPlayerB->timeStop)
			return;
		if (this->collisionType == COLLISION_TYPE_5) {
			this->lifetime = 0;
			return;
		}
		switch (this->frameState.sequenceId) {
		case 0:
			if (this->parentPlayerB->HP < 1 || this->gameData.opponent->HP < 1)
				this->unknown36C = 1;
			this->renderInfos.zRotation += 8.0;
			if (this->position.y < this->getGroundHeight()) {
				if (this->speed.x > 0) {
					this->speed.x -= 0.1;
					if (this->speed.x < 0)
						this->speed.x = 0;
				}
				if (this->speed.x < 0) {
					this->speed.x += 0.1;
					if (this->speed.x > 0)
						this->speed.x = 0;
				}
			} else
				this->speed.y -= 0.35;
			if (this->speed.y < 0.0) {
				if (this->position.y < this->getGroundHeight()) {
					this->parentPlayerB->playSFX(1);
					if (this->speed.y < -2.0) {
						this->speed.x *= 0.85;
						this->speed.y *= -0.6;
					} else
						this->speed.y = 0.0;
				}
			}
			if (this->position.x > 1280 && this->speed.x > 0) {
				this->parentPlayerB->playSFX(1);
				this->speed.x *= -0.8;
			}
			if (this->position.x < 0 && this->speed.x < 0) {
				this->parentPlayerB->playSFX(1);
				this->speed.x *= -0.8;
			}
			if (this->frameState.currentFrame >= 120) {
				if (this->renderInfos.shaderType == 0) {
					this->renderInfos.shaderType = 3;
				}
				if (this->frameState.currentFrame >= 184)
					this->renderInfos.shaderColor.r = 0xff;
				else
					this->renderInfos.shaderColor.r = (this->frameState.currentFrame - 120) * 4;
				this->renderInfos.shaderColor.g = this->renderInfos.shaderColor.r;
				this->renderInfos.shaderColor.b = this->renderInfos.shaderColor.r;
			}
			if (this->frameState.currentFrame >= 180) {
				if (this->position.y <= this->getGroundHeight()) {
					float params[3] = {0, 0, 4};
	
					this->speed.x = 0;
					this->speed.y = 0;
					this->createEffect(0x7F, this->position.x, this->position.y,  this->direction, -1);
					this->createEffect(0x7F, this->position.x, this->position.y, -this->direction, -1);
					params[2] = 1;
					this->parentPlayerB->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
					params[2] = 3;
					this->parentPlayerB->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
					SokuLib::camera.shake = 20;
					this->parentPlayerB->playSFX(5);
					params[2] = 2;
					for (int i = 0; i < 20; i++) {
						params[0] = i * 18.f + (SokuLib::rand() % 0x1e);
						params[1] = 15 + (SokuLib::rand() % 100) * 0.1f;
						this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, (0 < sin(-params[0])) * -2 + 1, params, 3);
					};
					this->lifetime = this->lifetime + -1;
					return;
				}
			}
			this->position.x += this->direction * this->speed.x;
			this->position.y +=  this->speed.y;
			break;
		case 1:
			this->checkGrazed(0);
			this->checkProjectileHit(0);
			break;
		case 2:
			this->speed.x = std::cos(this->customData[0] * M_PI / 360) * this->customData[1];
			this->speed.y = std::sin(this->customData[0] * M_PI / 360) * this->customData[1];
			this->customData[1] -= 0.5;
			if (this->customData[1] < 0.5)
				this->customData[1] = 0.5;
			this->position.x += this->direction * this->speed.x;
			this->position.y += this->speed.y * 0.25;
			this->renderInfos.scale.x += 0.01;
			this->renderInfos.scale.y += 0.01;
			if (this->renderInfos.color.a < 3)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 3;
			break;
		case 3:
			this->renderInfos.scale.x += 0.5;
			this->renderInfos.scale.y += 0.5;
			if (this->renderInfos.color.a < 15) {
				this->lifetime = 0;
				break;
			} else
				this->renderInfos.color.a -= 15;
			this->renderInfos.color.g -= 15;
			this->renderInfos.color.r -= 15;
		}
		if (this->advanceFrame()) {
			this->lifetime = 0;
			return;
		}
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 2) {
			this->lifetime = 0;
			return;
		}
		if (this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 5) {
			this->lifetime = 0;
			return;
		}

		if (this->frameState.sequenceId != 1 || this->frameState.poseFrame != 0 || this->frameState.poseId != 0xd)
			return;

		float params[3] = {0, 0, 4};

		this->parentPlayerB->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params);
	}

	void Marisa::initializeAction()
	{
		if (!this->customData) {
			this->parentPlayerB->playSFX(0);
			this->position.x = SokuLib::rand(1280);
			this->position.y = 1280;
			this->speed.x = (SokuLib::rand(10) - 5.f);
			this->speed.x += std::copysign(5, this->speed.x);
			this->isActive = true;
			this->unknown138 = -6.0;
		} else {
			this->setSequence(this->customData[2]);
			if (this->customData[2] == 1)
				this->collisionLimit = 1;
			if (this->customData[2] != 2)
				return;
			this->renderInfos.scale.x = (SokuLib::rand() % 100) * 0.01 + 1.0;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			this->renderInfos.zRotation = (SokuLib::rand() % 360);
		}
	}
}