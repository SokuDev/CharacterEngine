//
// Created by PinkySmile on 05/02/25.
//

#include "LanternYoukai.hpp"

#define SPAWN_DELAY 4
#define MAX_VELOCITY 15
#define MAX_TURNAROUND 8
#define spawnTimer gpShort[0]
#define startTimer gpShort[0]
#define velocity gpFloat[0]
#define turnaround gpFloat[1]

void LanternYoukai::update()
{
	float params[4];

	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
	switch (this->frameState.sequenceId) {
	case 0:
	case 1:
		if (this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->frameState.poseId == 5 || this->frameState.poseId == 6) {
			if (this->spawnTimer % SPAWN_DELAY == 0) {
				params[0] = 90 + static_cast<int>(SokuLib::rand(90)) * this->direction;
				params[0] *= -1;
				if (this->frameState.sequenceId == 0)
					params[1] = 0.25f;
				else
					params[1] = 0.125f;
				params[2] = 2;
				params[3] = 60 * (this->frameState.sequenceId + 1);
				this->parentPlayerB->playSFX(this->spawnTimer / SPAWN_DELAY % 3 + 10);
				this->createObject(this->frameState.actionId, this->position.x + 100 * this->direction, this->position.y + 250, 1, 1, params);
			}
			this->spawnTimer++;
		}
		break;
	case 2:
		if (this->frameState.poseId == 0 && this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->position.x < -100 || this->position.x > 1380 || this->position.y < -200 || this->position.y > 1200) {
			this->lifetime = 0;
			return;
		}
		if (this->collisionType != COLLISION_TYPE_NONE) {
			this->lifetime = 0;
			return;
		}
		if (this->startTimer < 20) {
			this->startTimer++;
			this->updateSpeedAngled(this->renderInfos.zRotation, 2 + (20 - this->startTimer) / 5.f);
			this->position += this->speed;
		} else {
			this->trackOpponent(-this->turnaround, this->turnaround, 100);
			this->renderInfos.zRotation = this->customData[0];
			this->updateSpeedAngled(this->renderInfos.zRotation, this->velocity);
			this->position += this->speed;
			this->velocity += this->customData[1];
			if (this->velocity > MAX_VELOCITY)
				this->velocity = MAX_VELOCITY;
			if (this->frameState.currentFrame < this->customData[3]) {
				this->turnaround += this->customData[1];
				if (this->turnaround > MAX_TURNAROUND)
					this->turnaround = MAX_TURNAROUND;
			} else
				this->turnaround = 2;
			if (this->frameState.currentFrame % 4 == 0) {
				this->updateSpeedAngled(this->renderInfos.zRotation, 1);

				SokuLib::Vector2f pos = this->position + this->speed;

				params[0] = this->customData[0] - 90;
				params[2] = 3;
				this->createObject(this->frameState.actionId, pos.x, pos.y, this->direction, -1, params);
			}
		}
		break;
	}
}

void LanternYoukai::initializeAction()
{
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 2) {
		this->velocity = 2;
		this->renderInfos.zRotation = this->customData[0];
		this->collisionLimit = 1;
	}
	if (this->frameState.sequenceId == 3) {
		this->renderInfos.zRotation = this->customData[0];
	}
}
