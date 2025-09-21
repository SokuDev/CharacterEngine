//
// Created by PinkySmile on 26/10/24.
//

#include <algorithm>
#include "Suika.hpp"

#define generatedSpeed gpShort[0]

void UnexpectedAssistance::Suika::update()
{
	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	if (this->frameState.sequenceId == 1) {
		if (!this->parentB || this->parentB->collisionType != COLLISION_TYPE_NONE) {
			if (this->renderInfos.color.a < 5) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 5;
			if (!this->generatedSpeed) {
				this->generatedSpeed = 1;
				this->speed.x = SokuLib::rand(200) / 10.f - 10;
				this->speed.y = SokuLib::rand(100) / 20.f + 10;
			}
			this->position += this->speed;
			this->speed.y -= 0.5;
			this->renderInfos.zRotation -= 0.5;
		} else {
			this->lifetime = this->parentB->lifetime;
			this->renderInfos.zRotation = -(this->parentB->renderInfos.zRotation + this->customData[0]);
			this->position.x = this->parentB->position.x + SokuLib::cos(this->customData[1] + this->parentB->renderInfos.zRotation) * this->customData[2];
			this->position.y = this->parentB->position.y + SokuLib::sin(this->customData[1] + this->parentB->renderInfos.zRotation) * this->customData[2];
		}
		return;
	}
	if (this->collisionType == COLLISION_TYPE_BULLET_ABSORBED) {
		this->lifetime = 0;
		return;
	}
	this->advanceFrame();
	this->checkGrazed(0);
	this->checkProjectileHit(1);
	if (this->frameState.currentFrame == 90)
		this->parentPlayerB->playSFX(31);
	if (this->frameState.currentFrame == 120) {
		this->customData[0] = fmod(atan2(this->gameData.opponent->position.y + 75 - this->position.y, this->gameData.opponent->position.x - this->position.x) * 180 / M_PI + 360, 360);
		this->customData[1] = 12;
		this->collisionLimit = 1;
	}
	if (this->collisionType != COLLISION_TYPE_NONE) {
		this->parentPlayerB->playSFX(32);
		this->lifetime = 0;
		return;
	}
	this->renderInfos.zRotation += 5;
	if (this->position.x < -500 || this->position.x > 1780 || this->position.y < -700 || this->position.y > 1200) {
		this->lifetime = 0;
		return;
	}
	this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1] * this->direction;
	this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
	this->position.x += this->speed.x;
	this->position.y += this->speed.y;
}

void UnexpectedAssistance::Suika::initializeAction()
{
	if (this->customData) {
		this->renderInfos.scale.x = SokuLib::rand(100) / 100.f + 1;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		return this->setSequence(1);
	}

	this->collisionType = COLLISION_TYPE_NONE;
	this->customData = new float[2];
	this->customData[0] = 0;
	this->customData[1] = 0;
	switch (SokuLib::rand(3)) {
	case 0:
		this->position.x = SokuLib::rand(1280);
		this->position.y = 1100;
		break;
	case 1:
		this->position.x = -400;
		this->position.y = SokuLib::rand(1280);
		break;
	default:
		this->position.x = 1680;
		this->position.y = SokuLib::rand(1100);
		break;
	}

	std::array<std::pair<float, float>, 100> positions;

	this->parentPlayerB->playSFX(30);
	for (auto &pos : positions) {
		float angle = SokuLib::rand(360);

		pos.first = angle;
		pos.second = log(SokuLib::rand(3000) / 1500.f + 1) * 150;
	}
	std::sort(positions.begin(), positions.end(), [](auto &a, auto &b){
		return a.second > b.second;
	});
	for (auto &pos : positions) {
		float params[3] = { static_cast<float>(SokuLib::rand(360)), pos.first, pos.second };

		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
	}
}
