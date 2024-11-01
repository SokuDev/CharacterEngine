//
// Created by PinkySmile on 31/10/24.
//

#include "Sanae.hpp"

void UnexpectedAssistance::Sanae::update()
{
	float params[3];

	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	if (this->collisionType == COLLISION_TYPE_5) {
		this->lifetime = 0;
		return;
	}
	switch (this->frameState.sequenceId) {
	case 0:
		this->renderInfos.zRotation += 16.0;
		this->speed.y -= this->gpFloat[0];
		if (this->position.x >= 1380 || this->position.x <= -100.0)  {
			this->lifetime = 0;
			return;
		}
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
		if (this->position.y <= this->getGroundHeight() || this->collisionType != COLLISION_TYPE_NONE) {
			int randomValue = SokuLib::rand(100);

			if (randomValue <= 25) {
				// Very bad luck
				this->parentPlayerB->playSFX(38);
				params[0] = 0.0;
				params[1] = 0.15;
				params[2] = 5;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[2] = 12;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[1] = 30;
				params[2] = 13;
				for (int i = 0; i < 10; i++) {
					params[0] = i * 36.0f;
					this->createObject(this->frameState.actionId, this->position.x,this->position.y,this->direction, (0.0 <= SokuLib::sin(params[0])) * -2 + 1, params);
				}
				params[1] = 0;
				params[2] = 1;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, SokuLib::RIGHT, 1, params);
			} else if (randomValue <= 50) {
				// Bad luck
				this->parentPlayerB->playSFX(39);
				params[0] = 0.0;
				params[1] = 0.15;
				params[2] = 6;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[2] = 12;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[1] = 15;
				params[2] = 13;
				for (int i = 0; i < 8; i++) {
					params[0] = i * 45.0f;
					this->createObject(this->frameState.actionId, this->position.x,this->position.y,this->direction, (0.0 <= SokuLib::sin(params[0])) * -2 + 1, params);
				}
				params[0] = SokuLib::rand(180);
				params[1] = 6;
				params[2] = 14;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[0] += 120.0;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[0] += 120.0;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[1] = 0;
				params[2] = 2;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, SokuLib::RIGHT, 1, params);
			} else if (randomValue <= 75) {
				// Good luck
				this->parentPlayerB->playSFX(40);
				params[0] = 0.0;
				params[1] = 0.15;
				params[2] = 7;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[2] = 9;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[1] = 15;
				params[2] = 10;
				for (int i = 0; i < 8; i++) {
					params[0] = i * 45.0f;
					this->createObject(this->frameState.actionId, this->position.x,this->position.y,this->direction, (0.0 <= SokuLib::sin(params[0])) * -2 + 1, params);
				}
				params[1] = 0;
				params[2] = 3;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, SokuLib::RIGHT, 1, params);
			} else {
				// Very good luck
				this->parentPlayerB->playSFX(41);
				params[0] = 0.0;
				params[1] = 0.15;
				params[2] = 16;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[2] = 9;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[2] = 11;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				params[1] = 30.0;
				params[2] = 10;
				for (int i = 0; i < 10; i++) {
					params[0] = i * 36.0f;
					this->createObject(this->frameState.actionId, this->position.x,this->position.y,this->direction, (0.0 <= SokuLib::sin(params[0])) * -2 + 1, params);
				}
				params[1] = 0.0;
				params[2] = 4;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, SokuLib::RIGHT, 1, params);
			}
			this->lifetime = 0;
			return;
		}
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		this->renderInfos.scale.x += 0.005;
		this->renderInfos.scale.y += 0.005;
		if (this->frameState.currentFrame >= 30) {
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
		}
		this->position.y += 2;
		break;
	case 5:
		if (this->gpShort[0] == 0) {
			this->renderInfos.scale.x += this->customData[1];
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			this->customData[1] /= 2;
			this->checkGrazed(0);
			if (this->collisionType != COLLISION_TYPE_NONE) {
				this->gpShort[1]++;
				if (this->gpShort[1] > 3) {
					this->collisionType = COLLISION_TYPE_NONE;
					this->gpShort[1] = 0;
				}
			}
			if (this->frameState.currentFrame >= 56 || this->collisionLimit == 0) {
				this->gpShort[0] = 1;
				this->collisionLimit = 0;
			}
		} else if (this->gpShort[0] == 1) {
			this->renderInfos.scale.x -= 0.005;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->frameState.currentFrame >= 12)
				this->gpShort[0] = 2;
		} else {
			this->renderInfos.scale.x *= 0.95;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
		}
		break;
	case 6:
		if (this->gpShort[0] == 0) {
			this->renderInfos.scale.x += this->customData[1];
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			this->customData[1] /= 2;
			this->checkGrazed(0);
			if (this->frameState.currentFrame >= 2) {
				this->gpShort[0] = 1;
				this->collisionLimit = 0;
			}
		} else if (this->gpShort[0] == 1) {
			this->renderInfos.scale.x -= 0.005;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->frameState.currentFrame >= 4)
				this->gpShort[0] = 2;
		} else {
			this->renderInfos.scale.x *= 0.95;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
		}
		break;
	case 7:
		if (this->gpShort[0] == 0) {
			this->renderInfos.scale.x += this->customData[1];
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			this->customData[1] /= 2;
			this->checkGrazed(0);
			if (this->frameState.currentFrame >= 6) {
				this->gpShort[0] = 1;
				this->collisionLimit = 0;
			}
		} else if (this->gpShort[0] == 1) {
			this->renderInfos.scale.x -= 0.005;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->frameState.currentFrame >= 12)
				this->gpShort[0] = 2;
		} else {
			this->renderInfos.scale.x *= 0.95;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
			if (this->renderInfos.color.g >= 20) {
				this->renderInfos.color.g -= 20;
				this->renderInfos.color.r -= 20;
			}
		}
		break;
	case 8:
	case 16:
		if (this->gpShort[0] == 0) {
			this->renderInfos.scale.x += this->customData[1];
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			this->customData[1] /= 2;
			this->checkGrazed(0);
			this->checkProjectileHit(0);
			if (this->frameState.currentFrame >= 6) {
				this->gpShort[0] = 1;
				this->collisionLimit = 0;
			}
		} else if (this->gpShort[0] == 1) {
			this->renderInfos.scale.x -= 0.005;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->frameState.currentFrame >= 12)
				this->gpShort[0] = 2;
		} else {
			this->renderInfos.scale.x *= 0.95;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
			if (this->renderInfos.color.g >= 20) {
				this->renderInfos.color.g -= 20;
				this->renderInfos.color.r -= 20;
			}
		}
		break;
	case 9:
	case 12:
		this->renderInfos.scale.x += this->gpFloat[0];
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->gpFloat[0] *= 0.9;
		if (this->renderInfos.color.a < 30) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 30;
		this->renderInfos.color.r -= 30;
		this->renderInfos.color.g -= 30;
		break;
	case 10:
	case 13:
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->customData[1] -= 0.5;
		if (this->customData[1] < 0.5)
			this->customData[1] = 0.5;
		this->renderInfos.zRotation = this->renderInfos.zRotation + this->gpFloat[0];
		this->gpFloat[0] *= 0.95;
		this->gpFloat[1] += 0.1;
		this->position.x += this->direction * this->speed.x;
		this->position.y += this->speed.y * 0.25 + this->gpFloat[1];
		this->renderInfos.scale.x += 0.01;
		this->renderInfos.scale.y += 0.01;
		if (this->renderInfos.color.a < 6) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 6;
		this->renderInfos.color.g -= 6;
		this->renderInfos.color.b -= 6;
		break;
	case 11:
		this->renderInfos.scale.x += this->gpFloat[0];
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->gpFloat[0] = this->gpFloat[0] * 0.85;
		if (0x1d < (int)this->frameState.currentFrame) {
			if (this->renderInfos.color.a < 8) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 8;
		}
		break;
	case 14:
		this->trackOpponent(0.0, 1.0, 100.0);
		if (this->gpShort[0] == 0) {
			this->renderInfos.scale.x += 0.075;
			if (this->renderInfos.scale.x < 1)
				this->renderInfos.scale.y = this->renderInfos.scale.x;
			else {
				this->renderInfos.scale.x = 1.0;
				this->collisionLimit = 1;
				this->gpShort[0] = 1;
				this->renderInfos.scale.y = this->renderInfos.scale.x;
			}
		}
		else {
			if ((int)this->frameState.currentFrame % 3 == 0) {
				params[0] = SokuLib::rand(360);
				params[1] = SokuLib::rand(5) + 5;
				params[2] = 15;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			}
			this->checkGrazed(5);
			if (this->collisionType != COLLISION_TYPE_NONE || this->frameState.currentFrame >= 240) {
				for (int i = 0; i < 8; i++) {
					params[0] = SokuLib::rand(20) + i * 45;
					params[1] = SokuLib::rand(6) + 10;
					params[2] = 15;
					this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				}
				this->lifetime = 0;
				return;
			}
		}
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->position.x += this->direction * this->speed.x;
		this->position.y += this->speed.y;
		if (this->position.x >= 1380.0 || this->position.x <= -100.0 || this->position.y >= 1280.0 || this->position.y <= -250.0) {
			this->lifetime = 0;
			return;
		}
		break;
	case 15:
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->customData[1] *= 0.9;
		if (this->customData[1] < 0.5)
			this->customData[1] = 0.5;
		this->renderInfos.zRotation = this->renderInfos.zRotation + this->gpFloat[0];
		this->gpFloat[0] *= 0.95;
		this->renderInfos.scale.x = this->renderInfos.scale.x + 0.01;
		this->renderInfos.scale.y = this->renderInfos.scale.y + 0.01;
		if (this->renderInfos.color.a < 6) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 6;
		this->position.x += this->direction * this->speed.x;
		this->position.y += this->speed.y;
		if (this->position.x >= 1380.0 || this->position.x <= -100.0 || this->position.y >= 1280.0 || this->position.y <= -250.0) {
			this->lifetime = 0;
			return;
		}
	}
	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
}

