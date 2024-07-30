//
// Created by PinkySmile on 20/07/24.
//

#include <cmath>
#include "Alice.hpp"

namespace UnexpectedAssistance
{
	void Alice::update()
	{
		float params[3];

		if (this->parentPlayerB->timeStop)
			return;
		if (this->collisionType == COLLISION_TYPE_5) {
			this->lifetime = 0;
			return;
		}
		switch(this->frameState.sequenceId) {
		case 0:
			if (this->speed.y + this->position.y <= this->getGroundHeight()) {
				this->speed.x = 0.0;
				this->speed.y = 0.0;
				this->nextSequence();
				this->position.y = this->getGroundHeight();
				return;
			}
			if (this->position.x >= 1380 || this->position.x <= -100 || this->position.y >= 1400 || this->position.y <= -160) {
				this->lifetime = 0;
				return;
			}
			this->speed.y = this->speed.y - 0.5;
			break;
		case 1:
			if (this->HP < 0) {
				this->setSequence(6);
				this->speed.x = 0.0;
				this->speed.y = 7.5;
				return;
			}
			this->speed.x = 2.0;
			this->speed.y = 0.0;
			if (this->position.y <= this->getGroundHeight()) {
				this->setSequence(0);
				return;
			}
			if (
				(std::pow(this->position.x - this->gameData.opponent->position.x, 2) < 2500 && std::pow(this->position.y - this->gameData.opponent->position.y, 2) < 10000) ||
				this->position.y < this->getGroundHeight() ||
				this->HP < 0
		    	) {
				SokuLib::camera.shake = 10;
				this->renderInfos.shaderType = 1;
				this->setSequence(6);
				this->isActive = false;
				params[0] = 0;
				params[1] = 0;
				params[2] = 2;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params,3);
				this->parentPlayerB->playSFX(9);
				this->position.y = this->getGroundHeight();
				this->speed.y = 12.0;
				this->speed.x = -10.0;
				return;
			}
			if (this->position.x >= 1380 || this->position.x <= -100 || this->position.y >= 1400 || this->position.y <= -160) {
				this->lifetime = 0;
				return;
			}
			break;
		case 2:
			this->checkGrazed(0);
			this->renderInfos.scale.x -= 0.01;
			this->renderInfos.scale.y -= 0.01;
			if (this->renderInfos.color.a < 0xb) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
			this->renderInfos.color.g -= 10;
			this->renderInfos.color.b -= 10;
			break;
		case 3:
			this->renderInfos.scale.x += 0.02;
			this->renderInfos.scale.x += 0.02;
			if (this->renderInfos.color.g > 0x15) {
				this->renderInfos.color.g -= 0x14;
				this->renderInfos.color.b -= 0x14;
			} else {
				if (this->renderInfos.color.a < 0x15) {
					this->lifetime = 0;
					return;
				}
				this->renderInfos.color.g = 0;
				this->renderInfos.color.b = 0;
				this->renderInfos.color.a -= 0x14;
			}
			break;
		case 4:
			this->renderInfos.scale.x -= 0.1;
			this->renderInfos.scale.y -= 0.1;
			if (this->renderInfos.color.b < 0x1e) {
				this->renderInfos.color.g = 0;
				this->renderInfos.color.b = 0;
			} else {
				this->renderInfos.color.g -= 0x1e;
				this->renderInfos.color.b -= 0x1e;
			}
			if (this->renderInfos.color.a < 0x19) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 0x19;
			break;
		case 5:
			if (this->frameState.currentFrame % 2 == 0) {
				params[0] = 0;
				params[1] = 0;
				params[2] = 2;
				this->createObject(this->frameState.actionId, this->position.x, (this->frameState.currentFrame * 0x32) + this->position.y, this->direction, 1, params, 3);
			}
			if (this->frameState.currentFrame == 4) {
				this->lifetime = 0;
				return;
			}
		case 6:
			this->renderInfos.zRotation += 15;
			if (this->renderInfos.color.a < 6) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 5;
			this->speed.y -= 0.5;
			break;
		case 7:
			this->speed.y -= 0.5;
			if (this->speed.y + this->position.y <= this->getGroundHeight() + this->unknown380 && this->speed.y < 0) {
				this->setSequence(10);
				this->position.y = this->getGroundHeight() + this->unknown380;
				this->speed.y = 0.0;
				return;
			}
			if (
				(std::pow(this->position.x - this->gameData.opponent->position.x, 2) <= 10000 &&std::pow(this->position.y - this->gameData.opponent->position.y, 2) <= 10000) ||
				this->HP < 0
			) {
				SokuLib::camera.shake = 10;
				this->renderInfos.shaderType = 1;
				this->setSequence(6);
				this->isActive = false;
				params[0] = 0;
				params[1] = 0;
				params[2] = 2;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
				this->parentPlayerB->playSFX(9);
				this->speed.y = 12.0;
				this->speed.x = -10.0;
			}
			if (this->position.x >= 1380 || this->position.x <= -100 || this->position.y >= 1400 || this->position.y <= -160) {
				this->lifetime = 0;
				return;
			}
			break;
		case 8:
			this->speed.y -= 0.5;
			if (
				this->speed.y + this->position.y <= this->getGroundHeight() + this->unknown380 ||
				(std::pow(this->position.x - this->gameData.opponent->position.x, 2) < 2500 && std::pow(this->position.y - this->gameData.opponent->position.y, 2) < 10000) ||
				this->position.y < this->getGroundHeight() + this->unknown380 ||
				this->HP < 0
			) {
				SokuLib::camera.shake = 10;
				this->renderInfos.shaderType = 1;
				this->setSequence(6);
				this->isActive = false;
				params[0] = 0;
				params[1] = 0;
				params[2] = 2;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
				this->parentPlayerB->playSFX(9);
				this->speed.y = 12.0;
				this->speed.x = -10.0;
			}
			if (1380 >= this->position.x || this->position.x <= -100 || 1000 < this->position.y || this->position.y < -160) {
				this->lifetime = 0;
				return;
			}
			break;
		case 9:
			if (this->HP < 0) {
				this->setSequence(6);
				this->speed.x = 0.0;
				this->speed.y = 7.5;
				return;
			}
			if (this->speed.y + this->position.y <= this->getGroundHeight() + this->unknown380) {
				this->speed.x = 0.0;
				this->speed.y = 0.0;
				this->nextSequence();
				this->position.y = this->getGroundHeight() + this->unknown380;
				return;
			}
			if (this->position.x >= 1380 || this->position.x <= -100 || this->position.y >= 1400 || this->position.y <= -160) {
				this->lifetime = 0;
				return;
			}
			this->speed.y = this->speed.y - 0.5;
			break;
		case 10:
			if (this->HP < 0) {
				this->setSequence(6);
				this->speed.x = 0.0;
				this->speed.y = 7.5;
				return;
			}
			this->speed.x = this->unknown378;
			this->speed.y = 0.0;
			if (this->position.y > this->getGroundHeight() + this->unknown380) {
				this->setSequence(9);
				return;
			}
			if (this->unknown36E == 1) {
				float diff = this->direction * this->gameData.opponent->position.x - this->position.x;

				if (diff <= 200 && diff >= 0 && this->gameData.opponent->position.y <= this->position.y) {
					this->setSequence(8);
					this->createEffect(1, this->position.x, this->position.y, this->direction, 1);
					SokuLib::playSEWaveBuffer(SokuLib::SFX_SLAP_HIT);
					this->speed.x += 3.0;
					this->speed.y = 8.0;
					return;
				}
			}
			if (this->unknown36E == 2) {
				float diff = this->direction * this->gameData.opponent->position.x - this->position.x;

				if (diff <= 200 && diff > 0 && this->position.y > this->gameData.opponent->position.y) {
					this->unknown36C = 1;
					this->setSequence(7);
					this->speed.x = 4.0;
					this->speed.y = 15.0;
					return;
				}
			}
			if (
				(std::pow(this->position.x - this->gameData.opponent->position.x, 2) <= 2500 && std::pow(this->position.y - this->gameData.opponent->position.y, 2) <= 10000) ||
				this->position.y < this->getGroundHeight() + this->unknown380 ||
				this->HP < 0
			) {
				SokuLib::camera.shake = 10.0;
				this->setSequence(6);
				this->renderInfos.shaderType = 1;
				this->isActive = false;
				params[0] = 0;
				params[1] = 0;
				params[2] = 2;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
				this->parentPlayerB->playSFX(9);
				this->position.y = this->getGroundHeight() + this->unknown380;
				this->speed.y = 12.0;
				this->speed.x = -10.0;
			}
			if (this->position.x >= 1380 || this->position.x <= -100 || this->position.y >= 1400 || this->position.y <= -160) {
				this->lifetime = 0;
				return;
			}
			break;
		case 11:
			if (this->frameState.currentFrame < 60) {
				if (this->frameState.currentFrame % 30 == 0)
					this->parentPlayerB->playSFX(10);
				break;
			}
			if (this->frameState.currentFrame >= 240) {
				this->lifetime = 0;
				return;
			}
			if (this->frameState.currentFrame % 2 == 0)
				this->parentPlayerB->playSFX(10);
			this->position.x = SokuLib::rand(1280);
			this->position.y = 1000;
			if (this->position.x < 640)
				this->direction = SokuLib::RIGHT;
			else
				this->direction = SokuLib::LEFT;
			params[0] = SokuLib::rand(50) * 0.1 + 1.0;
			params[1] = SokuLib::rand(20) * 0.1;
			params[2] = 9.0;
			this->parentPlayerB->createObject(this->frameState.actionId, this->position.x, this->position.y + 50.0, this->direction, -1, params, 3);
			break;
		}
		if (this->advanceFrame())
			this->lifetime = 0;
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
	}

	bool Alice::initializeAction()
	{
		float params[3];

		if (!this->customData) {
			this->setSequence(11);
			return true;
		}
		this->collisionLimit = 1;
		this->setSequence(this->customData[2]);
		if (this->frameState.sequenceId == 0 || this->frameState.sequenceId == 9) {
			this->isActive = true;
			this->speed.x = this->customData[0];
			this->speed.y = this->customData[1];
			this->unknown378 = SokuLib::rand(120) * 0.1 + 6.0;
			this->unknown36E = SokuLib::rand(10);
			if (this->frameState.sequenceId == 9)
				this->unknown380 = SokuLib::rand(20) - 10.0;
			this->isActive = true;
			this->unknown138 = this->unknown380 - 6;
		}
		if (this->frameState.sequenceId == 2) {
			params[0] = 0.0;
			params[1] = 0.0;
			params[2] = 3.0;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
			params[2] = 4.0;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
		}
		if (this->frameState.sequenceId == 4)
			this->renderInfos.zRotation = SokuLib::rand(360);
		return true;
	}
}