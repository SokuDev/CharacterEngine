//
// Created by PinkySmile on 09/02/25.
//

#include "HotPot.hpp"

#define MIN_BORDER_SPACING 200
#define WALL_X 200
#define HITBOX_X 225
#define RING_SIZE 50
#define HITBOX_Y 250
#define HITBOX_INNER_HIT 200
#define HITBOX_INNER 150

#define OWNER_STARTED 1
#define OPPONENT_STARTED 2
#define OWNER_HIT 4
#define OPPONENT_HIT 8

#define startedFlag gpShort[0]
#define animationCounter gpShort[1]
#define disappear gpShort[2]
#define smokeTimer gpShort[3]

void HotPot::update()
{
	float params[3];
	SokuLib::Vector2f piecesSpeed[] = {
		{8, -30},
		{24, -10},
		{34, -2},
		{-32, -32},
		{28, 10},
		{20, 20},
		{10, 32},
		{24, -12},
		{-24, -24},
		{-34, 2},
	};

	switch (this->frameState.sequenceId) {
	case 0:
	case 1:
	case 2:
		if (this->frameState.sequenceId == 0)
			for (int i = 0; i < RING_SIZE * this->renderInfos.scale.x; i++) {
				int j = (int)(this->position.x + WALL_X * this->renderInfos.scale.x - i);
				int k = (int)(this->position.x - WALL_X * this->renderInfos.scale.x + i);

				if (j >= 0 && j < 1280)
					SokuLib::v2::groundHeight[j] = 0;
				if (k >= 0 && k < 1280)
					SokuLib::v2::groundHeight[k] = 0;
			}
		if (this->position.x - MIN_BORDER_SPACING * this->renderInfos.scale.x < 40)
			this->position.x = 40 + MIN_BORDER_SPACING * this->renderInfos.scale.x;
		if (this->position.x + MIN_BORDER_SPACING * this->renderInfos.scale.x > 1240)
			this->position.x = 1240 - MIN_BORDER_SPACING * this->renderInfos.scale.x;
		if (this->frameState.currentFrame < 30) {
			this->renderInfos.scale.x = 0.1 + 0.9 * this->frameState.currentFrame / 30;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
		}
		if (this->frameState.currentFrame == 30) {
			this->renderInfos.scale.x = 1;
			this->renderInfos.scale.y = 1;
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = 1;
		}
		if (this->position.y > this->getGroundHeight())
			this->position.y -= this->speed.y;
		if (this->position.y <= this->getGroundHeight()) {
			this->position.y = this->getGroundHeight();
			this->resetForces();
		}
		if (this->frameState.sequenceId != 0) {
			this->lifetime = this->parentObject->lifetime;
			if (this->frameState.sequenceId == 1)
				this->renderInfos = this->parentObject->renderInfos;
			this->disappear = this->parentObject->disappear;
			if (!this->disappear)
				break;
		}
		if (this->disappear) {
			this->renderInfos.scale.x -= 0.05;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->renderInfos.scale.x <= 0.05) {
				this->renderInfos.scale.x = 0;
				this->lifetime = 0;
			}
		}
		if (this->frameState.sequenceId == 0)
			for (int i = 0; i < RING_SIZE * this->renderInfos.scale.x; i++) {
				int j = (int)(this->position.x + WALL_X * this->renderInfos.scale.x - i);
				int k = (int)(this->position.x - WALL_X * this->renderInfos.scale.x + i);

				if (j >= 0 && j < 1280)
					SokuLib::v2::groundHeight[j] = HITBOX_Y * this->renderInfos.scale.y;
				if (k >= 0 && k < 1280)
					SokuLib::v2::groundHeight[k] = HITBOX_Y * this->renderInfos.scale.y;
			}
		if (this->disappear)
			break;
		this->smokeTimer += this->animationCounter / 10 + 1;
		while (this->smokeTimer >= 16) {
			params[0] = SokuLib::rand(400) / 100 - 2;
			params[1] = SokuLib::rand(400) / 100 + 0.2;
			params[2] = 14;
			this->createObject(this->frameState.actionId, this->position.x + SokuLib::rand(200) - 100, this->position.y + 300, this->direction, -1, params);
			this->smokeTimer -= 16;
		}

		if (this->startedFlag & OWNER_HIT) {
			this->parentPlayerB->advanceFrame();
			this->parentPlayerB->grabInvulTimer = 1;
			this->parentPlayerB->projectileInvulTimer = 1;
			this->parentPlayerB->meleeInvulTimer = 1;
			if (!(this->startedFlag & OWNER_STARTED)) {
				this->parentPlayerB->untech = 2;
				this->parentPlayerB->hitStop = 2;
				this->parentPlayerB->position.x += this->position.x;
				this->parentPlayerB->position.x /= 2;
				this->parentPlayerB->position.y += this->parentPlayerB->speed.y;
				if (this->parentPlayerB->gravity.y <= 0)
					this->parentPlayerB->gravity.y = 1;
				this->parentPlayerB->speed.y -= this->parentPlayerB->gravity.y;
				if (this->position.y + 100 > this->parentPlayerB->position.y) {
					this->startedFlag |= OWNER_STARTED;
					this->parentPlayerB->position.x = this->position.x - this->direction * 50;
					this->parentPlayerB->position.y = this->position.y + 275;
					this->parentPlayerB->speed.x = 0;
					this->parentPlayerB->speed.y = 0;
					this->parentPlayerB->gravity.y = 0;
					this->parentPlayerB->gpShort[0] = 200;
					this->parentPlayerB->setActionSequence(SokuLib::ACTION_USING_SC_ID_200, 4);
					this->parentPlayerB->renderInfos.color.a = 0;
					this->parentPlayerB->gpShort[0] = 0;
				}
			} else {
				this->parentPlayerB->hp += 4;
				if (this->parentPlayerB->hp > this->parentPlayerB->maxHP)
					this->parentPlayerB->hp = this->parentPlayerB->maxHP;
				this->parentPlayerB->position.x = this->position.x - this->direction * 50;
				this->parentPlayerB->position.y = this->position.y + 275;
			}
		}
		if (this->startedFlag & OPPONENT_HIT) {
			this->gameData.opponent->untech = 10;
			this->gameData.opponent->hitStop = 10;
			this->gameData.opponent->advanceFrame();
			this->gameData.opponent->grabInvulTimer = 1;
			this->gameData.opponent->projectileInvulTimer = 1;
			this->gameData.opponent->meleeInvulTimer = 1;
			if (!(this->startedFlag & OPPONENT_STARTED)) {
				this->gameData.opponent->position.x += this->position.x;
				this->gameData.opponent->position.x /= 2;
				this->gameData.opponent->position.y += this->gameData.opponent->speed.y;
				if (this->gameData.opponent->gravity.y <= 0)
					this->gameData.opponent->gravity.y = 1;
				this->gameData.opponent->speed.y -= this->gameData.opponent->gravity.y;
				if (this->position.y + 100 > this->gameData.opponent->position.y) {
					this->startedFlag |= OPPONENT_STARTED;
					this->gameData.opponent->position.x = this->position.x;
					this->gameData.opponent->position.y = this->position.y + 100;
					this->gameData.opponent->setAction(106);
					this->gameData.opponent->renderInfos.yRotation = 90;
				}
			} else {
				this->gameData.opponent->hitStop = 10;
				this->animationCounter++;
				this->damageOpponent(4 * this->parentPlayerB->spellDmgMultiplier, 1000, 1, false);
				if (this->renderInfos.color.b)
					this->renderInfos.color.b -= 3;
				else if (this->renderInfos.color.g)
					this->renderInfos.color.g -= 3;
				this->gameData.opponent->position.x = this->position.x;
				this->gameData.opponent->position.y = this->position.y + 100;
				if (this->animationCounter == 240) {
					params[0] = 0;
					params[1] = 0;
					if (this->startedFlag & OWNER_HIT) {
						this->parentPlayerB->renderInfos.yRotation = 0;
						this->parentPlayerB->position.y -= 100;
						this->parentPlayerB->setAction(72);
						this->parentPlayerB->untech = 400;
						this->parentPlayerB->grabInvulTimer = 0;
						this->parentPlayerB->projectileInvulTimer = 0;
						this->parentPlayerB->meleeInvulTimer = 0;
						params[2] = 15;
					} else
						params[2] = 3;
					this->gameData.opponent->renderInfos.yRotation = 0;
					this->gameData.opponent->setAction(72);
					this->gameData.opponent->untech = 400;
					this->gameData.opponent->grabInvulTimer = 0;
					this->gameData.opponent->projectileInvulTimer = 0;
					this->gameData.opponent->meleeInvulTimer = 0;
					if (this->startedFlag & OWNER_STARTED)
						this->parentPlayerB->nextSequence();
					this->lifetime = 0;
					for (int i = 0; i < RING_SIZE * this->renderInfos.scale.x; i++) {
						SokuLib::v2::groundHeight[(int)(this->position.x + WALL_X * this->renderInfos.scale.x - i)] = 0;
						SokuLib::v2::groundHeight[(int)(this->position.x - WALL_X * this->renderInfos.scale.x + i)] = 0;
					}
					this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
					for (int i = 4; i < 14; i++) {
						params[2] = i;
						this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
					}
					for (int i = 0; i < 100; i++) {
						params[0] = SokuLib::rand(1500) / 100 - 7.5;
						params[1] = SokuLib::rand(1500) / 100 - 7.5;
						params[2] = 14;
						this->createObject(this->frameState.actionId, this->position.x + SokuLib::rand(200) - 100, this->position.y + SokuLib::rand(300), this->direction, -1, params);
					}
				}
			}
		} else if (this->frameState.currentFrame == 720) {
			this->disappear = true;
			if (this->startedFlag & OWNER_STARTED)
				this->parentPlayerB->nextSequence();
		} else {
			if (
				this->gameData.opponent->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
				this->gameData.opponent->frameState.actionId < SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN &&
				!this->gameData.opponent->hitStop &&
				!this->parentPlayerB->timeStop
			) {
				if (std::abs(this->position.x - this->gameData.opponent->position.x) < 2)
					this->gameData.opponent->position.x = this->position.x;
				else if (this->position.x > this->gameData.opponent->position.x)
					this->gameData.opponent->position.x += 2;
				else
					this->gameData.opponent->position.x -= 2;
			}
			this->_handleCollisions(this->gameData.opponent, this->frameState.currentFrame >= 30);
		}
		if (!(this->startedFlag & OWNER_HIT))
			this->_handleCollisions(this->parentPlayerB, this->frameState.currentFrame >= 30);
		break;
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		this->speed.y -= 0.6;
		this->position.x += piecesSpeed[this->frameState.sequenceId - 4].x * this->direction;
		this->position.y += piecesSpeed[this->frameState.sequenceId - 4].y + this->speed.y;
		if (this->renderInfos.color.a <= 5)
			this->lifetime = 0;
		else
			this->renderInfos.color.a -= 5;
		break;
	case 14:
		this->position.x += this->customData[0];
		this->position.y += this->customData[1];
		if (this->disappear) {
			if (this->renderInfos.color.a <= 3)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 3;
		} else if (SokuLib::rand(100) < 20)
			this->disappear = true;
		break;
	}
	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
	if (this->frameState.poseId == 0 && this->frameState.poseFrame == 0)
		this->lifetime = 0;
}

