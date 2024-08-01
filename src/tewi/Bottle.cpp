//
// Created by PinkySmile on 28/07/24.
//

#include <cmath>
#include "Bottle.hpp"

#define timer(t) (*(unsigned *)&t->data[0])
#define timer2(t) (*(unsigned *)&t->data[4])

void Bottle::update()
{
	this->advanceFrame();
	if (this->frameState.sequenceId == 0) {
		if ((this->collisionType != COLLISION_TYPE_NONE || (
			this->position.y <= this->getGroundHeight() ||
			this->position.x <= 40 ||
			this->position.x >= 1240
		)) && this->speed.x != 0 && this->frameState.currentFrame > 5) {
			if (this->customData[4] != 0) {
				float params[3] = {this->customData[0], 0, 3};

				this->createObject(807, 256, -50, SokuLib::RIGHT, -1, params);
				this->createObject(807, 768, -50, SokuLib::RIGHT, -1, params);
				this->createObject(807, 1280, -50, SokuLib::RIGHT, -1, params);
			}
			this->nextSequence();
			this->parentPlayerB->playSFX(2);
			this->parentPlayerB->playSFX(13);
			this->position.y = this->getGroundHeight();
		}
		this->checkGrazed(0);
		this->checkProjectileHit(0);
		if (this->checkTurnIntoCrystals(false, 0, 2))
			this->lifetime = 0;
		this->renderInfos.zRotation += 10;
		this->position += this->speed;
		this->speed.y -= this->gravity.y;
	} else if (this->frameState.sequenceId == 1) {
		float distance;

		this->position.y = this->getGroundHeight();
		if (this->renderInfos.color.a < 35)
			this->renderInfos.color.a = 0;
		else
			this->renderInfos.color.a -= 35;
		if (this->collisionType == COLLISION_TYPE_HIT || this->collisionType == COLLISION_TYPE_BLOCKED) {
			this->position = this->gameData.opponent->position;
			this->position.y += 25;
		}
		if (this->customData[4] == 0) {
			if (this->collisionType == COLLISION_TYPE_HIT)
				distance = 0;
			else if (this->collisionType == COLLISION_TYPE_BLOCKED)
				distance = 50;
			else {
				auto diff = this->position - this->gameData.opponent->position;

				distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
			}
		} else {
			if (this->collisionType == COLLISION_TYPE_HIT)
				distance = 0;
			else if (this->collisionType == COLLISION_TYPE_BLOCKED)
				distance = 50;
			else {
				auto diff = this->position - this->gameData.opponent->position;

				diff.x /= 10;
				diff.y /= 2;
				distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
			}
		}
		if (distance <= 100) {
			timer(this) += 100 - distance;
			timer2(this) += 100 - distance;
			this->gameData.opponent->renderInfos.color.g = distance * 255 / 100;
			this->gameData.opponent->renderInfos.color.b = distance * 255 / 100;
			while (timer(this) >= this->customData[1]) {
				this->gameData.opponent->currentSpirit--;
				this->gameData.opponent->spiritRegenDelay = 60;
				timer(this) -= this->customData[1];
			}
			while (timer2(this) >= this->customData[3]) {
				this->gameData.opponent->HP--;
				timer2(this) -= this->customData[3];
			}
			if (this->gameData.opponent->HP <= 0)
				this->gameData.opponent->HP = 1;
		} else {
			this->gameData.opponent->renderInfos.color.g = 255;
			this->gameData.opponent->renderInfos.color.b = 255;
		}
		if (this->frameState.currentFrame >= this->customData[0]) {
			this->gameData.opponent->renderInfos.color.g = 255;
			this->gameData.opponent->renderInfos.color.b = 255;
			this->lifetime = 0;
		}

		if (this->customData[4] == 0) {
			float params[] = {
				2.5f - SokuLib::rand(50) / 10.f,
				2.5f + SokuLib::rand(50) / 10.f,
				2, 2
			};

			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, SokuLib::rand() % 2 == 0 ? 1 : -1, params);
		} else {
			float params[] = {
				2.5f - SokuLib::rand(50) / 10.f,
				2.5f + SokuLib::rand(50) / 10.f,
				2, 2
			};

			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, SokuLib::rand() % 2 == 0 ? 1 : -1, params);
			params[0] = 5.f - SokuLib::rand(75) / 10.f;
			params[1] = 5.f + SokuLib::rand(75) / 10.f;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, SokuLib::rand() % 2 == 0 ? 1 : -1, params);
		}
		if (this->frameState.currentFrame % 30 == 0)
			this->parentPlayerB->playSFX(13);
	} else if (this->frameState.sequenceId == 2) {
		this->position.x += this->customData[0];
		this->position.y += this->customData[1];
		this->customData[0] = this->customData[0] * 499 / 500;
		this->customData[1] = this->customData[1] * 499 / 500;
		if (this->renderInfos.color.a <= this->customData[3]) {
			this->renderInfos.color.a = 0;
			this->lifetime = 0;
		} else
			this->renderInfos.color.a -= this->customData[3];
	} else if (this->frameState.sequenceId == 3) {
		if (this->frameState.currentFrame >= this->customData[0]) {
			if (this->renderInfos.color.a <= 10)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 10;
		} else if (this->renderInfos.color.a >= 245)
			this->renderInfos.color.a = 255;
		else
			this->renderInfos.color.a += 10;

		if (SokuLib::rand(500 - this->renderInfos.color.a) < 50) {
			float params[] = {
				1.f - SokuLib::rand(75) / 10.f,
				1.f + SokuLib::rand(75) / 10.f,
				2, SokuLib::rand(10) + 1.f
			};

			this->createObject(this->frameState.actionId, SokuLib::rand(1200) + 40, 0, this->direction, SokuLib::rand() % 2 == 0 ? 1 : -1, params);
		}
	}
}

bool Bottle::initializeAction()
{
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 3)
		this->renderInfos.color.a = 0;
	return true;
}
