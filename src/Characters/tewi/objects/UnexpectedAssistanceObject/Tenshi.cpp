//
// Created by PinkySmile on 30/10/24.
//

#include "Tenshi.hpp"

void UnexpectedAssistance::Tenshi::update()
{
	float params[3];

	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	if (this->frameState.sequenceId == 0) {
		if (this->frameState.currentFrame == 5) {
			SokuLib::weatherCounter = 999;
			if (SokuLib::activeWeather == SokuLib::WEATHER_CLEAR)
				SokuLib::activateWeather(SokuLib::displayedWeather, true);
			this->gameData.opponent->SORDebuffTimer = 900;
		}
		this->renderInfos.zRotation = SokuLib::sin(this->frameState.currentFrame * 8.0) * 20.0;
		if (this->collisionLimit > 0 && this->frameState.currentFrame % 3 == 0) {
			params[0] = this->renderInfos.zRotation;
			params[1] = 0;
			params[2] = 1;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		}

		if (this->collisionLimit > 0) {
			float newAngle = -atan2(this->gameData.opponent->position.y + 100.0 - this->position.y, this->gameData.opponent->position.x - this->position.x) * 180 / M_PI;
			float maxDrift = (300 - this->frameState.currentFrame) / 50.f;

			if (this->customData[0] - newAngle > 180)
				newAngle += 360;
			else if (newAngle - this->customData[0] > 180)
				newAngle -= 360;
			if (this->customData[0] - newAngle < -maxDrift)
				this->customData[0] += maxDrift;
			else if (this->customData[0] - newAngle > maxDrift)
				this->customData[0] -= maxDrift;
			else
				this->customData[0] = newAngle;
			if (this->customData[0] > 180)
				this->customData[0] -= 360;
			if (this->customData[0] < -180)
				this->customData[0] += 360;
		}
		if (this->collisionType != COLLISION_TYPE_NONE) {
			this->gpShort[1]++;
			if (this->gpShort[1] > 2) {
				this->collisionType = COLLISION_TYPE_NONE;
				this->gpShort[1] = 0;
			}
		}
		if (this->position.x < -200 || this->position.x >= 1540 || this->position.y < -200 || this->position.y >= 1200) {
			this->lifetime = 0;
			return;
		}
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->position.x = this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
	}
	if (this->frameState.sequenceId == 1) {
		this->renderInfos.scale.x += 0.02;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (this->renderInfos.color.a < 0xf) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 0xf;
	}
	this->advanceFrame();
}

void UnexpectedAssistance::Tenshi::initializeAction()
{
	if (!this->customData) {
		this->parentPlayerB->playSFX(0);
		this->customData = new float[3];
		this->position.x = SokuLib::rand(100) < 50 ? 0 : 1280;
		this->position.y = SokuLib::rand(1000);
		this->customData[0] = -atan2(this->gameData.opponent->position.y + 100.0 - this->position.y, this->gameData.opponent->position.x - this->position.x) * 180 / M_PI;
		this->customData[1] = 25;
		this->customData[2] = 0;
	}
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 0)
		this->collisionLimit = 5;
	if (this->frameState.sequenceId == 1)
		this->renderInfos.zRotation = this->customData[0];
}