void HotPot::initializeAction()
{
	float params[3];

	if (this->customData == nullptr) {
		params[0] = 0;
		params[1] = 0;
		params[2] = 1;
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params);
		params[2] = 2;
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params);
	} else
		this->setSequence(this->customData[2]);
	switch (this->frameState.sequenceId) {
	case 0:
	case 1:
	case 2:
		this->renderInfos.scale.x = 0.1;
		this->renderInfos.scale.y = 0.1;
		break;
	case 3:
	case 15:
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		break;
	case 14:
		this->renderInfos.scale.x = SokuLib::rand(500) / 1000.f + 0.5;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		break;
	}
}

void HotPot::_handleCollisions(SokuLib::v2::Player *player, bool canHit)
{
	auto diff = std::abs(player->position.x - this->position.x);
	bool isHit = canHit;

	if (diff > HITBOX_X * this->renderInfos.scale.x)
		return;
	if (diff > (HITBOX_X - RING_SIZE) * this->renderInfos.scale.x) {
		if (player->position.y > this->position.y + HITBOX_Y * this->renderInfos.scale.y)
			return;
		isHit = false;
	} else if (player->position.y > this->position.y + HITBOX_INNER * this->renderInfos.scale.y)
		return;
	if (isHit && player->position.y > this->position.y + HITBOX_INNER_HIT * this->renderInfos.scale.y)
		isHit = false;
	if (isHit) {
		if (player->grabInvulTimer)
			return;
		if (player->damageLimited)
			return;
		if (this->parentPlayerB == player) {
			this->parentPlayerB->playSFX(7);
			this->startedFlag |= OWNER_HIT;
		} else {
			this->damageOpponent(0, 1000, 1, false);
			this->parentPlayerB->playSFX(7);
			if (player->frameState.actionId < SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN || player->frameState.actionId >= SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN)
				player->setAction(SokuLib::ACTION_FALLING);
			this->startedFlag |= OPPONENT_HIT;
			this->parentPlayerB->playSFX(8);
		}
		return;
	}
	if (!canHit) {
		if (player->position.x < this->position.x)
			player->position.x += this->position.x - HITBOX_X * this->renderInfos.scale.x;
		else
			player->position.x += this->position.x + HITBOX_X * this->renderInfos.scale.x;
		player->position.x /= 2;
	}
}
