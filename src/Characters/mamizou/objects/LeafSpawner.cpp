//
// Created by PinkySmile on 14/01/25.
//

#include "LeafSpawner.hpp"

#define rotateSide gpShort[0]
#define velocity gpShort[1]
#define angle gpShort[2]
#define disabled gpShort[3]
#define stepX gpFloat[0]
#define stepY gpFloat[1]

#define LEAF_SPIN_AMOUNT 10
#define LEAF_SPAWN_TIME 15

#define FROG_SPEED_X 5
#define FROG_JUMP_IMPULS 20
#define FROG_GRAVITY 0.75
#define FROG_IDLE_TIME 10

void LeafSpawner::update()
{
	float params[3];

	if (this->parentPlayerB->timeStop)
		return;
	switch (this->frameState.sequenceId) {
	case 0:
	case 3:
		if (this->checkTurnIntoCrystals(false, 0, 5)) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.zRotation = this->renderInfos.zRotation + (this->rotateSide ? LEAF_SPIN_AMOUNT : -LEAF_SPIN_AMOUNT);
		while (this->renderInfos.zRotation > 180)
			this->renderInfos.zRotation -= 360;
		while (this->renderInfos.zRotation < -180)
			this->renderInfos.zRotation += 360;
		this->position.x += this->stepX;
		this->position.y += this->stepY;
		if (this->frameState.currentFrame == LEAF_SPAWN_TIME) {
			this->nextSequence();
			this->parentPlayerB->playSFX(3);
			params[2] = this->frameState.sequenceId + 1;
			this->updateSpeedAngled(this->angle, this->velocity);
			this->collisionLimit = 1;
			this->collisionType = COLLISION_TYPE_NONE;
			this->renderInfos.zRotation = this->angle;
			this->gpShort[2] = 0;
			this->gravity.y = FROG_GRAVITY;
			this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			return;
		}
		break;
	case 1:
		if (this->disabled) {
			if (this->renderInfos.color.a < 20) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 20;
			this->renderInfos.scale.x -= 0.075;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			break;
		}
		this->position.x += this->speed.x * this->direction;
		this->position.y += this->speed.y;
		if (this->checkTurnIntoCrystals(false, 0, 5)) {
			this->lifetime = 0;
			return;
		}
		if (this->checkGrazed(1) || this->checkProjectileHit(2)) {
			this->disabled = true;
			return;
		}
		if (this->collisionType)
			this->disabled = true;
		if (this->position.x < -100)
			this->lifetime = 0;
		if (this->position.x > 1380)
			this->lifetime = 0;
		break;
	case 4:
		this->renderInfos.yRotation += 10;
		while (this->renderInfos.yRotation > 180)
			this->renderInfos.yRotation -= 360;
		while (this->renderInfos.yRotation < -180)
			this->renderInfos.yRotation += 360;
		if (this->disabled) {
			if (this->renderInfos.color.a < 20) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 20;
			this->renderInfos.scale.x -= 0.075;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			break;
		}
		if (this->gpShort[2]) {
			this->gpShort[2]--;
			if (this->gpShort[2] == 0)
				this->speed.y = FROG_JUMP_IMPULS;
		} else {
			this->position.x += FROG_SPEED_X * this->direction;
			this->position.y += this->speed.y;
			this->speed.y -= this->gravity.y;
			if (this->position.y - 20 < this->getGroundHeight())
				this->gpShort[2] = FROG_IDLE_TIME;
		}
		if (this->checkTurnIntoCrystals(false, 2, 5)) {
			this->lifetime = 0;
			return;
		}
		if (this->checkGrazed(10) || this->checkProjectileHit(1)) {
			this->disabled = true;
			return;
		}
		if (this->collisionType)
			this->disabled = true;
		if (this->position.x < -100)
			this->lifetime = 0;
		if (this->position.x > 1380)
			this->lifetime = 0;
		break;
	case 2:
	case 5:
		if (!this->parentB) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.yRotation = this->parentB->renderInfos.yRotation;
		this->renderInfos.zRotation = this->parentB->renderInfos.zRotation;
		this->renderInfos.color.a = this->parentB->renderInfos.color.a;
		this->renderInfos.scale = this->parentB->renderInfos.scale;
		this->position = this->parentB->position;
		break;
	}
	this->advanceFrame();
}

void LeafSpawner::initializeAction()
{
	this->setSequence(this->customData[2]);
	switch (this->frameState.sequenceId) {
	case 0:
	case 3:
		this->rotateSide = SokuLib::rand(100) >= 50;
		this->angle = this->customData[0];
		this->velocity = this->customData[1];
		this->stepX = (this->customData[3] - this->position.x) / LEAF_SPAWN_TIME;
		this->stepY = (this->customData[4] - this->position.y) / LEAF_SPAWN_TIME;
		break;
	}
}