void UnexpectedAssistance::Sanae::initializeAction()
{
	if (!this->customData) {
		this->parentPlayerB->playSFX(42);
		this->customData = new float[3];
		this->customData[2] = 0;
		for (int i = 0; i < 24; i++) {
			this->customData[0] = SokuLib::rand(100) / 10.f - 5;
			this->customData[1] = SokuLib::rand(100) / 10.f - 5;
			this->createObject(this->frameState.actionId, SokuLib::rand(1000) + 140, 1000, SokuLib::RIGHT, 1, this->customData, 3);
		}
		this->customData[0] = SokuLib::rand(100) / 10.f - 5;
		this->customData[1] = SokuLib::rand(100) / 10.f - 5;
		this->position.x = SokuLib::rand(1000) + 140;
		this->position.y = 1000;
	}
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 0) {
		this->collisionLimit = 1;
		this->gpFloat[0] = SokuLib::rand(100) / 200.f + 0.25;
		this->speed.x = this->customData[0];
		this->speed.y = this->customData[1];
	}
	if (this->frameState.sequenceId == 5)
		this->collisionLimit = 14;
	if (this->frameState.sequenceId == 7 || this->frameState.sequenceId == 8 || this->frameState.sequenceId == 16)
		this->collisionLimit = 1;
	if (this->frameState.sequenceId == 9 || this->frameState.sequenceId == 12)
		this->gpFloat[0] = 0.5;
	if (this->frameState.sequenceId == 10 || this->frameState.sequenceId == 13) {
		this->renderInfos.zRotation = SokuLib::rand(360);
		this->gpFloat[0] = SokuLib::rand(10) - 5.0;
	}
	if (this->frameState.sequenceId == 11) {
		this->renderInfos.zRotation = SokuLib::rand(360);
		this->gpFloat[0] = 0.2;
	}
	if (this->frameState.sequenceId == 14) {
		this->renderInfos.scale.x = 0.1;
		this->renderInfos.scale.y = 0.1;
	}
}
