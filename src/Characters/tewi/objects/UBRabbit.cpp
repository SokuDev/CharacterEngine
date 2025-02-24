//
// Created by PinkySmile on 21/09/24.
//

#include "UBRabbit.hpp"

void UBRabbit::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->frameState.sequenceId != 0) {
		if (this->renderInfos.color.a <= 15) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.zRotation += this->direction * 2;
		this->renderInfos.color.a -= 15;
		this->position.x += this->customData[0];
		this->position.y += this->customData[1];
		this->customData[1] -= 0.5;
		return;
	}
	if (this->frameState.actionId == 854) {
		this->position.x += this->direction;
		this->advanceFrame();
		if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			for (int i = 0; i < 4; i++) {
				float params[] = {
					SokuLib::rand(1000) / 100.f * -this->direction,
					SokuLib::rand(200) / 100.f + 4,
					SokuLib::rand(300) / 100 + 1.f
				};

				this->createObject(this->frameState.actionId, this->position.x - (30 * this->direction), this->position.y, this->direction, 1, params);
			}
		}
		if (this->frameState.poseFrame == 0 && this->frameState.poseId == 9) {
			for (int i = 0; i < 2; i++) {
				float params[] = {
					SokuLib::rand(1000) / 100.f * -this->direction,
					SokuLib::rand(200) / 100.f + 4,
					SokuLib::rand(300) / 100 + 1.f
				};

				this->createObject(this->frameState.actionId, this->position.x - (30 * this->direction), this->position.y - 12, this->direction, 1, params);
			}
		}
		if (this->frameState.currentFrame == 0)
			this->lifetime = 0;
		return;
	}
	this->advanceFrame();
	if (this->frameState.currentFrame > 35)
		this->lifetime = 0;
	else if (this->frameState.currentFrame > 20)
		this->renderInfos.color.a = (35 - this->frameState.currentFrame) * 255 / 15;
	this->position.x += this->speed.x;
	if (this->frameState.poseId == this->frameState.sequenceSize - 1) {
		this->position.y += this->speed.y;
		this->speed.y -= 0.75;
	}
}

void UBRabbit::initializeAction()
{
	if (this->customData) {
		this->setSequence(this->customData[2]);
		return;
	}
	if (this->frameState.actionId == 854)
		return;

	this->speed.y = min(22.5, 7.5 + (this->gameData.opponent->position.y - this->position.y) / 25);
	this->speed.x = 6 * this->direction;
	this->collisionLimit = 1;
}
