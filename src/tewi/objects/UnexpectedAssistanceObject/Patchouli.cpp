//
// Created by PinkySmile on 21/07/24.
//

#include <cmath>
#include "Patchouli.hpp"

#define unknown36C gpShort[0]
#define unknown36E gpShort[1]
#define unknown378 gpFloat[0]
#define unknown37C gpFloat[1]
#define unknown380 gpFloat[2]

namespace UnexpectedAssistance
{
	void Patchouli::update()
	{
		float params[3];
		auto FUN_004b0e40 = (void (__thiscall *)(SokuLib::v2::GameObject *, int, int, int, int, short, short, short))0x4B0E40;

		if (this->parentPlayerB->timeStop)
			return;
		switch(this->frameState.sequenceId) {
		case 0:
			if (this->frameState.currentFrame % 5 == 0) {
				params[0] = SokuLib::rand(360);
				params[1] = 12.0;
				params[2] = 5.0;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
			}
			if (this->frameState.currentFrame % 3 == 0) {
				params[0] = SokuLib::rand(360);
				params[1] = SokuLib::rand(200) + 60.0;
				params[2] = 6.0;
				this->createObject(
					this->frameState.actionId,
					this->direction * std::cos(params[0] * M_PI / 180) * params[1] + this->position.x,
					std::sin(params[0] * M_PI / 180) * params[1] + this->position.y,
					this->direction, 1, params, 3
				);
			}
			this->position.y += 0.5;
			if (this->frameState.currentFrame >= 48) {
				this->parentPlayerB->playSFX(12);
				this->nextSequence();
				this->collisionLimit = 99;
				this->collisionType = COLLISION_TYPE_NONE;
				for (int i = 0; i < 3; i++) {
					params[0] = i * 120.0 + SokuLib::rand(40);
					params[1] = 0.0;
					params[2] = 4.0;
					this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
				};
				for (int i = 0; i < 4; i++) {
					params[0] = i * 90.0 + SokuLib::rand(45) + 160.0;
					params[1] = 1.0;
					params[2] = 4.0;
					this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
				};
			}
			break;
		case 1:
			if (this->unknown36C == 0) {
				this->renderInfos.scale.x = (1.0 - this->unknown378) * 25.0 + 1.0;
				this->renderInfos.scale.y = (1.0 - this->unknown378) * 25.0 + 1.0;
				this->unknown378 *= 0.95;
				FUN_004b0e40(this, this->renderInfos.scale.x * -70, this->renderInfos.scale.x * 70, this->renderInfos.scale.x * 70, this->renderInfos.scale.x * -70, 0, 0, 0);
				this->checkGrazed(0);
				if (this->frameState.currentFrame % 2 == 0) {
					params[0] = SokuLib::rand(360);
					params[1] = SokuLib::rand(40) + 20.0;
					params[2] = 2.0;
					this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
					params[0] = SokuLib::rand(360);
					params[1] = SokuLib::rand(15) + 10.0;
					params[2] = 7.0;
					this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, '\x01', params, 3);
				}
				if (this->frameState.currentFrame % 0xf == 0) {
					params[0] = 0.0;
					params[1] = 0.0;
					params[2] = 3.0;
					this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params, 3);
				}
				if (this->collisionType != COLLISION_TYPE_NONE && ++this->unknown36E > 2) {
					this->unknown36E = 0;
					this->collisionType = COLLISION_TYPE_NONE;
				}
				if (this->renderInfos.color.a < 0xc4) {
					this->renderInfos.color.a = 0xc4;
					this->renderInfos.color.g = 0xc4;
					this->renderInfos.color.b = 0xc4;
				} else {
					this->renderInfos.color.a -= 8;
					this->renderInfos.color.g -= 8;
					this->renderInfos.color.b -= 8;
				}
				if (this->frameState.currentFrame >= 120) {
					this->unknown36C = 1;
				}
			}
			if (this->unknown36C == 1) {
				this->collisionLimit = 0;
				if (this->renderInfos.color.a < 10) {
					this->lifetime = 0;
					return;
				}
				this->renderInfos.color.a -= 10;
				this->renderInfos.color.b -= 10;
				this->renderInfos.color.g -= 10;
			}
			break;
		case 2:
			this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
			this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
			this->renderInfos.scale.x += 0.1;
			this->renderInfos.scale.y *= 0.99;
			if (this->frameState.currentFrame > 14) {
				if (this->renderInfos.color.a < 10) {
					this->lifetime = 0;
					return;
				}
				this->renderInfos.color.a -= 10;
				this->renderInfos.color.b -= 10;
				this->renderInfos.color.g -= 10;
			}
			this->position.x += this->direction * this->speed.x;
			this->position.y += this->speed.y;
			break;
		case 3:
			this->renderInfos.scale.x += 1;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
			this->renderInfos.color.b -= 10;
			this->renderInfos.color.g -= 10;
			break;
		case 4:
			this->renderInfos.zRotation += this->unknown380;
			if (this->unknown36C == 0) {
				this->renderInfos.scale.y = (1- this->unknown37C) * this->unknown378;
				this->unknown37C *= 0.9;
				if (this->parentB != nullptr) {
					this->position.x = this->parentB->position.x;
					this->position.y = this->parentB->position.y;
				} else
					this->unknown36C = 1;
			}
			if (this->unknown36C == 1) {
				this->unknown380 *= 0.98;
				this->renderInfos.scale.y *= 0.85;
				if (this->renderInfos.color.a < 10) {
					this->lifetime = 0;
					return;
				}
				this->renderInfos.color.a -= 10;
				this->renderInfos.color.b -= 10;
				this->renderInfos.color.g -= 10;
			}
			break;
		case 5:
			this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
			this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
			if (0xe < (int)this->frameState.currentFrame) {
				if (this->renderInfos.color.a < 10) {
					this->lifetime = 0;
					return;
				}
				this->renderInfos.color.a -= 10;
				this->renderInfos.color.b -= 10;
				this->renderInfos.color.g -= 10;
			}
			this->position.x += this->direction * this->speed.x;
			this->position.y += this->speed.y;
			break;
		case 6:
			if (this->parentB == nullptr) {
				this->lifetime = 0;
				return;
			}
			this->position.x += this->direction * std::cos(this->customData[0] * M_PI / 180) * this->customData[1];
			this->position.y += std::sin(this->customData[0] * M_PI / 2) * this->customData[1];
			this->customData[1] *= 0.95;
			if (this->unknown36C == 0) {
				if (this->renderInfos.color.a < 236) {
					this->renderInfos.color.a += 20;
					this->renderInfos.color.g += 20;
					this->renderInfos.color.b += 20;
				} else {
					this->renderInfos.color.a = 0xff;
					this->renderInfos.color.g = 0xff;
					this->renderInfos.color.b = 0xff;
				}
				if (0x1d < this->frameState.currentFrame)
					this->unknown36C = 1;
			}
			if (this->unknown36C == 1) {
				if (this->renderInfos.color.a < 5) {
					this->lifetime = 0;
					return;
				}
				this->renderInfos.color.a -= 5;
				this->renderInfos.color.g -= 5;
				this->renderInfos.color.b -= 5;
			}
			break;
		case 7:
			this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
			this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
			this->customData[1] = this->customData[1] * 0.9;
			if (0xe < (int)this->frameState.currentFrame) {
				if (this->renderInfos.color.a < 10) {
					this->lifetime = 0;
					return;
				}
				this->renderInfos.color.a -= 10;
				this->renderInfos.color.b -= 10;
				this->renderInfos.color.g -= 10;
			}
			this->position.x += this->direction * this->speed.x;
			this->position.y += this->speed.y;
			break;
		case 8:
			this->position.y -= 5;
			if (this->position.y <= 0) {
				this->parentPlayerB->playSFX(11);
				this->position.y = 0;
				this->setSequence(0);
				this->unknown378 = 1;
				return;
			}
			break;
		}
		if (this->advanceFrame())
			this->lifetime = 0;
	}

	void Patchouli::initializeAction()
	{
		if (!this->customData) {
			this->setSequence(8);
			this->position.x = 640;
			this->position.y = 800;
			return;
		}
		this->renderInfos.zRotation = this->customData[0];
		this->unknown360 = 1;
		this->setSequence(this->customData[2]);
		if (this->frameState.sequenceId == 4) {
			this->renderInfos.scale.y = 0;
			if (this->customData[1] == 0.0) {
				this->unknown380 = 2;
				this->unknown378 = SokuLib::rand(5) * 0.1 + 0.5;
				this->unknown37C = 1.0;
			}
			if (this->customData[1] == 1.0) {
				this->unknown380 = -4;
				this->unknown378 = SokuLib::rand(2) * 0.1 + 0.2;
				this->unknown37C = 1.0;
			}
		}
		if (this->frameState.sequenceId == 6) {
			this->renderInfos.scale.x = SokuLib::rand(10) * 0.1 + 0.5;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
		}
		if (this->frameState.sequenceId == 7) {
			this->renderInfos.scale.x = SokuLib::rand(125) * 0.01 + 0.75;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
		}
	}
}