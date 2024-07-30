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
		this->position.y = this->getGroundHeight();
		if (this->renderInfos.color.a < 10)
			this->renderInfos.color.a = 0;
		else
			this->renderInfos.color.a -= 10;
		if (this->collisionType == COLLISION_TYPE_HIT || this->collisionType == COLLISION_TYPE_BLOCKED)
			this->position = this->gameData.opponent->position;

		float distance;

		if (this->collisionType == COLLISION_TYPE_HIT)
			distance = 0;
		else if (this->collisionType == COLLISION_TYPE_BLOCKED)
			distance = 50;
		else {
			auto diff = this->position - this->gameData.opponent->position;

			distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
		}
		if (distance <= 100) {
			timer(this) += 100 - distance;
			timer2(this) += 100 - distance;
			this->gameData.opponent->renderInfos.color.g = distance * 255 / 100;
			this->gameData.opponent->renderInfos.color.b = distance * 255 / 100;
			if (timer(this) >= 75) {
				this->gameData.opponent->currentSpirit--;
				this->gameData.opponent->spiritRegenDelay = 60;
				timer(this) -= 75;
			}
			while (timer2(this) >= 40) {
				this->gameData.opponent->HP--;
				timer2(this) -= 40;
			}
			if (this->gameData.opponent->HP <= 0)
				this->gameData.opponent->HP = 1;
		} else {
			this->gameData.opponent->renderInfos.color.g = 255;
			this->gameData.opponent->renderInfos.color.b = 255;
		}
		if (this->frameState.currentFrame >= 300) {
			this->gameData.opponent->renderInfos.color.g = 255;
			this->gameData.opponent->renderInfos.color.b = 255;
			this->lifetime = 0;
		}

		float params[2] = {
			2.5f - SokuLib::rand(50) / 10.f,
			2.5f + SokuLib::rand(50) / 10.f
		};

		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, SokuLib::rand() % 2 == 0 ? 1 : -1, params, 2);
		if (this->frameState.currentFrame % 30 == 0)
			this->parentPlayerB->playSFX(13);
	} else if (this->frameState.sequenceId == 2) {
		this->position.x += this->customData[0];
		this->position.y += this->customData[1];
		this->customData[0] = this->customData[0] * 499 / 500;
		this->customData[1] = this->customData[1] * 499 / 500;
		if (this->renderInfos.color.a < 2) {
			this->renderInfos.color.a = 0;
			this->lifetime = 0;
		} else
			this->renderInfos.color.a -= 2;
	}
}

bool Bottle::initializeAction()
{
	if (this->customData)
		this->setSequence(2);
	return true;
}
