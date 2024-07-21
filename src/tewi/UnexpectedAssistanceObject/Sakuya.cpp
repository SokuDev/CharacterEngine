//
// Created by PinkySmile on 19/07/24.
//

#include <cmath>
#include "Sakuya.hpp"

namespace UnexpectedAssistance
{
	void Sakuya::_spawnActualBullet(unsigned id)
	{
		float params[3];

		this->parentPlayerB->playSFX(6);
		this->direction = SokuLib::RIGHT;
		params[0] = std::atan2(
			this->gameData.opponent->position.y + 100 - this->position.y,
			this->direction * this->gameData.opponent->position.x - this->position.x
		) * 180 / M_PI;
		params[1] = 80.0;
		params[2] = id;
		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
		this->lifetime = 0;
	}

	bool Sakuya::_handleBorderBounce(float angle)
	{
		float params[3];

		if (this->frameState.currentFrame >= 60 && this->customData[3] != 0.0) {
			this->_spawnActualBullet(8);
			return true;
		}
		if (this->frameState.currentFrame >= 150) {
			this->_spawnActualBullet(4);
			return true;
		}
		params[0] = 0.0;
		params[1] = 0.0;
		params[2] = 2.0;
		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
		this->parentPlayerB->playSFX(7);
		this->customData[0] = (SokuLib::rand(120) - 60.0) + angle;
		this->renderInfos.zRotation = -this->customData[0];
		this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
		this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
		return false;
	}

	void Sakuya::update()
	{
		float params[3];

		if (this->parentPlayerB->timeStop)
			return;
		if (this->collisionType == COLLISION_TYPE_5)
			this->lifetime = 0;
		if (this->frameState.sequenceId == 0 && this->parentPlayerB->HP > 0) {
			if (this->direction * this->speed.x < 0 && this->position.x <= SokuLib::camera.leftEdge && this->_handleBorderBounce((this->direction == SokuLib::LEFT) * 180))
				return;
			if (this->direction * this->speed.x > 0 && this->position.x >= SokuLib::camera.rightEdge && this->_handleBorderBounce((this->direction == SokuLib::RIGHT) * 180))
				return;
			if (this->speed.y > 0 && this->position.y >= SokuLib::camera.topEdge && this->_handleBorderBounce(-90))
				return;
			if (this->speed.y < 0 && this->position.y <= 0 && this->_handleBorderBounce(90))
				return;
			if (this->position.x > 1580 || this->position.x < -300) {
				this->lifetime = 0;
				return;
			}
			if (this->position.y > 1260 || this->position.y < -300) {
				this->lifetime = 0;
				return;
			}
		}
		if (this->frameState.sequenceId == 1) {
			if (this->parentB == nullptr) {
				this->lifetime = 0;
				return;
			}
			this->direction = this->parentB->direction;
			this->position.x = this->direction * this->parentB->speed.x + this->parentB->position.x;
			this->position.y = this->parentB->speed.y + this->parentB->position.y;
			this->renderInfos.zRotation = this->parentB->renderInfos.zRotation;
			if (0 < this->parentB->frameState.poseId) {
				this->lifetime = 0;
				return;
			}
		}
		if (this->frameState.sequenceId == 2) {
			this->renderInfos.scale.x -= 0.01;
			this->renderInfos.scale.y -= 0.01;
			if (this->renderInfos.color.a < 0x15) { 
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 0x14;
		}
		if (this->frameState.sequenceId == 3 || this->frameState.sequenceId == 6) {
			this->renderInfos.scale.x += this->unknown378;
			this->renderInfos.scale.y += this->unknown378;
			this->unknown378 -= 0.1;
			if (this->unknown378 < 0.005)
				this->unknown378 = 0.005;
			if (this->renderInfos.color.a < 6) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 5;
		}
		if (this->frameState.sequenceId == 4 || this->frameState.sequenceId == 8) {
			this->checkGrazed(0);
			if (this->collisionType != 0) {
				this->unknown36C = 5;
				this->unknown36E++;
				if (2 < this->unknown36E) {
					this->unknown36E = 0;
					this->collisionType = COLLISION_TYPE_NONE;
				}
			}
			if (this->unknown36C == 0) {
				this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
				this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
			} else {
				this->unknown36C--;
				this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1] * 0.02;
				this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1] * 0.02;
			}
			params[0] = this->renderInfos.zRotation;
			params[1] = 0;
			params[2] = 7.0;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
			if (this->position.x > 1680 || this->position.x < -300) {
				this->lifetime = 0;
				return;
			}
			if (this->position.y > 1200 || this->position.y < -300) {
				this->lifetime = 0;
				return;
			}
		}
		if (this->frameState.sequenceId == 5) {
			this->renderInfos.scale.x *= 0.99;
			this->renderInfos.scale.y *= 0.99;
			if (this->renderInfos.color.a < 7) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 6;
		}
		if (this->frameState.sequenceId == 7) {
			this->renderInfos.scale.x += this->unknown378;
			this->renderInfos.scale.y += this->unknown378;
			this->unknown378 -= 0.1;
			if (this->unknown378 < 0.005)
				this->unknown378 = 0.005;
			if (this->renderInfos.color.a < 11) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
		}
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
		if (!this->advanceFrame())
			return;
		this->lifetime = 0;
	}

	bool Sakuya::initializeAction()
	{
		float params[3];

		if (!this->customData) {
			this->customData = SokuLib::New<float>(4);
			this->customData[0] = 2;
			this->customData[1] = 150;
			this->customData[2] = 0;
			this->customData[3] = 0;
			this->direction = SokuLib::rand() % 2 ? SokuLib::LEFT : SokuLib::RIGHT;
			this->position.x = SokuLib::rand(SokuLib::camera.rightEdge - SokuLib::camera.leftEdge) + SokuLib::camera.leftEdge;
			this->position.y = SokuLib::rand(SokuLib::camera.topEdge);
			this->parentPlayerB->playSFX(8);
		}
		if (this->customData[2] > 0)
			this->setSequence(this->customData[2]);
		if (this->frameState.sequenceId == 0) {
			this->setTail(990, 15.0, 20, 1, 2);
			this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
			this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
			this->renderInfos.zRotation = -std::atan2(this->speed.y, this->speed.x) * 180 / M_PI;
		}
		if (this->frameState.sequenceId == 2) {
			params[0] = 0.0;
			params[1] = 0.0;
			params[2] = 3.0;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
		}
		if (this->frameState.sequenceId == 3)
			this->unknown378 = 0.4;
		if (this->frameState.sequenceId == 4 || this->frameState.sequenceId == 8) {
			if (this->frameState.sequenceId == 8)
				this->collisionLimit = 8;
			else
				this->collisionLimit = 10;
			this->setTail(990, 15.0, 20, 1, 2);
			params[0] = this->customData[0];
			params[1] = 0.0;
			params[2] = 1.0;
			this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params, 3);
			params[2] = 5.0;
			this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params, 3);
			params[2] = 6.0;
			this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params, 3);
			this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
			this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
			this->renderInfos.zRotation = -this->customData[0];
		}
		if (this->frameState.sequenceId == 5) {
			this->renderInfos.scale.x = 2.0;
			this->renderInfos.scale.y = 2.0;
		}
		if (this->frameState.sequenceId == 6) {
			this->renderInfos.scale.x = 2.0;
			this->renderInfos.scale.y = 2.0;
			this->unknown378 = 0.4;
		}
		if (this->frameState.sequenceId == 7)
			this->unknown378 = 0.4;
		return true;
	}
}