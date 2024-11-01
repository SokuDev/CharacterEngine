//
// Created by PinkySmile on 08/05/24.
//

#include <complex>
#include <map>
#include <dinput.h>
#include "Tewi.hpp"
#include "TewiObjectFactory.hpp"
#include "GameObjectList.hpp"

#ifndef _DEBUG
#define printf(...)
#endif

#define EFFECT_COUNT 16

const std::map<unsigned short, unsigned short> systemToHammer{
	{ SokuLib::ACTION_IDLE,                                  Tewi::ACTION_IDLE_NO_HAMMER },
	{ SokuLib::ACTION_CROUCHING,                             Tewi::ACTION_CROUCHING_NO_HAMMER },
	{ SokuLib::ACTION_CROUCHED,                              Tewi::ACTION_CROUCHED_NO_HAMMER },
	{ SokuLib::ACTION_STANDING_UP,                           Tewi::ACTION_STANDING_UP_NO_HAMMER },
	{ SokuLib::ACTION_WALK_FORWARD,                          Tewi::ACTION_WALK_FORWARD_NO_HAMMER },
	{ SokuLib::ACTION_WALK_BACKWARD,                         Tewi::ACTION_WALK_BACKWARD_NO_HAMMER },
	{ SokuLib::ACTION_NEUTRAL_JUMP,                          Tewi::ACTION_NEUTRAL_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_FORWARD_JUMP,                          Tewi::ACTION_FORWARD_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_BACKWARD_JUMP,                         Tewi::ACTION_BACKWARD_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_FALLING,                               Tewi::ACTION_FALLING_NO_HAMMER },
	{ SokuLib::ACTION_LANDING,                               Tewi::ACTION_LANDING_NO_HAMMER },
	{ SokuLib::ACTION_NEUTRAL_HIGH_JUMP,                     Tewi::ACTION_NEUTRAL_HIGH_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_FORWARD_HIGH_JUMP,                     Tewi::ACTION_FORWARD_HIGH_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_BACKWARD_HIGH_JUMP,                    Tewi::ACTION_BACKWARD_HIGH_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH,    Tewi::ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER },
	{ SokuLib::ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH,    Tewi::ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER },
	{ SokuLib::ACTION_BE2,                                   Tewi::ACTION_BE2_NO_HAMMER },
	{ SokuLib::ACTION_BE1,                                   Tewi::ACTION_BE1_NO_HAMMER },
	{ SokuLib::ACTION_BE6,                                   Tewi::ACTION_BE6_NO_HAMMER },
	{ SokuLib::ACTION_BE4,                                   Tewi::ACTION_BE4_NO_HAMMER },
};
const std::map<unsigned short, unsigned short> hammerToSystem{
	{ Tewi::ACTION_IDLE_NO_HAMMER,                                  SokuLib::ACTION_IDLE },
	{ Tewi::ACTION_CROUCHING_NO_HAMMER,                             SokuLib::ACTION_CROUCHING },
	{ Tewi::ACTION_CROUCHED_NO_HAMMER,                              SokuLib::ACTION_CROUCHED },
	{ Tewi::ACTION_STANDING_UP_NO_HAMMER,                           SokuLib::ACTION_STANDING_UP },
	{ Tewi::ACTION_WALK_FORWARD_NO_HAMMER,                          SokuLib::ACTION_WALK_FORWARD },
	{ Tewi::ACTION_WALK_BACKWARD_NO_HAMMER,                         SokuLib::ACTION_WALK_BACKWARD },
	{ Tewi::ACTION_NEUTRAL_JUMP_NO_HAMMER,                          SokuLib::ACTION_NEUTRAL_JUMP },
	{ Tewi::ACTION_FORWARD_JUMP_NO_HAMMER,                          SokuLib::ACTION_FORWARD_JUMP },
	{ Tewi::ACTION_BACKWARD_JUMP_NO_HAMMER,                         SokuLib::ACTION_BACKWARD_JUMP },
	{ Tewi::ACTION_FALLING_NO_HAMMER,                               SokuLib::ACTION_FALLING },
	{ Tewi::ACTION_LANDING_NO_HAMMER,                               SokuLib::ACTION_LANDING },
	{ Tewi::ACTION_NEUTRAL_HIGH_JUMP_NO_HAMMER,                     SokuLib::ACTION_NEUTRAL_HIGH_JUMP },
	{ Tewi::ACTION_FORWARD_HIGH_JUMP_NO_HAMMER,                     SokuLib::ACTION_FORWARD_HIGH_JUMP },
	{ Tewi::ACTION_BACKWARD_HIGH_JUMP_NO_HAMMER,                    SokuLib::ACTION_BACKWARD_HIGH_JUMP },
	{ Tewi::ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER,    SokuLib::ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH },
	{ Tewi::ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER,    SokuLib::ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH },
	{ Tewi::ACTION_BE2_NO_HAMMER,                                   SokuLib::ACTION_BE2 },
	{ Tewi::ACTION_BE1_NO_HAMMER,                                   SokuLib::ACTION_BE1 },
	{ Tewi::ACTION_BE6_NO_HAMMER,                                   SokuLib::ACTION_BE6 },
	{ Tewi::ACTION_BE4_NO_HAMMER,                                   SokuLib::ACTION_BE4 },
};
const std::map<unsigned short, unsigned short> moveToHammer{
	{ SokuLib::ACTION_5B,                      Tewi::ACTION_5B_HAMMER  },
	{ SokuLib::ACTION_j5B,                     Tewi::ACTION_j5B_HAMMER },
	{ SokuLib::ACTION_6A,                      Tewi::ACTION_6A_HAMMER  },
	{ SokuLib::ACTION_j6A,                     Tewi::ACTION_j6A_HAMMER },
	{ SokuLib::ACTION_2A,                      Tewi::ACTION_2A_HAMMER  },
	{ SokuLib::ACTION_j2A,                     Tewi::ACTION_j2A_HAMMER },
	{ SokuLib::ACTION_3A,                      Tewi::ACTION_3A_HAMMER  },
	{ SokuLib::ACTION_j8A,                     Tewi::ACTION_j8A_HAMMER },
	{ SokuLib::ACTION_f5A,                     Tewi::ACTION_f5A_HAMMER },
	{ SokuLib::ACTION_j5A,                     Tewi::ACTION_j5A_HAMMER },
	{ SokuLib::ACTION_HANGEKI,                 Tewi::ACTION_HANGEKI_HAMMER },
	{ SokuLib::ACTION_LEFT_HANDED_FOLDING_FAN, Tewi::ACTION_LEFT_HANDED_FOLDING_FAN_HAMMER },
	{ SokuLib::ACTION_SPELL_BREAKING_DRUG,     Tewi::ACTION_SPELL_BREAKING_DRUG_HAMMER },
};
int __practiceEffect = -1;
SokuLib::DrawUtils::Sprite leaf;
SokuLib::DrawUtils::Sprite hammer;
SokuLib::DrawUtils::Sprite faces[21];
bool loaded = false;

Tewi::Tewi(SokuLib::PlayerInfo &info) :
	SokuLib::v2::Player(info)
{
	if (!loaded) {
		SokuLib::loadPalette("data/character/tewi/palette000.pal");
		hammer.texture.loadFromGame("data/character/tewi/hammerBullet000.png");
		hammer.rect.width = hammer.texture.getSize().x;
		hammer.rect.height = hammer.texture.getSize().y;
		hammer.setSize(hammer.texture.getSize());
		hammer.setRotation(-M_PI_4);
		hammer.setCamera(&SokuLib::camera);

		leaf.texture.loadFromGame("data/character/tewi/faces/leaf.png");
		leaf.rect.width = leaf.texture.getSize().x;
		leaf.rect.height = leaf.texture.getSize().y;
		leaf.setSize({15, 15});
		leaf.setPosition({25, 105});

		for (int i = 0; i < std::size(faces); i++) {
			auto &face = faces[i];

			face.texture.loadFromGame(("data/character/tewi/faces/" + std::string(i == SokuLib::CHARACTER_RANDOM ? "namazu" : SokuLib::getCharName(i)) + ".png").c_str());
			face.rect.width = face.texture.getSize().x;
			face.rect.height = face.texture.getSize().y;
			face.setSize(face.texture.getSize());
			face.setPosition({5, 80});
		}
		loaded = true;
	}
	this->objectList = new GameObjectList<TewiObjectFactory>(this);
	if (info.isRight == 0)
		__practiceEffect = -1;
}

bool Tewi::_checkDashSlide()
{
	if (!this->groundDashCount) {
		this->resetForces();
		return this->advanceFrame();
	}
	if (this->speed.x > 0) {
		this->speed.x -= 0.5;
		if (this->speed.x <= 0) {
			this->groundDashCount = 0;
			this->resetForces();
		}
	} else {
		this->speed.x += 0.5;
		if (this->speed.x >= 0) {
			this->groundDashCount = 0;
			this->resetForces();
		}
	}
	return this->advanceFrame();
}

void Tewi::_jumpUpdate(float xSpeed)
{
	if (this->frameState.sequenceId == 0) {
		this->applyGroundMechanics();
		if (!this->groundDashCount)
			this->resetForces();
	} else {
		this->speed.y -= this->gravity.y;
		if (this->applyAirMechanics()) {
			this->setAction(SokuLib::ACTION_LANDING);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			return;
		}
	}
	this->advanceFrame();
	if (this->frameState.sequenceId == 1 && this->frameState.poseId == 0) {
		this->updateGroundMovement(xSpeed);
		this->speed.y = this->_hammer ? JUMP_SPEED_Y : HAMMER_JUMP_SPEED_Y;
		this->gravity = {0, this->_hammer ? JUMP_GRAVITY : HAMMER_JUMP_GRAVITY};
		this->groundDashCount = 0;
	}
}

void Tewi::_highJumpUpdate(float xSpeed, float ySpeed, float gravity)
{
	if (this->frameState.sequenceId == 0)
		this->applyGroundMechanics();
	if (this->applyAirMechanics()) {
		this->setAction(SokuLib::ACTION_LANDING);
		this->position.y = this->getGroundHeight();
		this->resetForces();
		return;
	}
	if (this->isGrounded() || this->speed.y > 0)
		this->grazeTimer = 2;
	if (this->frameState.sequenceId > 0) {
		this->speed.y -= this->gravity.y;
		if (this->speed.y < -20.0)
			this->speed.y = -20.0;
	}
	if (this->frameState.sequenceId == 1 && this->speed.y < 4.0)
		this->setSequence(2);
	this->advanceFrame();
	if (this->frameState.currentFrame != 0 || this->frameState.poseFrame != 0 || this->frameState.poseId != 0 || this->frameState.sequenceId != 1)
		return;
	this->updateGroundMovement(xSpeed);
	this->speed.y = ySpeed;
	this->gravity.y = gravity;
	this->groundDashCount = 0;
	this->createEffect(0x3F, this->position.x, this->position.y, this->direction, 1);
}

void Tewi::_bSeriesUpdate(float angle, float speed, float slowDown, float yOffset, bool isAirborne)
{
	if (isAirborne) {
		if (this->frameState.sequenceId == 2) {
			this->applyGroundMechanics();
			if (this->advanceFrame())
				this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		this->advanceFrame();
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId != 0) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		if (this->applyAirMechanics()) {
			this->setSequence(2);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			return;
		}
	} else {
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
	}
	if (this->frameState.sequenceId < 1 && this->inputData.keyInput.b == 0)
		this->chargedAttack = false;
	if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
		if (this->frameState.poseId == 4 && this->chargedAttack) {
			this->nextSequence();
			this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
		}
		if (this->frameState.poseId >= 6 && this->frameState.poseId < 11) {
			short id = 801 + 5 * (this->_hammer != nullptr);
			float carrotParams[3] = {angle + SokuLib::rand(40) - 20.f, speed, slowDown};
			auto obj = this->createObject(id, (this->direction * 40) + this->position.x, this->position.y + yOffset, this->direction, 1, carrotParams);

			if (this->frameState.poseId == 6) {
				this->consumeSpirit(200, 45);
				this->addCardMeter(20);
			}
			this->collisionType = COLLISION_TYPE_HIT;
			this->playSFX(1);
			obj->collisionLimit = 1;
		}
	}

	if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0) {
		if (this->frameState.poseId >= 2 && this->frameState.poseId < 7) {
			float carrotParams[6] = {
				angle + 20.f - (this->frameState.poseId - 2) * 10, speed, slowDown,
				angle - 20.f + (this->frameState.poseId - 2) * 10, speed, slowDown
			};
			short id = 801 + 5 * (this->_hammer != nullptr);
			auto obj1 = this->createObject(id, (this->direction * 40) + this->position.x, this->position.y + yOffset, this->direction, 1, &carrotParams[0], 3);
			auto obj2 = this->createObject(id, (this->direction * 40) + this->position.x, this->position.y + yOffset, this->direction, 1, &carrotParams[3], 3);

			if (this->frameState.poseId == 2) {
				this->consumeSpirit(200, 45);
				this->addCardMeter(40);
			}
			this->collisionType = COLLISION_TYPE_HIT;
			this->playSFX(1);
			obj1->collisionLimit = 1;
			obj2->collisionLimit = 1;
		}
	}
}

void Tewi::update()
{
	this->spinRotationCenter.x = 0;
	this->spinRotationCenter.y = 57;
	if (this->_revive && this->frameState.actionId == SokuLib::ACTION_KNOCKED_DOWN_STATIC) {
		if (this->_hammer)
			this->setAction(SokuLib::ACTION_USING_SC_ID_211);
		else
			this->setAction(ACTION_USING_SC_ID_211_HAMMER);
		this->_revive = false;
	}
	if (
		SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN <= this->frameState.actionId && this->frameState.actionId <= SokuLib::ACTION_AIR_GUARD &&
		!this->blockObjectSpawned
	) {
		float yOffset;

		this->blockObjectSpawned = true;
		if (this->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_HIGH_HUGE_BLOCKSTUN)
			yOffset = 74;
		else if (this->frameState.actionId == SokuLib::ACTION_AIR_GUARD)
			yOffset = 120;
		else //if (SokuLib::ACTION_RIGHTBLOCK_LOW_SMALL_BLOCKSTUN <= this->frameState.actionId && this->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_LOW_HUGE_BLOCKSTUN)
			yOffset = 40;
		this->createObject(998, (float)(this->direction * 57) + this->position.x, this->position.y + yOffset, this->direction, 1);
	}
	if (this->hitStop)
		return;
	if (this->gameData.opponent->timeStop)
		return;
	if ((
		this->frameState.actionId == ACTION_USING_SC_ID_206_HAMMER ||
		this->frameState.actionId == SokuLib::ACTION_USING_SC_ID_206
	) && this->frameState.sequenceId == 1)
		this->_rabbitAnimation = false;
	if (this->_rabbitAnimation) {
		this->projectileInvulTimer = 10;
		this->meleeInvulTimer = 10;
		this->grabInvulTimer = 10;
		this->collisionLimit = 0;
		this->advanceFrame();
		this->speed.y -= this->gravity.y;
		if (this->isOnGround())
			this->position.y = this->getGroundHeight();
		if ((
			this->frameState.actionId == ACTION_USING_SC_ID_206_HAMMER ||
			this->frameState.actionId == SokuLib::ACTION_USING_SC_ID_206
		) && this->frameState.sequenceId == 1)
			this->setAction(SokuLib::ACTION_IDLE);
		this->unknown7D8 += 15;
		this->renderInfos.color.a = 255 - this->unknown7D8;
		if (this->unknown7D8 >= 255) {
			this->renderInfos.xRotation = 0;
			this->renderInfos.yRotation = 0;
			this->renderInfos.zRotation = 0;
			this->setActionSequence(this->_hammer == nullptr ? ACTION_USING_SC_ID_206_HAMMER : SokuLib::ACTION_USING_SC_ID_206, 1);
			this->_rabbitAnimation = false;
			this->speed = {0, 0};
			this->gravity.y = 0;
			this->position.y = this->getGroundHeight();
			SokuLib::camera.forceScale = true;
			SokuLib::camera.forcedScale = 1;
			SokuLib::camera.forceXCenter = true;
			SokuLib::camera.forcedXCenter = this->gameData.opponent->position.x;
			SokuLib::camera.forceYCenter = true;
			SokuLib::camera.forcedYCenter = 50;
			this->unknown7D8 = 0;
		}
		return;
	}

	const SokuLib::Vector2f bottleOffsets[10] = {
		{50, 117},
		{50, 117},
		{47, 110},
		{40, 100},
		{-10, 80},
		{-30, 65},
		{-30, 60},
		{-30, 40},
		{20, 40},
		{60, 120}
	};

	if (SokuLib::checkKeyOneshot(DIK_F5, false, false, false) && SokuLib::mainMode == SokuLib::BATTLE_MODE_PRACTICE && SokuLib::subMode == SokuLib::BATTLE_SUBMODE_PLAYING1) {
		SokuLib::playSEWaveBuffer(SokuLib::SFX_MENU_CONFIRM);
		__practiceEffect = (__practiceEffect + 2) % (EFFECT_COUNT + 1) - 1;
	}
	if (this->_hammerPickTimer)
		this->_hammerPickTimer--;
	switch (this->frameState.actionId) {
	case SokuLib::ACTION_IDLE:
		this->applyGroundMechanics();
		if (this->speed.x == 0)
			this->groundDashCount = 0;
		this->_checkDashSlide();
		break;
	case SokuLib::ACTION_CROUCHING:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_CROUCHED);
		break;
	case SokuLib::ACTION_CROUCHED:
		this->applyGroundMechanics();
		this->_checkDashSlide();
		break;
	case SokuLib::ACTION_STANDING_UP:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		break;
	case SokuLib::ACTION_WALK_FORWARD:
		this->applyGroundMechanics();
		this->updateGroundMovement(this->_hammer ? FRONT_WALK_SPEED : HAMMER_FRONT_WALK_SPEED);
		this->advanceFrame();
		break;
	case SokuLib::ACTION_WALK_BACKWARD:
		this->applyGroundMechanics();
		this->updateGroundMovement(this->_hammer ? BACK_WALK_SPEED : HAMMER_BACK_WALK_SPEED);
		this->advanceFrame();
		break;
	case SokuLib::ACTION_NEUTRAL_JUMP:
		this->_jumpUpdate(0);
		break;
	case SokuLib::ACTION_FORWARD_JUMP:
		this->_jumpUpdate(this->_hammer ? JUMP_SPEED_X : HAMMER_JUMP_SPEED_X);
		break;
	case SokuLib::ACTION_BACKWARD_JUMP:
		this->_jumpUpdate(this->_hammer ? -JUMP_SPEED_X : -HAMMER_JUMP_SPEED_X);
		break;
	case SokuLib::ACTION_FALLING:
		if (this->gravity.y == 0)
			this->gravity.y = this->_hammer ? FALLING_GRAVITY : HAMMER_FALLING_GRAVITY;
		this->speed -= this->gravity;
		if (!this->applyAirMechanics()) {
			this->advanceFrame();
			break;
		}
		this->setAction(SokuLib::ACTION_LANDING);
		this->position.y = this->getGroundHeight();
		this->resetForces();
		break;
	case SokuLib::ACTION_LANDING:
		this->applyGroundMechanics();
		this->resetForces();
		if (this->advanceFrame())
			this->setAction(this->inputData.keyInput.verticalAxis == 0 ? SokuLib::ACTION_IDLE : SokuLib::ACTION_CROUCHED);
		break;
	case SokuLib::ACTION_FORWARD_TECH:
		if (
			(this->frameState.sequenceId == 0 && this->frameState.poseId < 4) ||
			(this->frameState.sequenceId == 2 && this->frameState.poseId > 0)
		)
			this->applyGroundMechanics();
		if (this->frameState.sequenceId < 3)
			this->speed.y -= this->gravity.y;
		if (this->speed.y < 0 && this->frameState.sequenceId < 2) {
			this->setSequence(2);
			return;
		}
		if (this->applyAirMechanics() && this->frameState.sequenceId < 3) {
			this->position.y = this->getGroundHeight();
			this->resetForces();
			this->setSequence(3);
			return;
		}
		if (this->advanceFrame()) {
			if (this->inputData.inputType == 2) {
				this->setAction(700);
				this->meleeInvulTimer = 3;
				this->projectileInvulTimer = 3;
				return;
			}
			this->setAction(this->inputData.keyInput.verticalAxis > 0 ? SokuLib::ACTION_CROUCHED : SokuLib::ACTION_IDLE);
		}
		if (this->frameState.sequenceId == 3 && this->frameState.poseId == 1 && this->frameState.poseFrame == 0) {
			if (this->position.x > this->gameData.opponent->position.x) {
				this->direction = SokuLib::LEFT;
				return;
			}
			if (this->position.x < this->gameData.opponent->position.x) {
				this->direction = SokuLib::RIGHT;
				return;
			}
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			if (this->_hammer) {
				this->speed.x = 12;
				this->gravity.y = 1;
				this->speed.y = 10;
			} else {
				this->speed.x = 10;
				this->gravity.y = 0.5;
				this->speed.y = 10;
			}
			return;
		}
		break;
	case SokuLib::ACTION_BACKWARD_TECH:
		if ((this->frameState.sequenceId == 0 && this->frameState.poseId < 4) || this->frameState.sequenceId == 2)
			this->applyGroundMechanics();
		if (this->frameState.sequenceId < 2)
			this->speed.y -= this->gravity.y;
		if (this->applyAirMechanics() && this->frameState.sequenceId < 2) {
			this->position.y = this->getGroundHeight();
			this->resetForces();
			this->setSequence(2);
			return;
		}
		if (this->advanceFrame()) {
			if (this->inputData.inputType == 2) {
				this->setAction(700);
				this->meleeInvulTimer = 3;
				this->projectileInvulTimer = 3;
				return;
			}
			this->setAction(this->inputData.keyInput.verticalAxis > 0 ? SokuLib::ACTION_CROUCHED : SokuLib::ACTION_IDLE);
		}
		if (this->frameState.sequenceId == 2 && this->frameState.poseId == 1 && this->frameState.poseFrame == 0) {
			if (this->position.x > this->gameData.opponent->position.x) {
				this->direction = SokuLib::LEFT;
				return;
			}
			if (this->position.x < this->gameData.opponent->position.x) {
				this->direction = SokuLib::RIGHT;
				return;
			}
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			if (this->_hammer) {
				this->speed.x = -12;
				this->gravity.y = 1;
				this->speed.y = 10;
			} else {
				this->speed.x = -10;
				this->gravity.y = 0.5;
				this->speed.y = 10;
			}
		}
		break;
	case SokuLib::ACTION_NEUTRAL_TECH:
		this->applyGroundMechanics();
		if (this->advanceFrame()) {
			if (this->inputData.inputType == 2) {
				this->setAction(700);
				this->meleeInvulTimer = 3;
				this->projectileInvulTimer = 3;
				return;
			}
			this->setAction(this->inputData.keyInput.verticalAxis > 0 ? SokuLib::ACTION_CROUCHED : SokuLib::ACTION_IDLE);
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 12 && this->frameState.poseFrame == 0) {
			if (this->position.x > this->gameData.opponent->position.x) {
				this->direction = SokuLib::LEFT;
				return;
			}
			if (this->position.x < this->gameData.opponent->position.x) {
				this->direction = SokuLib::RIGHT;
				return;
			}
		}
		break;
	case SokuLib::ACTION_BE6:
	case SokuLib::ACTION_FORWARD_DASH:
		this->applyGroundMechanics();
		this->groundDashCount = 1;
		if (this->frameState.actionId == SokuLib::ACTION_BE6) {
			if (this->_hammer) {
				if (TOP_BE6_SPEED < this->speed.x) {
					this->speed.x -= 0.25;
					if (this->speed.x < TOP_BE6_SPEED)
						this->speed.x = TOP_BE6_SPEED;
				}
			} else {
				if (HAMMER_TOP_BE6_SPEED < this->speed.x) {
					this->speed.x -= 0.15;
					if (this->speed.x < HAMMER_TOP_BE6_SPEED)
						this->speed.x = HAMMER_TOP_BE6_SPEED;
				}
			}
		} else {
			if (this->_hammer) {
				if (TOP_DASH_SPEED < this->speed.x) {
					this->speed.x -= 0.5;
					if (this->speed.x < TOP_DASH_SPEED)
						this->speed.x = TOP_DASH_SPEED;
				}
			} else {
				if (HAMMER_TOP_DASH_SPEED < this->speed.x) {
					this->speed.x -= 0.25;
					if (this->speed.x < HAMMER_TOP_DASH_SPEED)
						this->speed.x = HAMMER_TOP_DASH_SPEED;
				}
			}
		}
		this->advanceFrame();
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame % 5 == 0) {
				this->createEffect(
					0x7c,
					this->position.x + 50.0f - (float)(SokuLib::rand() % 100),
					this->position.y +         (float)(SokuLib::rand() % 200),
					this->direction, 1
				);
			}
			if (this->inputData.keyInput.verticalAxis < 0) {
				if (this->direction * this->inputData.keyInput.horizontalAxis > 0) {
					this->setAction(SokuLib::ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH);
					return;
				}
				this->setAction(SokuLib::ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH);
				return;
			}

			int minTimer = this->frameState.actionId == SokuLib::ACTION_FORWARD_DASH ? 5 : 25;

			this->dashTimer++;
			if ((this->direction * this->inputData.keyInput.horizontalAxis < 1 && this->dashTimer > minTimer) || this->dashTimer > MAX_DASH_HOLD) {
				this->setAction(SokuLib::ACTION_GROUNDDASH_RECOVERY);
				return;
			}
		}
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			if (this->frameState.actionId == SokuLib::ACTION_BE6)
				this->updateGroundMovement(this->_hammer ? INITIAL_BE6_SPEED : HAMMER_INITIAL_BE6_SPEED);
			else
				this->updateGroundMovement(this->_hammer ? INITIAL_DASH_SPEED : HAMMER_INITIAL_DASH_SPEED);
			this->createEffect(0x7d, (float)(this->direction * 0x50) + this->position.x, this->position.y + 80, this->direction, 1);
			this->createEffect(0x7e, this->position.x, this->position.y + 80, this->direction, 1);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
		}
		break;
	case SokuLib::ACTION_BE4:
	case SokuLib::ACTION_BACKDASH:
		if (this->frameState.sequenceId == 0 || frameState.sequenceId == 3)
			this->applyGroundMechanics();
		if (this->frameState.sequenceId == 3) {
			if (this->frameState.actionId == SokuLib::ACTION_BE4)
				this->speed.x += this->_hammer ? BE4_DECEL : HAMMER_BE4_DECEL;
			else
				this->speed.x += this->_hammer ? BACKDASH_DECEL : HAMMER_BACKDASH_DECEL;
			if (this->speed.x > 0)
				this->speed.x = 0;
		}
		if (!this->isGrounded() && (this->frameState.currentFrame % 5 == 0))
			this->createEffect(
				0x7c,
				this->position.x + 50.0f - (float)(SokuLib::rand() % 100),
				this->position.y + (float)(SokuLib::rand() % 200),
				this->direction, 1
			);
		if (this->frameState.sequenceId < 3) {
			this->speed.y -= this->gravity.y;
			if (this->applyAirMechanics()) {
				this->speed.y = 0.0;
				this->position.y = this->getGroundHeight();
				this->setSequence(3);
				return;
			}
		}
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
			this->speed.x = 0;
			return;
		}
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1 && this->dashTimer == 0) {
			this->dashTimer = 1;
			if (this->frameState.actionId == SokuLib::ACTION_BE4) {
				this->updateGroundMovement(this->_hammer ? BE4_IMPULSE : HAMMER_BE4_IMPULSE);
				this->speed.y = BE4_IMPULSE_UP;
				this->gravity.y = BE4_GRAVITY;
			} else {
				this->updateGroundMovement(this->_hammer ? BACKDASH_IMPULSE : HAMMER_BACKDASH_IMPULSE);
				this->speed.y = BACKDASH_IMPULSE_UP;
				this->gravity.y = BACKDASH_GRAVITY;
			}
			this->createEffect(0x7d, this->position.x, this->position.y + 80.0, -this->direction, 1);
			this->createEffect(0x7e, this->position.x, this->position.y + 80.0, -this->direction, 1);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
			return;
		}
		break;
	case SokuLib::ACTION_jBE6:
	case SokuLib::ACTION_FORWARD_AIRDASH:
		this->speed.y -= this->gravity.y;
		if (this->applyAirMechanics()) {
			this->resetForces();
			this->position.y = this->getGroundHeight();
			this->setAction(SokuLib::ACTION_LANDING);
			return;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame % 5 == 0)
			this->createEffect(
				0x7c,
				(this->position.x + 50.0f) - (float)(SokuLib::rand() % 100),
				this->position.y + (float)(SokuLib::rand() % 200),
				this->direction, 1
			);
		this->advanceFrame();
		if (this->frameState.sequenceId == 1 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
			this->dashTimer = 0;
			this->updateGroundMovement(this->_hammer ? FAD_SPEED_X : HAMMER_FAD_SPEED_X);
			this->speed.y = this->_hammer ? FAD_SPEED_Y : HAMMER_FAD_SPEED_Y;
			this->gravity.y = this->_hammer ? FAD_GRAVITY : HAMMER_FAD_GRAVITY;
			this->createEffect(0x7D, (float)(this->direction * 0x50) + this->position.x, this->position.y + 110.0, this->direction,1);
			this->createEffect(0x7E, this->position.x, this->position.y + 110.0, this->direction, 1);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
		}
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 2) {
			this->Unknown487C20();
			return;
		}
		break;
	case SokuLib::ACTION_jBE4:
	case SokuLib::ACTION_BACKWARD_AIRDASH:
		this->speed.y -= this->gravity.y;
		if (this->applyAirMechanics()) {
			this->resetForces();
			this->position.y = this->getGroundHeight();
			this->setAction(SokuLib::ACTION_LANDING);
			return;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame % 5 == 0)
			this->createEffect(
				0x7c,
				(this->position.x + 50.0f) - (float)(SokuLib::rand() % 100),
				this->position.y + (float)(SokuLib::rand() % 200),
				this->direction, 1
			);
		this->advanceFrame();
		if (this->frameState.sequenceId == 1 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
			this->updateGroundMovement(this->_hammer ? BAD_SPEED_X : HAMMER_BAD_SPEED_X);
			this->speed.y = this->_hammer ? BAD_SPEED_Y : HAMMER_BAD_SPEED_Y;
			this->gravity.y = this->_hammer ? BAD_GRAVITY : HAMMER_BAD_GRAVITY;
			this->createEffect(0x7D, (float)(this->direction * 0x50) + this->position.x, this->position.y + 110.0, this->direction,1);
			this->createEffect(0x7E, this->position.x, this->position.y + 110.0, this->direction, 1);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
		}
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 2) {
			this->Unknown487C20();
			return;
		}
		break;
	case SokuLib::ACTION_GROUNDDASH_RECOVERY:
		this->applyGroundMechanics();
		this->speed.x -= DASH_RECOVERY_DECEL;
		if (this->speed.x < 0) {
			this->speed.x = 0.0;
			this->groundDashCount = 0;
		}
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
			this->groundDashCount = 0;
		}
		break;
	case SokuLib::ACTION_FLY:
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		if (this->frameState.currentFrame == 0) {
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 6) {
				this->setAction(SokuLib::ACTION_FALLING);
				return;
			}
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
		}
		if (this->inputData.keyInput.verticalAxis > 0) {
			if (this->inputData.keyInput.horizontalAxis * this->direction < 0)
				this->flightTargetAngle = -135;
			else if (this->inputData.keyInput.horizontalAxis * this->direction > 0)
				this->flightTargetAngle = -45;
			else
				this->flightTargetAngle = -90;
		} else if (this->inputData.keyInput.verticalAxis < 0) {
			if (this->inputData.keyInput.horizontalAxis * this->direction > 0)
				this->flightTargetAngle = 45;
			else if (this->inputData.keyInput.horizontalAxis * this->direction < 0)
				this->flightTargetAngle = 135;
			else
				this->flightTargetAngle = 90;
		} else if (this->inputData.keyInput.horizontalAxis * this->direction < 0)
			this->flightTargetAngle = 180;
		else if (this->inputData.keyInput.horizontalAxis * this->direction > 0)
			this->flightTargetAngle = 0;

		if (this->frameState.sequenceId == 5 || this->frameState.sequenceId == 6)
			this->speed.y -= this->gravity.y;
		if (0 < this->frameState.sequenceId && this->frameState.sequenceId < 5) {
			this->flightTimer++;
			this->flightAngleDiff = this->flightTargetAngle - this->flightAngle;
			if (this->flightAngleDiff > 180)
				this->flightAngleDiff -= 360;
			if (this->flightAngleDiff < -180)
				this->flightAngleDiff += 360;
			if (this->flightAngleDiff > 0) {
				if (this->weatherId == SokuLib::WEATHER_SUNNY)
					this->flightAngle += this->_hammer ? FLIGHT_TURN_SUNNY : HAMMER_FLIGHT_TURN_SUNNY;
				else
					this->flightAngle += this->_hammer ? FLIGHT_TURN : HAMMER_FLIGHT_TURN;
			}
			if (this->flightAngleDiff < 0) {
				if (this->weatherId == SokuLib::WEATHER_SUNNY)
					this->flightAngle -= this->_hammer ? FLIGHT_TURN_SUNNY : HAMMER_FLIGHT_TURN_SUNNY;
				else
					this->flightAngle -= this->_hammer ? FLIGHT_TURN : HAMMER_FLIGHT_TURN;
			}
			this->speed.x = cos(this->flightAngle * M_PI / 180) * this->flightSpeed;
			this->speed.y = sin(this->flightAngle * M_PI / 180) * this->flightSpeed;
			if (this->position.y > 680 && this->speed.y > 0)
				this->speed.y = 0.0;
			this->flightSpeed += 0.3;
			if (this->flightSpeed > (this->_hammer ? FLIGHT_SPEED : HAMMER_FLIGHT_SPEED))
				this->flightSpeed = this->_hammer ? FLIGHT_SPEED : HAMMER_FLIGHT_SPEED;
			if (this->weatherId == SokuLib::WEATHER_SUNNY)
				this->consumeSpirit(5, 1);
			else
				this->consumeSpirit(10, 1);
			this->renderInfos.zRotation = -this->flightAngle;
			if (this->speed.x < 0.0)
				this->renderInfos.zRotation = 180.0 - this->flightAngle;
			if (this->speed.x < 0.0 && this->frameState.sequenceId == 1)
				this->setSequence(3);
			if (this->speed.x >= 0 && this->frameState.sequenceId == 3)
				this->setSequence(1);
			if (this->speed.x < 0.0 && this->frameState.sequenceId == 2)
				this->setSequence(4);
			if (this->speed.x >= 0 && this->frameState.sequenceId == 4)
				this->setSequence(2);
			if (this->frameState.currentFrame % 5 == 1)
				this->createEffect(
					0x7d,
					this->position.x + cos(this->flightAngle * M_PI / 180) * this->direction * 100.0,
					this->position.y + sin(this->flightAngle * M_PI / 180) * 100.0 + 100.0,
					this->direction, 1
				);
			if ((this->inputData.keyInput.d == 0 && this->flightTimer > 10) || this->currentSpirit < 1) {
				this->resetRenderInfo();
				if (this->frameState.sequenceId == 1 || this->frameState.sequenceId == 2) {
					if (this->direction == SokuLib::RIGHT) {
						if (this->gameData.opponent->position.x >= this->position.x)
							this->setSequence(5);
						else {
							this->direction = SokuLib::LEFT;
							this->speed.x = -this->speed.x;
							this->setSequence(6);
						}
					} else {
						if (this->gameData.opponent->position.x <= this->position.x)
							this->setSequence(5);
						else {
							this->direction = SokuLib::RIGHT;
							this->setSequence(6);
							this->speed.x = -this->speed.x;
						}
					}
				}
				if (this->frameState.sequenceId != 3 && this->frameState.sequenceId != 4)
					return;
				if (this->direction != SokuLib::RIGHT) {
					if (this->position.x < this->gameData.opponent->position.x) {
						this->direction = SokuLib::RIGHT;
						this->speed.x = -this->speed.x;
						this->setSequence(5);
						return;
					}
					this->setSequence(6);
					return;
				}
				if (this->position.x > this->gameData.opponent->position.x) {
					this->direction = SokuLib::LEFT;
					this->speed.x = -this->speed.x;
					this->setSequence(5);
					return;
				}
				this->setSequence(6);
				return;
			}
		}
		if (!this->applyAirMechanics())
			return;
		this->position.y = this->getGroundHeight();
		this->speed.y = 0.0;
		this->gravity.y = 0.0;
		if (this->frameState.sequenceId > 4) {
			this->setAction(SokuLib::ACTION_LANDING);
			this->resetForces();
			return;
		}
		this->resetRenderInfo();
		this->setAction(SokuLib::ACTION_HARDLAND);
		break;
	case SokuLib::ACTION_HARDLAND:
		this->applyGroundMechanics();
		if (this->speed.x > 0) {
			this->speed.x -= 0.75;
			if (this->speed.x < 0)
				this->speed.x = 0;
		}
		if (this->speed.x < 0.0) {
			this->speed.x += 0.75;
			if (this->speed.x > 0)
				this->speed.x = 0.0;
		}
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		break;
	case SokuLib::ACTION_BE2:
	case SokuLib::ACTION_NEUTRAL_HIGH_JUMP:
		if (this->_hammer)
			this->_highJumpUpdate(0, HIGH_JUMP_SPEED_NEUTRAL_Y, HIGH_JUMP_GRAVITY);
		else
			this->_highJumpUpdate(0, HAMMER_HIGH_JUMP_SPEED_NEUTRAL_Y, HAMMER_HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_FORWARD_HIGH_JUMP:
		if (this->_hammer)
			this->_highJumpUpdate(HIGH_JUMP_SPEED_X, HIGH_JUMP_SPEED_Y, HIGH_JUMP_GRAVITY);
		else
			this->_highJumpUpdate(HAMMER_HIGH_JUMP_SPEED_X, HAMMER_HIGH_JUMP_SPEED_Y, HAMMER_HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_BE1:
	case SokuLib::ACTION_BACKWARD_HIGH_JUMP:
		if (this->_hammer)
			this->_highJumpUpdate(-HIGH_JUMP_SPEED_X, HIGH_JUMP_SPEED_Y, HIGH_JUMP_GRAVITY);
		else
			this->_highJumpUpdate(-HAMMER_HIGH_JUMP_SPEED_X, HAMMER_HIGH_JUMP_SPEED_Y, HAMMER_HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH:
		if (this->_hammer)
			this->_highJumpUpdate(N_HIGH_JUMP_FD_SPEED_X, HIGH_JUMP_SPEED_NEUTRAL_Y, HIGH_JUMP_GRAVITY);
		else
			this->_highJumpUpdate(HAMMER_N_HIGH_JUMP_FD_SPEED_X, HAMMER_HIGH_JUMP_SPEED_NEUTRAL_Y, HAMMER_HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH:
		if (this->_hammer)
			this->_highJumpUpdate(F_HIGH_JUMP_FD_SPEED_X, HIGH_JUMP_SPEED_Y, HIGH_JUMP_GRAVITY);
		else
			this->_highJumpUpdate(HAMMER_F_HIGH_JUMP_FD_SPEED_X, HAMMER_HIGH_JUMP_SPEED_Y, HAMMER_HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_2A:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_LIGHT_ATTACK);
		break;
	case SokuLib::ACTION_6A:
		this->applyGroundMechanics();
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.a == 0)
			this->chargedAttack = false;
		if (this->speed.x > 0)
			this->speed.x -= 0.5;
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
			if (this->frameState.poseId == 4 && this->chargedAttack) {
				this->nextSequence();
				this->createEffect(0x3E, (float)(this->direction * 120) + this->position.x, this->position.y + 122.0, this->direction, 1);
			}
			if (this->frameState.poseId == 5)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 1)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		break;
	case SokuLib::ACTION_3A:
		this->applyGroundMechanics();
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.a == 0)
			this->chargedAttack = false;
		if (this->speed.x > 0)
			this->speed.x -= 0.5;
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->setAction(SokuLib::ACTION_CROUCHED);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
			if (this->frameState.poseId == 2 && this->chargedAttack) {
				this->nextSequence();
				this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
			}
			if (this->frameState.poseId == 3) {
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
				this->speed.x = 12;
			}
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 1) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed.x = 20;
		}
		break;
	case SokuLib::ACTION_4A:
	case SokuLib::ACTION_5A:
	case SokuLib::ACTION_5AA:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_LIGHT_ATTACK);
		break;
	case SokuLib::ACTION_f2A:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->speed.x > 0)
			this->speed.x -= 0.5;
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 1)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		break;
	case SokuLib::ACTION_66A:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->speed.x > 0)
			this->speed.x -= 0.5;
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		break;
	case SokuLib::ACTION_f5A:
	case SokuLib::ACTION_5AAA:
	case SokuLib::ACTION_5AAAA:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->speed.x > 0)
			this->speed.x -= 0.5;
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		break;
	case SokuLib::ACTION_j5A:
		this->speed -= this->gravity;
		if (this->applyAirMechanics()) {
			this->setAction(SokuLib::ACTION_LANDING);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		break;
	case SokuLib::ACTION_j8A:
		if (
			(this->frameState.sequenceId == 1 && this->frameState.poseId > 2) ||
			(this->frameState.sequenceId == 0 && (this->frameState.poseId >= 5 || this->frameState.poseId < 3))
		)
			this->speed -= this->gravity;
		if (this->speed.y < 0 && (
			(this->frameState.sequenceId == 1 && this->frameState.poseId <= 2) ||
			(this->frameState.sequenceId == 0 && this->frameState.poseId < 5)
		)) {
			this->speed.y += 0.25;
			if (this->speed.y > 0)
				this->speed.y = 0;
		}
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.a == 0)
			this->chargedAttack = false;
		if (this->frameState.sequenceId == 2) {
			this->applyGroundMechanics();
			if (this->advanceFrame())
				this->setAction(SokuLib::ACTION_IDLE);
			break;
		}
		if (this->applyAirMechanics()) {
			this->setSequence(2);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.currentFrame == 0 && this->frameState.sequenceId != 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0)
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			this->speed.x = std::copysign(max(std::abs(this->speed.x), 2), this->speed.x);
			this->speed.y = 0;
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 4) {
			if (this->chargedAttack) {
				this->nextSequence();
				this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
			}
			this->collisionType = COLLISION_TYPE_NONE;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 5) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed.y = 10;
			this->gravity.y = FALLING_GRAVITY * 3 / 4;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed.y = 15;
			this->gravity.y = FALLING_GRAVITY;
		}
		break;
	case SokuLib::ACTION_j2A:
		if (this->frameState.sequenceId != 1 || this->frameState.poseId > 2)
			this->speed -= this->gravity;
		if (this->frameState.sequenceId == 2) {
			this->applyGroundMechanics();
			if (this->advanceFrame())
				this->setAction(SokuLib::ACTION_IDLE);
			break;
		}
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.a == 0)
			this->chargedAttack = false;
		if (this->applyAirMechanics()) {
			this->setSequence(2);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.currentFrame == 0 && this->frameState.sequenceId != 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0)
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.poseId < 3) {
			if (this->frameState.poseId == 2 && this->chargedAttack) {
				this->nextSequence();
				this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
			}
			if (this->speed.y > 1)
				this->speed.y -= 2;
			else if (this->speed.y < -1)
				this->speed.y += 2;
			else
				this->speed.y = 0;
			if (this->speed.x > 4)
				this->speed.x -= 1;
			else if (this->speed.x < -4)
				this->speed.x += 1;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed.y = 5;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed.y = 5;
		}
		break;
	case SokuLib::ACTION_j6A:
		this->speed -= this->gravity;
		if (this->frameState.sequenceId == 1) {
			this->applyGroundMechanics();
			if (this->advanceFrame())
				this->setAction(SokuLib::ACTION_IDLE);
			break;
		}
		if (this->applyAirMechanics()) {
			this->nextSequence();
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.currentFrame == 0 && this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0)
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0) {
			if (this->frameState.poseId < 3)
				this->speed = {12, 0};
			else if (this->frameState.poseId >= 4 && this->speed.x > 0)
				this->speed.x -= 0.5;
			else if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		}
		break;
	case ACTION_j8A_HAMMER:
		this->speed -= this->gravity;
		if (this->applyAirMechanics()) {
			this->setAction(SokuLib::ACTION_LANDING);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 4) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed.y = 15;
			this->gravity.y = 1;
		}
		break;
	case ACTION_j2A_HAMMER:
		this->speed -= this->gravity;
		if (this->applyAirMechanics()) {
			this->setAction(SokuLib::ACTION_LANDING);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		break;
	case ACTION_j6A_HAMMER:
		this->speed -= this->gravity;
		if (this->applyAirMechanics()) {
			this->setAction(SokuLib::ACTION_LANDING);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed = {-4, 10};
		}
		break;
	case ACTION_j5A_HAMMER:
		this->speed -= this->gravity;
		if (this->applyAirMechanics()) {
			this->setAction(SokuLib::ACTION_LANDING);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		break;
	case SokuLib::ACTION_66B:
	case SokuLib::ACTION_5AAA3A:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.poseId >= 4 && this->speed.x > 0)
			this->speed.x -= 0.5;
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		break;
	case SokuLib::ACTION_66C:
	case ACTION_5AAA6A:
		if (this->frameState.sequenceId == 1) {
			this->speed.y -= this->gravity.y;
			if (this->applyAirMechanics()) {
				this->speed.x = 0;
				this->nextSequence();
			}
		} else
			this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0) {
			this->speed.x = 15;
			this->speed.y = 6;
			this->gravity.y = 0.75;
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		}
		break;
	case ACTION_2A_HAMMER:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		break;
	case ACTION_4A_HAMMER:
	case ACTION_5A_HAMMER:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 4)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		break;
	case ACTION_66A_HAMMER:
		this->applyGroundMechanics();
		if (this->frameState.poseId < 4)
			this->advanceFrame();
		else if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		break;
	case ACTION_5AA_HAMMER:
		if (!this->gameData.frameData->frameFlags.airborne)
			this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		switch (this->frameState.poseId) {
		case 12:
			if (this->frameState.poseFrame == 0) {
				this->speed.x = 0;
				this->speed.y = 0;
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
				this->collisionType = COLLISION_TYPE_NONE;
				this->hasMoveBeenReset = true;
			}
			break;
		case 7:
			if (this->frameState.poseFrame == 0) {
				SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
				this->speed.x = 8;
				this->gravity.y = 0.75;
				this->speed.y = this->gravity.y * (8 + 8 + 8 + 4 + 4) / 2.f;
			}
			this->speed.y -= this->gravity.y;
			break;
		case 15:
			if (this->frameState.poseFrame == 0)
				this->speed.y = this->gravity.y * (8 + 8 + 8 + 4 + 4) / 2.f - this->gravity.y * (8 + 8 + 8 + 4);
		case 8:
		case 9:
		case 10:
		case 11:
		case 16:
			this->speed.y -= this->gravity.y;
			break;
		case 17:
			if (this->frameState.poseFrame == 0)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_LAND);
		}
		break;
	case ACTION_3A_HAMMER:
		this->applyGroundMechanics();
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.a == 0)
			this->chargedAttack = false;
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0)
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 2 && this->chargedAttack) {
			this->nextSequence();
			this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		break;
	case ACTION_66B_HAMMER:
		this->applyGroundMechanics();
		if (4 > this->frameState.poseId)
			this->advanceFrame();
		else if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
			if (3 <= this->frameState.poseId && this->frameState.poseId <= 9) {
				SokuLib::playSEWaveBuffer(SokuLib::SFX_LIGHT_ATTACK);
				this->collisionType = COLLISION_TYPE_NONE;
				this->hasMoveBeenReset = true;
			}
		}
		break;
	case ACTION_66C_HAMMER:
		if (!this->gameData.frameData->frameFlags.airborne)
			this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_CROUCHED);
		this->groundDashCount = 0;
		switch (this->frameState.poseId) {
		case 4:
			if (this->frameState.poseFrame == 0) {
				this->gravity.y = 0.75;
				this->speed.y = this->gravity.y * (4 + 4 + 16 + 8 + 4) / 2.f;
			}
			this->speed.y -= this->gravity.y;
			break;
		case 6:
			if (this->frameState.poseFrame == 0) {
				this->speed.y = 0;
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
				this->collisionType = COLLISION_TYPE_NONE;
				this->hasMoveBeenReset = true;
			}
			break;
		case 7:
			if (this->speed.x > 0)
				this->speed.x -= 0.5;
			if (this->frameState.poseFrame == 0) {
				float hammerParams[5] = {90.f, 30, 0, 0, 0};

				this->_hammerPickTimer = 20;
				this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
				this->_hammer->collisionType = COLLISION_TYPE_HIT;
				this->speed.y = this->gravity.y * -2;
			}
		case 5:
			this->speed.y -= this->gravity.y;
			break;
		case 8:
			if (this->speed.x > 0)
				this->speed.x -= 1;
			if (this->frameState.poseFrame == 0) {
				SokuLib::playSEWaveBuffer(SokuLib::SFX_KNOCK_DOWN);
			}
		}
		break;
	case ACTION_f5A_HAMMER:
	case ACTION_5AAA_HAMMER:
		this->applyGroundMechanics();
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.a == 0)
			this->chargedAttack = false;
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0)
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 2 && this->chargedAttack) {
			this->nextSequence();
			this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 4)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		break;
	case ACTION_6A_HAMMER:
		this->applyGroundMechanics();
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.a == 0)
			this->chargedAttack = false;
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0)
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 4 && this->chargedAttack) {
			this->nextSequence();
			this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 1)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 5)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		break;


	case ACTION_j2B_HAMMER:
		this->_bSeriesUpdate(-45, HAMMER_B_BULLET_SPEED, HAMMER_B_BULLET_SLOW_DOWN, 60, true);
		break;
	case SokuLib::ACTION_j2B:
		this->_bSeriesUpdate(-45, B_BULLET_SPEED, B_BULLET_SLOW_DOWN, 60, true);
		break;
	case ACTION_j5B_HAMMER:
		this->_bSeriesUpdate(0, HAMMER_B_BULLET_SPEED, HAMMER_B_BULLET_SLOW_DOWN, 100, true);
		break;
	case SokuLib::ACTION_j5B:
		this->_bSeriesUpdate(0, B_BULLET_SPEED, B_BULLET_SLOW_DOWN, 100, true);
		break;
	case ACTION_2B_HAMMER:
		this->_bSeriesUpdate(45, HAMMER_B_BULLET_SPEED, HAMMER_B_BULLET_SLOW_DOWN, 70, false);
		break;
	case SokuLib::ACTION_2B:
		this->_bSeriesUpdate(45, B_BULLET_SPEED, B_BULLET_SLOW_DOWN, 70, false);
		break;
	case ACTION_5B_HAMMER:
		this->_bSeriesUpdate(0, HAMMER_B_BULLET_SPEED, HAMMER_B_BULLET_SLOW_DOWN, 100, false);
		break;
	case SokuLib::ACTION_5B:
		this->_bSeriesUpdate(0, B_BULLET_SPEED, B_BULLET_SLOW_DOWN, 100, false);
		break;
	case ACTION_6B_HAMMER:
		this->_bSeriesUpdate(35, HAMMER_B_BULLET_SPEED, HAMMER_B_BULLET_SLOW_DOWN, 100, false);
		break;
	case ACTION_6B:
		this->_bSeriesUpdate(35, B_BULLET_SPEED, B_BULLET_SLOW_DOWN, 100, false);
		break;

	case ACTION_4C:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
			if (this->frameState.poseId == 6) {
				float hammerParams[5] = {90.f + 45 * this->direction, 20, 0, 0, 0};

				this->consumeSpirit(200, 60);
				this->collisionType = COLLISION_TYPE_HIT;
				this->addCardMeter(50);
				this->playSFX(0);
				this->_hammerPickTimer = 20;
				this->_hammer = this->createObject(800, (this->direction * -40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
			}
		}
		break;
	case SokuLib::ACTION_5C:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
			if (this->frameState.poseId == 4) {
				float hammerParams[5] = {90.f - 45 * this->direction, 20, 0, 0, 0};

				this->consumeSpirit(200, 60);
				this->collisionType = COLLISION_TYPE_HIT;
				this->addCardMeter(50);
				this->playSFX(0);
				this->_hammerPickTimer = 20;
				this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
			}
		}
		break;
	case SokuLib::ACTION_6C:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
			if (this->frameState.poseId == 5) {
				float hammerParams[5] = {90.f - 80 * this->direction, 25, 0, 0};

				this->consumeSpirit(200, 60);
				this->collisionType = COLLISION_TYPE_HIT;
				this->addCardMeter(50);
				this->playSFX(0);
				this->_hammerPickTimer = 20;
				this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
			}
		}
		break;
	case SokuLib::ACTION_2C:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
			if (this->frameState.poseId == 4) {
				float hammerParams[5] = {90.f - 10 * this->direction, 30, 0, 0};

				this->consumeSpirit(200, 60);
				this->collisionType = COLLISION_TYPE_HIT;
				this->addCardMeter(50);
				this->playSFX(0);
				this->_hammerPickTimer = 20;
				this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
			}
		}
		break;
	case SokuLib::ACTION_j5C:
	case SokuLib::ACTION_j6C:
	case SokuLib::ACTION_j2C:
		if (this->frameState.sequenceId == 2) {
			this->applyGroundMechanics();
			if (this->advanceFrame())
				this->setAction(SokuLib::ACTION_FALLING);
			break;
		}
		this->advanceFrame();
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId != 0) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		if (this->applyAirMechanics()) {
			this->setSequence(2);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			float hammerParams[5] = {-90.f + 45.f * this->direction, 30, 1, 0, 0};

			if (this->frameState.actionId == SokuLib::ACTION_j6C) {
				hammerParams[0] = 90.f - 75.f * this->direction;
				hammerParams[2] = 0;
			} else if (this->frameState.actionId == SokuLib::ACTION_j2C)
				hammerParams[0] = -90.f;

			this->consumeSpirit(200, 60);
			this->collisionType = COLLISION_TYPE_HIT;
			this->addCardMeter(50);
			this->playSFX(0);
			this->_hammerPickTimer = 20;
			this->speed.y = 12;
			this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 200, this->direction, 1, hammerParams);
		}
		if (this->frameState.poseId >= 3)
			this->speed -= this->gravity;
		else if (std::abs(this->speed.y) > 3)
			this->speed.y = std::copysign(3, this->speed.y);
		break;

	case ACTION_STAND_PICKUP_HAMMER_FROM_AIR:
	case ACTION_STAND_PICKUP_HAMMER_FROM_GROUND:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		break;
	case ACTION_AIR_PICKUP_HAMMER_FROM_AIR:
	case ACTION_AIR_PICKUP_HAMMER_FROM_GROUND:
		if (this->applyAirMechanics()) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		this->speed.y -= this->gravity.y;
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		break;

	case ACTION_d623B:
	case ACTION_d623C:
	case ACTION_jd623B:
	case ACTION_jd623C: {
		bool b = this->frameState.sequenceId == 0;

		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId != 0)
			this->speed.y -= this->gravity.y;
		if (b && this->frameState.sequenceId == 1) {
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			switch (this->frameState.actionId) {
			case ACTION_d623B:
				this->speed.x = 12;
				this->speed.y = 15;
				this->gravity.y = FALLING_GRAVITY / 2;
				break;
			case ACTION_jd623B:
				this->speed.x = 15;
				this->speed.y = 10;
				this->gravity.y = FALLING_GRAVITY;
				break;
			case ACTION_jd623C:
				this->speed.x = 10;
				this->speed.y = -10;
				this->gravity.y = 0;
				break;
			}
		}
		if (this->frameState.sequenceId == 4) {
			this->applyGroundMechanics();
			this->resetForces();
			this->resetRenderInfo();
			break;
		}
		if (this->applyAirMechanics()) {
			this->setSequence(4);
			return;
		}
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.poseId == 0 && this->frameState.poseFrame == 0)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
			if (this->collisionType != COLLISION_TYPE_NONE) {
				this->nextSequence();
				this->resetRenderInfo();
				this->center = {0, 0};
				this->gravity.y = 0.5;
				this->speed.x = -2;
				this->speed.y = 10;
			} else {
				this->center = {0, 78};
				this->renderInfos.zRotation = 360 * (this->frameState.poseFrame / 2) / this->frameState.poseDuration;
			}
		}
		break;
	}

	case ACTION_d623B_HAMMER:
	case ACTION_d623C_HAMMER:
	case ACTION_jd623B_HAMMER:
	case ACTION_jd623C_HAMMER: {
		bool b = this->frameState.sequenceId == 0;

		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 4 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0)
			this->setAction(SokuLib::ACTION_FALLING);
		if (b && this->frameState.sequenceId == 1)
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
		if (this->frameState.sequenceId == 4) {
			this->applyGroundMechanics();
			this->resetForces();
			this->resetRenderInfo();
			break;
		}
		if (this->applyAirMechanics()) {
			this->setSequence(4);
			return;
		}
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
				if (this->collisionType == COLLISION_TYPE_NONE && !b)
					this->collisionLimit--;
				if (this->collisionLimit == 0) {
					this->nextSequence();
					this->resetRenderInfo();
					this->center = {0, 0};
				} else {
					this->collisionType = COLLISION_TYPE_NONE;
					this->hasMoveBeenReset = true;
					SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
				}
			}
			if (this->frameState.sequenceId == 1) {
				this->center = {0, 78};
				this->renderInfos.zRotation = 360 * (this->frameState.poseFrame / 2) / this->frameState.poseDuration;
			}
		}
		this->speed.y -= this->gravity.y;
		break;
	}

	case ACTION_d236B:
	case ACTION_d236C:
	case ACTION_d236B_HAMMER:
	case ACTION_d236C_HAMMER: {
		float orbData[6] = {
			90.f - 15.f, 12.5, 2,
			90.f - 45.f, 10, 2
		};

		if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0)
			this->setPose(5 + this->effectiveSkillLevel[1]);
		if (this->frameState.poseId >= 3 && this->frameState.poseId <= 9) {
			this->advanceFrame();
			if (this->applyAirMechanics())
				this->setPose(10);
			this->speed.y -= this->gravity.y;
		} else if (this->_checkDashSlide() || (this->frameState.sequenceId == 1 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0))
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
			this->collisionType = COLLISION_TYPE_HIT;
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->playSFX(0);
			this->createObject(
				809,
				this->position.x, this->position.y + 100,
				this->direction, 1,
				&orbData[this->frameState.actionId % 2 * 3], 3
			)->skillIndex = 1;
			this->position.y = -1;
			this->gravity.y = (this->_hammer ? FALLING_GRAVITY : HAMMER_FALLING_GRAVITY) * 1.5;
			this->speed = {-5, 15};
		}
		break;
	}

	case ACTION_jd236B:
	case ACTION_jd236C:
	case ACTION_jd236B_HAMMER:
	case ACTION_jd236C_HAMMER: {
		float orbData[6] = {
			-90.f + 15.f, 12.5, 3,
			90.f - 45.f, 10, 2
		};

		if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0)
			this->setPose(5 + this->effectiveSkillLevel[1]);
		if (!this->isGrounded() || this->frameState.poseId <= 9) {
			if (this->frameState.poseId < 3)
				this->gravity.y = (this->_hammer ? FALLING_GRAVITY : HAMMER_FALLING_GRAVITY) / 2;
			this->advanceFrame();
			if (this->applyAirMechanics() || (this->frameState.sequenceId == 0 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0)) {
				if (this->frameState.poseId == 9)
					this->nextPose();
				else if (this->isGrounded())
					this->setAction(SokuLib::ACTION_IDLE);
				else
					this->setAction(SokuLib::ACTION_FALLING);
			}
			this->speed.y -= this->gravity.y;
		} else if (this->_checkDashSlide() || (this->frameState.sequenceId == 0 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0))
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
			this->collisionType = COLLISION_TYPE_HIT;
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->playSFX(0);
			this->createObject(
				809,
				this->position.x, this->position.y + 100,
				this->direction, 1,
				&orbData[this->frameState.actionId % 2 * 3], 3
			)->skillIndex = 1;
			this->gravity.y = (this->_hammer ? FALLING_GRAVITY : HAMMER_FALLING_GRAVITY) * 1.5;
			this->speed = {-5, 15};
		}
		break;
	}

	case ACTION_d214B:
	case ACTION_d214C:
	case ACTION_d214B_HAMMER:
	case ACTION_d214C_HAMMER:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (
			this->frameState.currentFrame == 25 &&
			this->createObject(803, this->position.x + (1 - this->frameState.actionId % 2 * 2) * 100 * this->direction, this->position.y, this->direction, -1)->lifetime
		) {
			this->playSFX(0);
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
		}
		if (this->frameState.currentFrame == 35)
			this->collisionType = COLLISION_TYPE_HIT;
		if (this->frameState.currentFrame == 60)
			this->nextSequence();
		break;

	case ACTION_d22B:
	case ACTION_d22C:
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.poseId == 7 && this->frameState.poseFrame == 0) {
			this->collisionType = COLLISION_TYPE_HIT;
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->playSFX(0);
			this->createObject(
				804,
				this->position.x, this->position.y,
				this->direction, 1
			);
		}
		break;

	case ACTION_jd22B:
	case ACTION_jd22C:
		this->speed.x = 0;
		this->speed.y = 0;
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.poseId <= 7 && this->frameState.poseFrame == 0) {
			this->speed.x = 0;
			this->speed.y = 0;
		} else
			this->speed.y -= 1;
		if (this->frameState.poseId == 7 && this->frameState.poseFrame == 0) {
			this->collisionType = COLLISION_TYPE_HIT;
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->playSFX(0);

			auto rabbit = this->createObject(
				804,
				this->position.x, this->position.y + 50,
				this->direction, 1
			);

			rabbit->setPose(4);
			if (this->frameState.actionId == ACTION_jd22B)
				rabbit->speed.x = 2;
			else
				rabbit->speed.x = 5;
		}
		break;

	case ACTION_d22B_HAMMER:
	case ACTION_d22C_HAMMER:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
			if (this->frameState.poseId == 5) {
				float hammerParams[5] = {90.f - 90.f * this->direction, 35, 0, 1, 0};

				if (this->frameState.actionId == ACTION_d22C_HAMMER) {
					hammerParams[0] = fmod(atan2(this->gameData.opponent->position.y - this->position.y, this->gameData.opponent->position.x - this->position.x) * 180 / M_PI + 360, 360);
					if (this->direction == SokuLib::LEFT) {
						if (hammerParams[0] < 90)
							hammerParams[0] = 90;
						else if (hammerParams[0] > 180)
							hammerParams[0] = 180;
					} else {
						if (hammerParams[0] > 180)
							hammerParams[0] = 0;
						else if (hammerParams[0] > 90)
							hammerParams[0] = 90;
					}
				}
				this->consumeSpirit(200, 120);
				this->collisionType = COLLISION_TYPE_HIT;
				this->addCardMeter(70);
				this->playSFX(0);
				this->_hammerPickTimer = 20;
				this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
				this->_hammer->HP = 10000;
				this->_hammer->gravity.y = 0;
			}
		}
		break;
	case ACTION_jd22B_HAMMER:
	case ACTION_jd22C_HAMMER:
		if (this->frameState.sequenceId == 2) {
			this->applyGroundMechanics();
			if (this->advanceFrame())
				this->setAction(SokuLib::ACTION_FALLING);
			break;
		}
		this->advanceFrame();
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId != 0) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		if (this->applyAirMechanics()) {
			this->setSequence(2);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			float hammerParams[5] = {90.f - 90.f * this->direction, 35, 0, 1, 0};

			if (this->frameState.actionId == ACTION_jd22C_HAMMER) {
				hammerParams[0] = fmod(atan2(this->gameData.opponent->position.y - this->position.y, this->gameData.opponent->position.x - this->position.x) * 180 / M_PI + 360, 360);
				if (this->direction == SokuLib::LEFT) {
					if (hammerParams[0] > 270)
						hammerParams[0] = 270;
					else if (hammerParams[0] < 180)
						hammerParams[0] = 180;
				} else {
					if (hammerParams[0] < 180)
						hammerParams[0] = 0;
					else if (hammerParams[0] < 270)
						hammerParams[0] = 270;
				}
			}
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->collisionType = COLLISION_TYPE_HIT;
			this->addCardMeter(70);
			this->playSFX(0);
			this->_hammerPickTimer = 20;
			this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
			this->_hammer->gravity.y = 0;
		}
		if (this->frameState.poseId >= 3)
			this->speed -= this->gravity;
		else if (std::abs(this->speed.y) > 3)
			this->speed.y = std::copysign(3, this->speed.y);
		break;

	case ACTION_a1_623B:
	case ACTION_a1_623C:
	case ACTION_a1_623B_HAMMER:
	case ACTION_a1_623C_HAMMER: {
		float orbData[3] = {90.f, 10, 2};

		this->applyGroundMechanics();
		if (this->advanceFrame() || (this->frameState.poseId == 0 && this->frameState.poseFrame == 0)) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0) {
			if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
				this->collisionType = COLLISION_TYPE_HIT;
				this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
				this->playSFX(0);
				this->createObject(853, this->position.x + 50 * this->direction, this->position.y + 100, this->direction, 1, orbData)->skillIndex = 4;
			}
			if (this->frameState.poseId >= 5 && this->frameState.poseId <= 11) {
				if (this->inputData.bufferedKeyInput.a || this->inputData.keyInput.a) {
					this->setSequence(1);
					this->collisionType = COLLISION_TYPE_NONE;
					this->hasMoveBeenReset = true;
				} else if (this->inputData.bufferedKeyInput.b || this->inputData.keyInput.b) {
					this->setSequence(2);
					this->collisionType = COLLISION_TYPE_NONE;
					this->hasMoveBeenReset = true;
				} else if (this->inputData.bufferedKeyInput.c || this->inputData.keyInput.c) {
					this->setSequence(3);
					this->collisionType = COLLISION_TYPE_NONE;
					this->hasMoveBeenReset = true;
				}
			}
		} else if (this->frameState.actionId < ACTION_a1_623B_HAMMER) {
			if (this->frameState.sequenceId == 1) {
				if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
					SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
			} else if (this->frameState.sequenceId == 2) {
				if (this->frameState.poseFrame == 0 && this->frameState.poseId == 1)
					SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
			} else if (this->frameState.sequenceId == 3) {
				if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
					SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			}
		} else {
			if (this->frameState.sequenceId == 1) {
				if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
					SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
			} else if (this->frameState.sequenceId == 2) {
				if (this->frameState.poseFrame == 0 && 3 <= this->frameState.poseId &&
				    this->frameState.poseId <= 9) {
					SokuLib::playSEWaveBuffer(SokuLib::SFX_LIGHT_ATTACK);
					this->collisionType = COLLISION_TYPE_NONE;
					this->hasMoveBeenReset = true;
				}
			} else if (this->frameState.sequenceId == 3) {
				if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
					SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			}
		}
		break;
	}

	case ACTION_a1_236B:
	case ACTION_a1_236C:
	case ACTION_a1_236B_HAMMER:
	case ACTION_a1_236C_HAMMER: {
		float rabbitData[6] = {
			10, 7.5, 1.f + this->effectiveSkillLevel[5],
			6, 12.5, 1.f + this->effectiveSkillLevel[5]
		};

		if (this->applyGroundMechanics())
			break;
		if (this->_checkDashSlide() || (this->frameState.sequenceId == 1 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0))
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.actionId % 2 == 0 && this->inputData.keyInput.b == 0)
			this->chargedAttack = false;
		if (this->frameState.actionId % 2 == 1 && this->inputData.keyInput.c == 0)
			this->chargedAttack = false;
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.poseId == 1 && this->frameState.poseFrame == 0) {
				this->collisionType = COLLISION_TYPE_HIT;
				this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
				this->playSFX(0);
				this->createObject(802, this->position.x, this->position.y, this->direction, 1, &rabbitData[0], 3)->skillIndex = 5;
				this->createObject(802, this->position.x, this->position.y, this->direction, 1, &rabbitData[3], 3)->skillIndex = 5;
			}
		} else if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
			if (this->chargedAttack) {
				this->nextSequence();
				this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
				break;
			}
			this->collisionType = COLLISION_TYPE_HIT;
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->playSFX(0);
			this->createObject(
				802,
				this->position.x, this->position.y,
				this->direction, 1,
				&rabbitData[this->frameState.actionId % 2 * 3], 3
			)->skillIndex = 5;
		}
		break;
	}

	case ACTION_ja1_236B:
	case ACTION_ja1_236C:
	case ACTION_ja1_236B_HAMMER:
	case ACTION_ja1_236C_HAMMER: {
		float rabbitData[6] = {
			10, 7.5, 1.f + this->effectiveSkillLevel[5],
			6, 12.5, 1.f + this->effectiveSkillLevel[5]
		};

		if (this->applyAirMechanics())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0) {
			this->collisionType = COLLISION_TYPE_HIT;
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->playSFX(0);
			this->playSFX(3);

			auto obj = this->createObject(
				802,
				this->position.x, this->position.y,
				this->direction, 1,
				&rabbitData[this->frameState.actionId % 2 * 3], 3
			);

			obj->skillIndex = 5;
			obj->setPose(4);
			obj->speed.x = rabbitData[this->frameState.actionId % 2 * 3 + 0] / 2;
			obj->speed.y = rabbitData[this->frameState.actionId % 2 * 3 + 1] / 2;
		}
		break;
	}

	case ACTION_a1_214B:
	case ACTION_a1_214B_HAMMER:
		if (this->inputData.keyInput.b == 0)
			this->chargedAttack = false;
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		else if (this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
			if (this->chargedAttack && this->_rabbitsStored < maxRabbits(this)) {
				this->collisionType = COLLISION_TYPE_NONE;
				this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 100, this->direction, 1);
			} else
				this->nextSequence();
		} else if (this->frameState.poseId == 0 && this->frameState.poseFrame == 3 && this->frameState.sequenceId == 0) {
			this->playSFX(0);
			this->createObject(854, this->position.x, this->position.y, this->direction, 1)->skillIndex = 6;
			this->_rabbitsStored++;
			this->collisionType = COLLISION_TYPE_HIT;
		}
		break;

	case ACTION_a1_214C:
	case ACTION_a1_214C_HAMMER:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.poseId >= 7 && this->frameState.currentFrame % 3 == 0 && this->_rabbitsStored) {
			auto dir = SokuLib::rand(2) ? SokuLib::LEFT : SokuLib::RIGHT;

			if (this->frameState.poseId == 7 && this->frameState.poseFrame < 3)
				this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->_rabbitsStored--;
			this->createObject(
				855,
				this->gameData.opponent->position.x - 100 * (float)dir,
				this->getGroundHeight(), dir, 1
			)->skillIndex = 6;
			if (this->_rabbitsStored == 0) {
				this->collisionType = COLLISION_TYPE_HIT;
				this->collisionLimit = 0;
			}
			this->playSFX(1);
		}
		break;

	case ACTION_a1_22B:
	case ACTION_a1_22C:
		if (this->frameState.currentFrame == 0)
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
		this->advanceFrame();
		if (this->frameState.currentFrame % 10 == 0) {
			float params[2] = {
				static_cast<float>(SokuLib::rand(360)),
				static_cast<float>(SokuLib::rand(150) + 250)
			};

			this->createObject(849, this->position.x, this->position.y, this->direction, 1, params);
		}
		break;

	case ACTION_a1_22B_HAMMER:
	case ACTION_a1_22C_HAMMER:
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 5) {
			float hammerParams[5] = {90.f, 25, 0, 1, 0};

			if (this->frameState.actionId % 2 == 0)
				hammerParams[0] -= 90.f * this->direction;
			else
				hammerParams[0] -= 45.f * this->direction;
			this->playSFX(0);
			this->collisionType = COLLISION_TYPE_HIT;
			this->_hammerPickTimer = 20;
			this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
			this->_hammer->gravity.y = 0;
			this->_hammer->skillIndex = 7;
			this->_hammer->collisionLimit = 1;
			this->_hammer->collisionType = COLLISION_TYPE_NONE;
			this->_hammer->HP = 1000 + 250 * this->effectiveSkillLevel[7];
			this->_hammer->setSequence(11);
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
		}
		break;

	case ACTION_ja1_22B_HAMMER:
	case ACTION_ja1_22C_HAMMER:
		if (this->frameState.sequenceId == 2) {
			this->applyGroundMechanics();
			if (this->advanceFrame())
				this->setAction(SokuLib::ACTION_FALLING);
			break;
		}
		this->advanceFrame();
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId != 0) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		if (this->applyAirMechanics()) {
			this->setSequence(2);
			this->position.y = this->getGroundHeight();
			this->resetForces();
			break;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			float hammerParams[5] = {-90.f, 25, 0, 1, 0};

			if (this->frameState.actionId % 2 == 0)
				hammerParams[0] += 90.f * this->direction;
			else
				hammerParams[0] += 45.f * this->direction;
			this->playSFX(0);
			this->collisionType = COLLISION_TYPE_HIT;
			this->_hammerPickTimer = 20;
			this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
			this->_hammer->gravity.y = 0;
			this->_hammer->skillIndex = 7;
			this->_hammer->collisionLimit = 1;
			this->_hammer->collisionType = COLLISION_TYPE_NONE;
			this->_hammer->setSequence(11);
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
		}
		if (this->frameState.poseId >= 3)
			this->speed -= this->gravity;
		else if (std::abs(this->speed.y) > 3)
			this->speed.y = std::copysign(3, this->speed.y);
		break;

	case ACTION_a2_623B:
	case ACTION_a2_623C:
	case ACTION_a2_623B_HAMMER:
	case ACTION_a2_623C_HAMMER:
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			break;
		}
		if (this->frameState.sequenceId == 1) {
			this->applyGroundMechanics();
			if (this->frameState.currentFrame >= 20)
				this->nextSequence();
			if (this->collisionType == COLLISION_TYPE_INVUL) {
				this->_cropLimit = 0;
				this->collisionType = COLLISION_TYPE_NONE;
				this->gameData.opponent->hitStop = 15 + (this->effectiveSkillLevel[8] * 5);
				this->setSequence(4);
				this->playSFX(20);
				this->createObject(856, this->position.x, this->position.y, this->direction, 1)->skillIndex = 8;
			}
		} else if (this->frameState.sequenceId == 2) {
			this->applyGroundMechanics();
			if (this->frameState.currentFrame >= 20)
				this->nextSequence();
		} else if (this->frameState.sequenceId == 4) {
			this->_hammerPickTimer = 2;
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 0)
				this->setAction(SokuLib::ACTION_IDLE);
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 1) {
				this->position.x = this->gameData.opponent->position.x + 150 * this->direction;
				this->direction = -this->direction;
			}
			if (this->_hammer) {
				if (this->frameState.poseId == 2 || this->frameState.poseId == 3) {
					this->speed.y = 10;
					this->gravity.y = 0.5;
				} else
					this->speed.y -= this->gravity.y;
				if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
					this->speed.x = 5;
					SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
				}
				if (this->frameState.poseId > 4)
					this->speed.x -= 0.1;
			} else {
				if (this->frameState.poseFrame == 0 && this->frameState.poseId == 2) {
					this->speed = {0, 20};
					this->gravity.y = HAMMER_FALLING_GRAVITY;
				}
				if (this->frameState.poseFrame == 0 && this->frameState.poseId == 5) {
					SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
					this->speed = {-4, 10};
				}
				this->speed.y -= this->gravity.y;
			}
			if (this->_cropLimit < 400 && this->frameState.poseId != 0) {
				this->_offset = 20;
				this->_cropLimit += 20;
			} else if (this->_offset)
				this->_offset--;
		} else
			this->applyGroundMechanics();
		break;

	case ACTION_a2_236B:
	case ACTION_a2_236C:
	case ACTION_a2_236B_HAMMER:
	case ACTION_a2_236C_HAMMER:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.currentFrame == 20) {
			if (this->_hammer)
				this->createObject(857, this->gameData.opponent->position.x - (100 + 200 * (this->frameState.actionId % 2)) * this->direction, this->gameData.opponent->getGroundHeight(), this->direction, 1)->skillIndex = 9;
			else
				this->createObject(858, this->gameData.opponent->position.x - (100 * (this->frameState.actionId % 2)) * this->direction, this->gameData.opponent->getGroundHeight(), this->direction, 1)->skillIndex = 9;
			this->collisionType = COLLISION_TYPE_HIT;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.currentFrame == 40)
			this->nextSequence();
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 30)
			this->nextSequence();
		break;

	case ACTION_a2_214B:
	case ACTION_a2_214C:
	case ACTION_a2_214B_HAMMER:
	case ACTION_a2_214C_HAMMER: {
		int mod[] = {7, 5, 4, 3};

		if (this->_checkDashSlide() || (this->frameState.sequenceId == 1 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0))
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.poseId >= 3 && this->frameState.poseId <= 5 && this->frameState.currentFrame % mod[min(this->effectiveSkillLevel[10] - 1, 3)] == 0) {
			this->collisionType = COLLISION_TYPE_HIT;
			if (this->_nbBambooSpawned == 0)
				this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->playSFX(1);
			this->createObject(
				859 + (this->frameState.actionId % 2),
				this->position.x + (50 + this->_nbBambooSpawned * (70 - 10 * this->effectiveSkillLevel[10])) * this->direction / (this->frameState.actionId % 2 + 1),
				this->position.y, this->direction, 1
			)->skillIndex = 10;
			this->_nbBambooSpawned++;
		}
		break;
	}

	case ACTION_a2_22B:
	case ACTION_ja2_22B:
		if (this->advanceFrame())
			this->setAction(this->gameData.frameData->frameFlags.airborne ? SokuLib::ACTION_FALLING : SokuLib::ACTION_IDLE);
		if (this->isGrounded())
			this->applyGroundMechanics();
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
			this->playSFX(20);
			this->createObject(861, this->gameData.opponent->position.x, this->gameData.opponent->position.y + 250, this->direction, 1);
			this->createObject(861, this->_hammer->position.x, this->_hammer->position.y, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->_hammer->collisionLimit = 0;
			this->_hammer->position.x = this->gameData.opponent->position.x;
			this->_hammer->position.y = this->gameData.opponent->position.y + 250;
			this->_hammer->setSequence(0);
			this->_hammer->speed.x = 0;
			this->_hammer->speed.y = 0;
			this->_hammer->gravity.y = 0.5;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame >= 20 - this->effectiveSkillLevel[11] * 4)
			this->nextSequence();
		if (this->frameState.sequenceId == 2)
			this->speed.y -= this->gravity.y;
		break;

	case ACTION_a2_22C:
	case ACTION_ja2_22C:
		if (this->advanceFrame())
			this->setAction(this->gameData.frameData->frameFlags.airborne ? SokuLib::ACTION_FALLING : SokuLib::ACTION_IDLE);
		if (this->isGrounded())
			this->applyGroundMechanics();
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
			this->playSFX(20);
			this->createObject(861, this->position.x, this->position.y, this->direction, 1);
			this->createObject(861, this->_hammer->position.x, this->_hammer->position.y, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->position = this->_hammer->position;
			if (this->position.x < this->gameData.opponent->position.x)
				this->direction = SokuLib::RIGHT;
			if (this->position.x > this->gameData.opponent->position.x)
				this->direction = SokuLib::LEFT;
			if (this->position.y == this->getGroundHeight())
				this->setActionSequence(ACTION_a2_22C, 1);
			else
				this->setActionSequence(ACTION_ja2_22C, 1);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame >= 24 - this->effectiveSkillLevel[11] * 4)
			this->nextSequence();
		if (this->frameState.sequenceId == 2)
			this->speed.y -= this->gravity.y;
		break;

	case ACTION_a2_22B_HAMMER:
	case ACTION_ja2_22B_HAMMER:
	case ACTION_a2_22C_HAMMER:
	case ACTION_ja2_22C_HAMMER:
		if (this->advanceFrame())
			this->setAction(this->gameData.frameData->frameFlags.airborne ? SokuLib::ACTION_FALLING : SokuLib::ACTION_IDLE);
		if (this->isGrounded())
			this->applyGroundMechanics();
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 1 && this->frameState.poseFrame == 0) {
			this->playSFX(20);
			this->createObject(861, this->position.x, this->position.y, this->direction, 1);
			this->createObject(861, this->position.x + 200 * this->direction * (this->frameState.actionId % 2 == 0 ? 1 : -1), this->position.y, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			this->position.x += 200 * this->direction * (this->frameState.actionId % 2 == 0 ? 1 : -1);
			if (this->position.x < this->gameData.opponent->position.x)
				this->direction = SokuLib::RIGHT;
			if (this->position.x > this->gameData.opponent->position.x)
				this->direction = SokuLib::LEFT;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame >= (this->frameState.actionId % 2) * 8 + 24 - this->effectiveSkillLevel[11] * 4)
			this->nextSequence();
		if (this->frameState.sequenceId == 2)
			this->speed.y -= this->gravity.y;
		break;

	case ACTION_USING_SC_ID_200_HAMMER:
	case SokuLib::ACTION_USING_SC_ID_200:
		this->applyGroundMechanics();
		if (this->frameState.sequenceId != 0)
			this->_hammerPickTimer = 2;
		if (this->frameState.sequenceId == 0) {
			this->advanceFrame();
			if (this->frameState.sequenceId == 1)
				this->setAction(SokuLib::ACTION_IDLE);
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
				this->spellStopCounter = 40;
				this->playSpellBackground(0, 60);
				SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
				this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
				this->consumeCard();
				this->eventSpellUse();
				this->eventWeatherCycle();
			}
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 4)
				this->createObject(848, this->position.x, this->position.y, this->direction, 1);
			if (this->collisionType == COLLISION_TYPE_HIT || this->collisionType == COLLISION_TYPE_ARMORED) {
				this->gameData.opponent->setAction(SokuLib::ACTION_GRABBED);
				this->gameData.opponent->untech = 300;
				this->gameData.opponent->speed.x = 0;
				this->gameData.opponent->speed.y = 0;
				this->gameData.opponent->position.x = this->position.x + 30 * this->direction;
				this->gameData.opponent->position.y = this->position.y;
				this->collisionType = COLLISION_TYPE_NONE;
				this->hasMoveBeenReset = true;
				this->collisionLimit = 1;
				this->nextSequence();
			}
		} else if (this->frameState.sequenceId == 1) {
			this->advanceFrame();
			this->spellBgTimer = 30;
			if (this->_hammer && this->frameState.sequenceId == 2) {
				this->_hammer->setSequence(6);
				this->_hammer->customData[4] = 1;
			}
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 5) {
				this->gameData.opponent->setAction(SokuLib::ACTION_AIR_HIT_BIG_HITSTUN);
				this->gameData.opponent->untech = 300;
				this->gameData.opponent->speed.x = 0;
				this->gameData.opponent->speed.y = 45;
				this->gameData.opponent->gravity.y = 0.5;
				SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
			}
		} else if (this->frameState.sequenceId == 2) {
			if (!this->_hammer) {
				this->nextSequence();
				break;
			}
			this->spellBgTimer = 30;
			this->advanceFrame();
			if (this->frameState.currentFrame % 10 == 0) {
				float params[2] = {
					static_cast<float>(SokuLib::rand(360)),
					static_cast<float>(SokuLib::rand(150) + 250)
				};

				this->createObject(849, this->position.x, this->position.y, this->direction, 1, params);
			}
			if (std::abs(this->_hammer->position.x - this->position.x) > 100)
				break;
			if (this->_hammer->position.y + 50 < this->position.y)
				break;
			if (this->_hammer->position.y - 200 > this->position.y)
				break;
			this->_hammer->lifetime = 0;
			this->_hammer = nullptr;
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SLAP_HIT);
			this->nextSequence();
		} else if (this->frameState.sequenceId == 3) {
			this->advanceFrame();
			this->spellBgTimer = 30;
			if (this->frameState.poseId != this->frameState.sequenceSize - 1)
				break;
			if (
				this->inputData.keyInput.a == 1 ||
				this->inputData.keyInput.b == 1 ||
				this->inputData.keyInput.c == 1 ||
				this->inputData.keyInput.d == 1 ||
				this->inputData.keyInput.spellcard == 1
			)
				this->nextSequence();
		} else if (this->frameState.sequenceId == 4) {
			if (this->advanceFrame())
				this->setAction(SokuLib::ACTION_IDLE);
			this->spellBgTimer = 5;
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 2)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		}
		break;

	case SokuLib::ACTION_USING_SC_ID_201:
	case ACTION_USING_SC_ID_201_HAMMER:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			this->spellStopCounter = 40;
			this->playSpellBackground(5, 180);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame < 120 && this->frameState.currentFrame % 2 == 0) {
				float rabbitData[3] = {5.f + SokuLib::rand(15), 5.f + SokuLib::rand(15), 0};
				float x = 640 - this->direction * 640;

				if (SokuLib::rand(2) != 0) {
					auto rabbit = this->createObject(805, x, SokuLib::rand(400), this->direction, 1,rabbitData);

					rabbit->setPose(4);
					rabbit->speed.x = rabbit->customData[0];
					rabbit->speed.y = rabbit->customData[1];
				} else
					this->createObject(805, x, 0, this->direction, 1, rabbitData);
			}
			if (this->frameState.currentFrame >= 120)
				this->nextSequence();
		}
		break;

	case SokuLib::ACTION_SC_ID_201_ALT_EFFECT:
	case ACTION_USING_SC_ID_201_HAMMER_AIR:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			this->spellStopCounter = 40;
			this->playSpellBackground(5, 180);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame < 120 && this->frameState.currentFrame % 2 == 0) {
				float rabbitData[3] = {5.f + SokuLib::rand(15), 5.f + SokuLib::rand(15), 1};
				float x = 640 - this->direction * 640;

				if (SokuLib::rand(2) != 0) {
					auto rabbit = this->createObject(805, x, SokuLib::rand(400), this->direction, 1,rabbitData);

					rabbit->setPose(4);
					rabbit->speed.x = rabbit->customData[0];
					rabbit->speed.y = rabbit->customData[1];
				} else
					this->createObject(805, x, 0, this->direction, 1, rabbitData);
			}
			if (this->frameState.currentFrame >= 120)
				this->nextSequence();
		}
		break;

	case SokuLib::ACTION_USING_SC_ID_202:
	case ACTION_USING_SC_ID_202_HAMMER:
	case SokuLib::ACTION_USING_SC_ID_203:
	case ACTION_USING_SC_ID_203_HAMMER:
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
			if (this->frameState.poseId == 2) {
				this->spellStopCounter = 40;
				this->playSpellBackground(6, 60);
				SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
				this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
				this->consumeCard();
				this->eventSpellUse();
				this->eventWeatherCycle();
			}
			if (this->frameState.poseId <= 10) {
				this->_tmpObject->position = this->position;
				this->_tmpObject->position.x += bottleOffsets[this->frameState.poseId - 1].x * this->renderInfos.scale.x * this->direction;
				this->_tmpObject->position.y += bottleOffsets[this->frameState.poseId - 1].y * this->renderInfos.scale.y;
			}
			if (this->frameState.poseId == 10) {
				this->_tmpObject->speed.x = 20 * this->direction;
				this->_tmpObject->speed.y = 10;
				this->_tmpObject->gravity.y = 1;
				this->_tmpObject->collisionLimit = 1;
				this->_tmpObject->position -= this->_tmpObject->speed;
			}
		}
		if (this->frameState.poseId < 10) {
			this->_tmpObject->renderInfos.zRotation = 0;
			this->_tmpObject->frameState.currentFrame = 0;
		}
		break;

	case ACTION_USING_SC_ID_204_HAMMER:
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2) {
			this->spellStopCounter = 40;
			this->playSpellBackground(4, 60);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 5) {
			float hammerParams[5] = {-90.f + 87.f * this->direction, 55, 0, 1, 0};

			this->playSFX(0);
			this->_hammerPickTimer = 20;
			this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
			this->_hammer->gravity.y = 0;
			this->_hammer->collisionLimit = 1;
			this->_hammer->collisionType = COLLISION_TYPE_NONE;
			this->_hammer->setSequence(9);
		}
		break;
	case ACTION_USING_SC_ID_204_HAMMER_AIR:
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2) {
			this->spellStopCounter = 40;
			this->playSpellBackground(4, 60);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 4) {
			float hammerParams[5] = {-90.f + 85.f * this->direction, 55, 0, 1, 0};

			this->playSFX(0);
			this->_hammerPickTimer = 20;
			this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
			this->_hammer->gravity.y = 0;
			this->_hammer->collisionLimit = 1;
			this->_hammer->collisionType = COLLISION_TYPE_NONE;
			this->_hammer->setSequence(9);
		}
		break;

	case SokuLib::ACTION_USING_SC_ID_205:
	case ACTION_USING_SC_ID_205_HAMMER:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		this->applyGroundMechanics();
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
			this->spellStopCounter = 60;
			this->playSpellBackground(1, 180);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, -1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame == 0) {
				this->createObject(808, this->position.x + 200 * this->direction, this->position.y, this->direction, -1);
				this->playSFX(3);
			}
			if (this->frameState.currentFrame == 120)
				this->nextSequence();
		}
		break;

	case SokuLib::ACTION_USING_SC_ID_206:
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		if (this->frameState.sequenceId == 0) {
			this->applyGroundMechanics();
			if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
				this->spellStopCounter = 40;
				this->playSpellBackground(2, 60);
				SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
				this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, -1);
				this->consumeCard();
				this->eventSpellUse();
				this->eventWeatherCycle();
			}
			if (this->timeStop)
				this->position.x -= this->speed.x * this->direction;
			else if (this->speed.x > 0)
				this->speed.x -= 0.5;
			if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			if (this->frameState.poseId == 6 && this->frameState.poseFrame == 0)
				this->createObject(852, 0, 0, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame == 0) {
				this->setAction(SokuLib::ACTION_IDLE);
				return;
			}
			this->speed = {0, 0};
			if (this->frameState.poseId <= 0) {
				this->lockedInStageX = false;
				this->position.x = this->gameData.opponent->position.x - 50 * this->direction;
				SokuLib::camera.forcedXCenter = this->gameData.opponent->position.x;
			} else {
				if (this->frameState.poseFrame == 0 && this->frameState.poseId == 1)
					this->position.y = 50;
				if (this->frameState.poseId >= 4)
					this->speed -= this->gravity;
				if (this->applyAirMechanics()) {
					this->setAction(SokuLib::ACTION_IDLE);
					this->position.y = this->getGroundHeight();
					this->resetForces();
					break;
				}
				if (this->frameState.poseFrame == 0 && this->frameState.poseId == 2)
					this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
				if (this->frameState.poseFrame == 0 && this->frameState.poseId == 4) {
					SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
					this->speed.y = 9;
					this->gravity.y = 1;
				}
				if (this->renderInfos.color.a > 225)
					this->renderInfos.color.a = 255;
				else
					this->renderInfos.color.a += 30;
			}
			if (this->frameState.poseId == 1 && this->frameState.poseFrame == 0) {
				SokuLib::camera.forceScale = false;
				SokuLib::camera.forceXCenter = false;
				SokuLib::camera.forceYCenter = false;
				this->lockedInStageX = true;
				this->direction = -this->direction;
				this->position.x = this->gameData.opponent->position.x - 500 * this->direction;
				this->position.y = this->getGroundHeight();
			}
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 5)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		}
		break;

	case ACTION_USING_SC_ID_206_HAMMER:
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0) {
			this->applyGroundMechanics();
			if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
				this->spellStopCounter = 40;
				this->playSpellBackground(2, 60);
				SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
				this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, -1);
				this->consumeCard();
				this->eventSpellUse();
				this->eventWeatherCycle();
			}
			if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			if (this->frameState.poseId == 7 && this->frameState.poseFrame == 0)
				this->createObject(851, 0, 0, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame == 0) {
				this->setAction(SokuLib::ACTION_IDLE);
				return;
			}
			this->speed = {0, 0};
			if (this->frameState.poseId <= 0) {
				this->lockedInStageX = false;
				this->position.x = this->gameData.opponent->position.x - 50 * this->direction;
				SokuLib::camera.forcedXCenter = this->gameData.opponent->position.x;
			} else {
				this->applyGroundMechanics();
				if (this->renderInfos.color.a > 225)
					this->renderInfos.color.a = 255;
				else
					this->renderInfos.color.a += 30;
			}
			if (this->frameState.poseId == 1 && this->frameState.poseFrame == 0) {
				SokuLib::camera.forceScale = false;
				SokuLib::camera.forceXCenter = false;
				SokuLib::camera.forceYCenter = false;
				this->lockedInStageX = true;
				this->direction = -this->direction;
				this->position.x = this->gameData.opponent->position.x - 500 * this->direction;
				this->position.y = this->getGroundHeight();
			}
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 5)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		}
		break;


	case SokuLib::ACTION_USING_SC_ID_207:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			this->spellStopCounter = 40;
			this->_hammerPickTimer = 60;
			this->playSpellBackground(6, 60);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame == 0)
				this->createObject(850, this->position.x, this->position.y, this->direction, 1);
			if (this->frameState.currentFrame >= 10)
				this->nextSequence();
		}
		break;

	case SokuLib::ACTION_SC_ID_207_ALT_EFFECT:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			this->spellStopCounter = 40;
			this->_hammerPickTimer = 60;
			this->playSpellBackground(6, 60);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame == 0)
				this->createObject(850, this->position.x, this->position.y, this->direction, 1);
			if (this->frameState.currentFrame >= 10)
				this->nextSequence();
		}
		break;

	case SokuLib::ACTION_USING_SC_ID_208:
	case SokuLib::ACTION_SC_ID_208_ALT_EFFECT: {
		bool b = this->frameState.sequenceId == 0;

		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 5 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0)
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 5) {
			this->applyGroundMechanics();
			this->resetForces();
			this->resetRenderInfo();
			break;
		}
		if (!this->gameData.frameData->frameFlags.airborne)
			this->applyGroundMechanics();
		else if (this->frameState.sequenceId != 1 && this->applyAirMechanics()) {
			if (this->frameState.sequenceId == 1) {
				this->speed = {-4, 10};
				this->gravity.y = FALLING_GRAVITY;
				this->nextSequence();
			} else
				this->setSequence(5);
			return;
		}
		if (this->frameState.sequenceId != 1)
			this->speed.y -= this->gravity.y;
		if (this->frameState.sequenceId == 1) {
			if (b) {
				this->spellStopCounter = 40;
				this->playSpellBackground(3, 120);
				SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
				this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
				this->consumeCard();
				this->eventSpellUse();
				this->eventWeatherCycle();
			}
			if (this->spellStopCounter == 0 && this->timeStop == 0) {
				if (this->inputData.keyInput.verticalAxis > 0)
					this->speed.y = -30;
				else if (this->inputData.keyInput.verticalAxis < 0)
					this->speed.y = 30;
				if (this->inputData.keyInput.horizontalAxis > 0)
					this->speed.x = 30 * this->direction;
				else if (this->inputData.keyInput.horizontalAxis < 0)
					this->speed.x = -30 * this->direction;
				if (this->speed.x != 0 && this->speed.y != 0) {
					this->speed.x /= std::sqrt(2);
					this->speed.y /= std::sqrt(2);
				}
				if (this->speed.x == 0 && this->speed.y == 0) {
					auto angle = fmod(atan2(this->gameData.opponent->position.y + 50 - this->position.y, this->gameData.opponent->position.x - this->position.x), M_PI * 2);

					this->speed.x = std::cos(angle) * 30 * this->direction;
					this->speed.y = std::sin(angle) * 30;
				}
				this->_oldSpeed = this->speed;
				this->nextSequence();
				this->collisionLimit = 8;
				this->collisionType = COLLISION_TYPE_NONE;
				this->hasMoveBeenReset = true;
				SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
			}
			this->center = {0, 78};
			this->renderInfos.zRotation = 180 * this->frameState.poseFrame / this->frameState.poseDuration;
		} else if (this->frameState.sequenceId == 2) {
			if (this->collisionType != COLLISION_TYPE_NONE && (this->speed.x > 4 || this->speed.y > 4)) {
				this->speed.x /= 20;
				this->speed.y /= 20;
			}
			if (
				(this->collisionLimit == 0 && !this->spellStopCounter) ||
				(this->position.x <= 40 && this->speed.x * this->direction < 0) ||
				(this->position.x >= 1240 && this->speed.x * this->direction > 0) ||
				(this->position.y <= this->getGroundHeight() && this->speed.y < 0) ||
				(this->position.y >= 1000 && this->speed.y > 0)
			) {
				this->nextSequence();
				this->resetRenderInfo();
				this->center = {0, 0};
				this->speed = {-10, 15};
				this->gravity.y = FALLING_GRAVITY;
			} else if (this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
				if (this->frameState.currentFrame % 8 == 0 || this->collisionType)
					SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
				this->collisionType = COLLISION_TYPE_NONE;
				this->hasMoveBeenReset = true;
			}
			if (this->collisionType == COLLISION_TYPE_NONE)
				this->speed = this->_oldSpeed;
			this->center = {0, 78};
			this->renderInfos.zRotation = 180 * this->frameState.poseFrame / this->frameState.poseDuration;
		}
		if (this->frameState.sequenceId == 3 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
			this->speed = {-10, 15};
			this->gravity.y = FALLING_GRAVITY;
		}
		break;
	}


	case ACTION_USING_SC_ID_208_HAMMER:
	case ACTION_USING_SC_ID_208_HAMMER_AIR: {
		bool b = this->frameState.sequenceId == 0;

		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 4 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0)
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 4) {
			this->applyGroundMechanics();
			this->resetForces();
			this->resetRenderInfo();
			break;
		}
		if (this->applyAirMechanics() && (this->frameState.sequenceId == 3 || (this->frameState.sequenceId == 2 && this->frameState.poseId > 1))) {
			this->setSequence(4);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 4) {
			if (this->frameState.poseFrame == 0) {
				this->_oldSpeed = this->speed;
				this->speed = {0, 0};
				this->spellStopCounter = 40;
				this->playSpellBackground(3, 120);
				SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
				this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
				this->consumeCard();
				this->eventSpellUse();
				this->eventWeatherCycle();
			}
		} else if (this->frameState.sequenceId == 0 && this->frameState.poseId == 5) {
			this->speed = this->_oldSpeed;
			this->speed.y -= this->gravity.y;
		} else if (this->frameState.sequenceId <= 1) {
			if (this->inputData.keyInput.verticalAxis > 0) {
				this->gravity.y += 0.01;
				if (this->gravity.y > 0.8)
					this->gravity.y = 0.8;
			} else if (this->inputData.keyInput.verticalAxis < 0) {
				this->gravity.y -= 0.01;
				if (this->gravity.y < 0.1)
					this->gravity.y = 0.1;
			}
			if (this->inputData.keyInput.horizontalAxis > 0) {
				this->speed.x += 0.2;
				if (this->speed.x > 5)
					this->speed.x = 5;
			} else if (this->inputData.keyInput.horizontalAxis < 0) {
				this->speed.x -= 0.2;
				if (this->speed.x < 5)
					this->speed.x = 5;
			}
			this->speed.y -= this->gravity.y;
		} else
			this->speed.y -= this->gravity.y;
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
				if (this->collisionType == COLLISION_TYPE_NONE && !b)
					this->collisionLimit--;
				if (this->collisionLimit == 0) {
					this->nextSequence();
					this->resetRenderInfo();
					this->center = {0, 0};
					if (this->collisionType != COLLISION_TYPE_NONE) {
						this->collisionLimit = 1;
						this->collisionType = COLLISION_TYPE_NONE;
						this->hasMoveBeenReset = true;
					} else {
						this->setPose(2);
						this->speed = {-4, 10};
						this->gravity.y = HAMMER_FALLING_GRAVITY;
					}
				} else {
					if (this->frameState.currentFrame % 8 == 0 || this->collisionType)
						SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
					this->collisionType = COLLISION_TYPE_NONE;
					this->hasMoveBeenReset = true;
				}
			}
			if (this->frameState.sequenceId == 1) {
				this->center = {0, 78};
				this->renderInfos.zRotation = 180 * this->frameState.poseFrame / this->frameState.poseDuration;
			}
		}
		if (this->frameState.sequenceId == 2 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
			this->speed = {0, 0};
			this->gravity.y = 0;
		}
		if (this->frameState.sequenceId == 2 && this->frameState.poseId == 1 && this->frameState.poseFrame == 0) {
			this->speed = {-4, 10};
			this->gravity.y = HAMMER_FALLING_GRAVITY;
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		}
		break;
	}

	case ACTION_USING_SC_ID_210_HAMMER:
	case SokuLib::ACTION_USING_SC_ID_210: {
		const unsigned probs[21] = {
			1, // Reimu - Gohei comes flying straight into the center of the screen or sides of the wall, creates a vertical or sideways wave of energy (think her 4card vertical amulet thingie)
			1, // Marisa - Her bomb falls onto the screen from the sides and jumps around, eventually doing a big explosion
			1, // Sakuya - Sakuya Richochet Spellcard (the 4card one, the "bad one") suddenly appears out of nowhere. The attack proceeds as you would expect the actual spellcard
			1, // Alice - Alice's dolls comeout of one of the sides. Fundamentally a weaker version of the lemmings spellcard or Tewi rabbits attack
			1, // Patchouli - A book (do we even have the graphic) falls from the center sky of the screen. Once it touches the ground it generates a weaker Royal Flare or another spell you prefer to use
			1, // Youmu - Her sword (shouldn't be hard to get a sprite) comes from somewhere offscreen, aiming towards the opponent, lodging itself somewhere, once it touches a wall or the ground it will emit a wisp effect and then explode in a similar vein to Youmu's 22 Wicked Soul (quite strong for pressure)
			1, // Remilia - Her lance comes from the ceiling or the sides at really high velocity. Does 2orbs blocked minimum and solid damage
			1, // Yuyuko - Butterflies or wisps come from sides of the screen / alternatively, sphere bloom from fucking nowhere just kind of happens
			1, // Yukari - Yukari comes out of a Border spinning. Its literally just her actual move, hitbox and AUB and whatnot
			1, // Suika - Either a rock falls from the sky or it comes diagonally from upperleft/right of the screen. It's a Type 3 Projectile, can't be grazed
			1, // Reisen - Fullscreen Laser, reusing that idea. That or she appears out of nowhere to assist Tewi
			1, // Aya - CROWS ALL OVER THE ALL OVER THE STAGE, CALL ME THE CROWS MAN
			1, // Komachi - Scythes comes flying from somewhere, lodges itself, calls out wisps or the slow walking effect that Komachi can do
			1, // Iku - 2 balls of electricity
			1, // Tenshi - Activates current weather (or force it back to 999) and give the weather debuff to opponent + spawn boomerang
			1, // Sanae - Good/Bad luck amulets randomized fall from the sky. This should hit tewi too
			0, // Cirno - A huge block of ice falls from the sky, for a while the entire arena has the "ice frog" effect. Gotta have funnies too
			0, // Meiling - Arena going through the entire stage
			0, // Utsuho - The arena becomes orangey/yellowish and then fire geysers come out from underground
			0, // Suwako - Rings out of nowhere bouncing all over. Reusing spells once again
			0, // Namazu - The meteor thing
		};

		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 5 && this->frameState.poseFrame == 0) {
			this->spellStopCounter = 40;
			this->playSpellBackground(4, 30);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
			unsigned total = 0;
			int character = 0;

			if (__practiceEffect == -1) {
				this->collisionType = COLLISION_TYPE_HIT;
				for (unsigned int prob: probs)
					total += prob;
				total = SokuLib::rand(total);

				while (character < sizeof(probs)) {
					unsigned prob = probs[character];

					if (total < prob && prob)
						break;
					character++;
					total -= prob;
				}
				if (character == sizeof(probs))
					character--;
			} else
				character = __practiceEffect;
			this->createObject(810 + character, 0, 0, SokuLib::RIGHT, 1);
		}
		break;
	}

	case SokuLib::ACTION_USING_SC_ID_211:
	case ACTION_USING_SC_ID_211_HAMMER:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 1 && this->frameState.poseFrame == 0) {
			this->spellStopCounter = 40;
			this->playSpellBackground(0, 60);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_BOMB);
			this->createEffect(140, this->position.x, this->position.y + 100.f, this->direction, 1);
			this->createEffect(140, this->position.x, this->position.y + 100.f, this->direction, 1);
			this->createEffect(140, this->position.x, this->position.y + 100.f, this->direction, 1);
			this->createEffect(141, this->position.x, this->position.y + 100.f, this->direction, -1);
			this->createEffect(142, this->position.x, this->position.y, this->direction, -1);
		}
		break;

	case SokuLib::ACTION_TALISMAN:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId != 0 || this->frameState.poseFrame != 0 || this->frameState.poseId != 6)
			return;
		SokuLib::playSEWaveBuffer(SokuLib::SFX_BOMB);
		this->createEffect(140, this->position.x, this->position.y + 100.f, this->direction, 1);
		this->createEffect(140, this->position.x, this->position.y + 100.f, this->direction, 1);
		this->createEffect(140, this->position.x, this->position.y + 100.f, this->direction, 1);
		this->createEffect(141, this->position.x, this->position.y + 100.f, this->direction, -1);
		this->createEffect(142, this->position.x, this->position.y,         this->direction, -1);
		return;

	case ACTION_HANGEKI_HAMMER:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		break;

	case SokuLib::ACTION_HANGEKI:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->speed.x > 0)
			this->speed.x -= 1;
		if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0)
			this->speed.x = 10;
		if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		break;

	case ACTION_LEFT_HANDED_FOLDING_FAN_HAMMER:
	case ACTION_SPELL_BREAKING_DRUG_HAMMER:
		this->frameState.actionId -= 3;
		this->updateDefaultBehavior();
		this->frameState.actionId += 3;
		break;

	default: {
		int old = this->frameState.poseFrame;

		this->updateDefaultBehavior();
		// Fail-safe to prevent softlocks
		if (old == this->frameState.poseFrame && this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
	}}
}

bool Tewi::setAction(short action)
{
	printf("Tewi::setAction(%i)\n", action);
	if (action < SokuLib::ACTION_5A) {
		short realAction = action;

		if (this->_hammer) {
			auto it = systemToHammer.find(action);

			if (it != systemToHammer.end())
				action = it->second;
		}

		bool result = GameObjectBase::setAction(action);

		if (this->frameState.actionId == action)
			this->frameState.actionId = realAction;
		return result;
	} else {
		if (this->_hammer) {
			if (action == SokuLib::ACTION_5C)
				action = SokuLib::ACTION_2B;
			if (action == SokuLib::ACTION_j5C)
				action = SokuLib::ACTION_j2B;
		} else {
			auto it = moveToHammer.find(action);

			if (it != moveToHammer.end())
				action = it->second;
		}
		return GameObjectBase::setAction(action);
	}
}

void Tewi::initializeAction()
{
	auto it = hammerToSystem.find(this->frameState.actionId);

	if (it != hammerToSystem.end())
		this->frameState.actionId = it->second;
	switch (this->frameState.actionId) {
		break;
	case SokuLib::ACTION_WALK_FORWARD:
		this->speed.x = this->_hammer ? FRONT_WALK_SPEED : HAMMER_FRONT_WALK_SPEED;;
		break;
	case SokuLib::ACTION_WALK_BACKWARD:
		this->speed.x = this->_hammer ? BACK_WALK_SPEED : HAMMER_BACK_WALK_SPEED;
		break;
	case SokuLib::ACTION_FLY:
		this->resetRenderInfo();
		this->speed.x = 0.0;
		this->flightTimer = 0;
		this->speed.y = 0.0;
		this->gravity.y = 0.6;
		this->center.x = 0.0;
		this->center.y = 95.0;
		this->flightSpeed = FLIGHT_SPEED;
		this->unknown7EC = 0;
		break;
	case ACTION_STAND_PICKUP_HAMMER_FROM_AIR:
	case ACTION_STAND_PICKUP_HAMMER_FROM_GROUND:
		this->speed.x = 0.0;
	case ACTION_AIR_PICKUP_HAMMER_FROM_AIR:
	case ACTION_AIR_PICKUP_HAMMER_FROM_GROUND:
		if (this->gravity.y < 0.1)
			this->gravity.y = FALLING_GRAVITY;
		this->resetRenderInfo();
		this->_hammer->lifetime = 0;
		this->_hammer = nullptr;
		break;
	case ACTION_5AA_HAMMER:
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 2;
		this->speed.x = 0.0;
		break;
	case SokuLib::ACTION_f5A:
		this->speed.x = 8;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_5AAA:
		this->speed.x = 10;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_66A:
	case SokuLib::ACTION_66B:
	case SokuLib::ACTION_66C:
	case SokuLib::ACTION_5AAAA:
	case ACTION_5AAA6A:
	case SokuLib::ACTION_5AAA3A:
	case SokuLib::ACTION_USING_SC_ID_206:
		this->speed.x = 14;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case ACTION_66A_HAMMER:
		this->speed.x = 10;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case ACTION_66B_HAMMER:
		this->speed.x = 10;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 7;
		break;
	case ACTION_66C_HAMMER:
		this->speed.x = 10;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_j8A:
		this->chargedAttack = true;
		this->speed.x = 0;
		this->gravity.y = FALLING_GRAVITY / 2;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 2;
		break;
	case SokuLib::ACTION_j2A:
		this->chargedAttack = true;
	case SokuLib::ACTION_j6A:
	case SokuLib::ACTION_j5A:
		this->gravity.y = FALLING_GRAVITY;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case ACTION_d623C:
		this->speed.x = 15;
		this->speed.y = 1;
		this->gravity.y = 0.05;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case ACTION_a1_623B:
	case ACTION_a1_623C:
	case ACTION_a1_623B_HAMMER:
	case ACTION_a1_623C_HAMMER:
		this->collisionLimit = 7;
		this->speed.x = 0;
		this->speed.y = 0;
		this->gravity.y = 0;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		break;
	case ACTION_d623B:
	case ACTION_jd623B:
	case ACTION_jd623C:
		this->speed.x = 0;
		this->speed.y = 0;
		this->gravity.y = 0;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case ACTION_USING_SC_ID_204_HAMMER:
	case ACTION_USING_SC_ID_204_HAMMER_AIR:
	case SokuLib::ACTION_USING_SC_ID_211:
	case ACTION_USING_SC_ID_211_HAMMER:
	case SokuLib::ACTION_USING_SC_ID_205:
	case ACTION_USING_SC_ID_205_HAMMER:
		this->speed = {0, 0};
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_USING_SC_ID_202:
	case ACTION_USING_SC_ID_202_HAMMER: {
		float params[] = {150, 50, 0, 20, 0};

		this->_tmpObject = this->createObject(
			807,
			this->position.x + 50 * this->renderInfos.scale.x * this->direction,
			this->position.y + 115 * this->renderInfos.scale.y,
			this->direction, 1, params
		);
		this->speed = {0, 0};
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 0;
		break;
	}
	case SokuLib::ACTION_USING_SC_ID_203:
	case ACTION_USING_SC_ID_203_HAMMER: {
		float params[] = {300, 60, 0, 15, 1};

		this->_tmpObject = this->createObject(
			807,
			this->position.x + 50 * this->renderInfos.scale.x * this->direction,
			this->position.y + 115 * this->renderInfos.scale.y,
			this->direction, 1, params
		);
		this->speed = {0, 0};
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 0;
		break;
	}
	case SokuLib::ACTION_USING_SC_ID_208:
	case SokuLib::ACTION_SC_ID_208_ALT_EFFECT:
		this->speed = {0, 0};
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 0;
		this->gravity.y = 0;
		break;
	case ACTION_USING_SC_ID_208_HAMMER:
		this->speed.x = 0;
		this->speed.y = 15;
		this->gravity.y = 0.5;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 10;
		break;
	case ACTION_USING_SC_ID_208_HAMMER_AIR:
		this->speed.x = 0;
		this->speed.y = 10;
		this->gravity.y = 0.5;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 10;
		break;
	case ACTION_d623B_HAMMER:
		this->speed.x = 7.5;
		this->speed.y = 10;
		this->gravity.y = 0.3;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 2 + this->effectiveSkillLevel[0];
		break;
	case ACTION_d623C_HAMMER:
		this->speed.x = 0;
		this->speed.y = 10;
		this->gravity.y = 0.3;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 2 + this->effectiveSkillLevel[0];
		break;
	case ACTION_jd623B_HAMMER:
		this->speed.x = 7.5;
		this->speed.y = 2.5;
		this->gravity.y = 0.3;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 2 + this->effectiveSkillLevel[0];
		break;
	case ACTION_jd623C_HAMMER:
		this->speed.x = 5;
		this->speed.y = 12.5;
		this->gravity.y = 0.3;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 2 + this->effectiveSkillLevel[0];
		break;
	case ACTION_j5A_HAMMER:
	case ACTION_j8A_HAMMER:
	case ACTION_j2A_HAMMER:
	case ACTION_j6A_HAMMER:
		this->gravity.y = HAMMER_FALLING_GRAVITY;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case ACTION_jd22B:
	case ACTION_jd22C:
	case ACTION_jd22B_HAMMER:
	case ACTION_jd22C_HAMMER:
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 0;
		break;
	case ACTION_ja2_22B:
	case ACTION_ja2_22C:
		this->speed.x = 0;
		this->speed.y = 0;
		this->gravity.y = FALLING_GRAVITY;
		this->collisionType = COLLISION_TYPE_HIT;
		this->hasMoveBeenReset = true;
		break;
	case ACTION_ja2_22B_HAMMER:
	case ACTION_ja2_22C_HAMMER:
		this->speed.x = 0;
		this->speed.y = 0;
		this->gravity.y = HAMMER_FALLING_GRAVITY;
		this->collisionType = COLLISION_TYPE_HIT;
		this->hasMoveBeenReset = true;
		break;
	case ACTION_a1_22B:
	case ACTION_a1_22C:
		this->_hammer->collisionType = COLLISION_TYPE_NONE;
		this->_hammer->collisionLimit = 0;
		this->_hammer->customData[1] = 0;
		this->_hammer->setSequence(12);
		this->speed.x = 0;
		this->speed.y = 0;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case ACTION_a1_214B:
	case ACTION_a1_214B_HAMMER:
		this->chargedAttack = true;
	case ACTION_d22B:
	case ACTION_d22C:
	case ACTION_d22B_HAMMER:
	case ACTION_d22C_HAMMER:
	case ACTION_a1_22B_HAMMER:
	case ACTION_a1_22C_HAMMER:
	case ACTION_d214B:
	case ACTION_d214C:
	case ACTION_d214B_HAMMER:
	case ACTION_d214C_HAMMER:
	case SokuLib::ACTION_USING_SC_ID_200:
	case ACTION_USING_SC_ID_200_HAMMER:
	case SokuLib::ACTION_USING_SC_ID_207:
	case SokuLib::ACTION_SC_ID_207_ALT_EFFECT:
	case ACTION_USING_SC_ID_206_HAMMER:
	case SokuLib::ACTION_USING_SC_ID_201:
	case ACTION_USING_SC_ID_201_HAMMER:
	case SokuLib::ACTION_SC_ID_201_ALT_EFFECT:
	case ACTION_USING_SC_ID_201_HAMMER_AIR:
	case ACTION_a1_214C:
	case ACTION_a1_214C_HAMMER:
	case ACTION_a2_623B:
	case ACTION_a2_623C:
	case ACTION_a2_623B_HAMMER:
	case ACTION_a2_623C_HAMMER:
	case ACTION_a2_236B:
	case ACTION_a2_236C:
	case ACTION_a2_236B_HAMMER:
	case ACTION_a2_236C_HAMMER:
	case ACTION_a2_214B:
	case ACTION_a2_214C:
	case ACTION_a2_214B_HAMMER:
	case ACTION_a2_214C_HAMMER:
		this->_nbBambooSpawned = 0;
		this->speed.x = 0;
		this->speed.y = 0;
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		break;
	case ACTION_a2_22B:
	case ACTION_a2_22C:
	case ACTION_a2_22B_HAMMER:
	case ACTION_a2_22C_HAMMER:
		this->speed.x = 0;
		this->speed.y = 0;
		this->collisionType = COLLISION_TYPE_HIT;
		this->hasMoveBeenReset = true;
		break;
	case SokuLib::ACTION_6A:
	case SokuLib::ACTION_3A:
	case ACTION_3A_HAMMER:
	case ACTION_f5A_HAMMER:
	case ACTION_6A_HAMMER:
	case ACTION_5AAA_HAMMER:
		this->chargedAttack = true;
		this->speed.x = 0.0;
	case ACTION_2A_HAMMER:
	case ACTION_4A_HAMMER:
	case ACTION_5A_HAMMER:
	case SokuLib::ACTION_2A:
	case SokuLib::ACTION_4A:
	case SokuLib::ACTION_5A:
	case SokuLib::ACTION_f2A:
	case SokuLib::ACTION_5AA:
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		this->collisionLimit = 1;
		if (this->groundDashCount == 0)
			this->speed.x = 0.0;
		break;
	case ACTION_ja1_236B:
	case ACTION_ja1_236C:
	case ACTION_ja1_236B_HAMMER:
	case ACTION_ja1_236C_HAMMER:
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		if (std::abs(this->speed.x) > 2)
			this->speed.x = std::copysign(2, this->speed.x);
		if (std::abs(this->speed.y) > 2)
			this->speed.y = std::copysign(2, this->speed.y);
		break;
	case ACTION_a1_236B:
	case ACTION_a1_236C:
	case ACTION_a1_236B_HAMMER:
	case ACTION_a1_236C_HAMMER:
	case ACTION_2B_HAMMER:
	case SokuLib::ACTION_2B:
	case ACTION_5B_HAMMER:
	case SokuLib::ACTION_5B:
	case ACTION_6B_HAMMER:
	case ACTION_6B:
		this->speed = {0, 0};
		this->chargedAttack = true;
	case ACTION_d236B:
	case ACTION_d236C:
	case ACTION_d236B_HAMMER:
	case ACTION_d236C_HAMMER:
	case ACTION_jd236B:
	case ACTION_jd236C:
	case ACTION_jd236B_HAMMER:
	case ACTION_jd236C_HAMMER:
	case ACTION_4C:
	case SokuLib::ACTION_5C:
	case SokuLib::ACTION_2C:
	case SokuLib::ACTION_6C:
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		break;
	case ACTION_j2B_HAMMER:
	case SokuLib::ACTION_j2B:
	case ACTION_j5B_HAMMER:
	case SokuLib::ACTION_j5B:
		this->chargedAttack = true;
		if (std::abs(this->speed.x) > 2)
			this->speed.x = std::copysign(2, this->speed.x);
		if (std::abs(this->speed.y) > 2)
			this->speed.y = std::copysign(2, this->speed.y);
		this->collisionType = COLLISION_TYPE_NONE;
		this->hasMoveBeenReset = true;
		break;
	case ACTION_HANGEKI_HAMMER:
	case ACTION_LEFT_HANDED_FOLDING_FAN_HAMMER:
	case ACTION_SPELL_BREAKING_DRUG_HAMMER:
		this->frameState.actionId -= 3;
		Player::initializeAction();
		this->frameState.actionId += 3;
		break;

	default:
		Player::initializeAction();
		break;
	}
}

bool Tewi::_canPickUpHammer()
{
	//if (SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN <= this->frameState.actionId && this->frameState.actionId < SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN)
	if (SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN <= this->frameState.actionId && this->frameState.actionId <= SokuLib::ACTION_NEUTRAL_TECH)
		return false;
	if (this->_hammer->frameState.sequenceId == 0) {
		if (std::abs(this->_hammer->position.x - this->position.x) > 100)
			return false;
		if (this->_hammer->position.y + 50 < this->position.y)
			return false;
		if (this->_hammer->position.y - 200 > this->position.y)
			return false;
	} else if (this->_hammer->renderInfos.zRotation == 0) {
		if (std::abs(this->_hammer->position.x - this->position.x) > 75)
			return false;
		if (std::abs(this->_hammer->position.y - this->position.y) > 150)
			return false;
	} else {
		if (this->_hammer->position.y - this->position.y > 150)
			return false;
		if (this->_hammer->position.y - this->position.y < -35)
			return false;
		if (std::abs(this->_hammer->position.x - this->position.x) > 150)
			return false;
	}
	return true;
}

bool Tewi::_tryPickUpHammer()
{
	if (this->_hammerPickTimer != 0)
		return false;
	if (this->inputData.keyInput.c == 0)
		return false;
	if (this->_hammer == nullptr)
		return false;
	if (!this->_canPickUpHammer())
		return false;

	unsigned action;

	if (!this->isGrounded())
		action = ACTION_AIR_PICKUP_HAMMER_FROM_AIR;
	else
		action = ACTION_STAND_PICKUP_HAMMER_FROM_AIR;
	if (this->_hammer->frameState.sequenceId != 0)
		action++;
	this->setAction(action);
	SokuLib::playSEWaveBuffer(SokuLib::SFX_SLAP_HIT);
	return true;
}

bool Tewi::_processAAirborne()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;


	if (this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL))
		return false;

	if (this->_hammer) {
		if (
			(this->inputData.keyInput.verticalAxis < 0 || this->inputData.bufferedKeyInput.verticalAxis < 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j8A)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_j8A);
			return true;
		}
		if (
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j2A)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_j2A);
			return true;
		}

		if (
			(hKeys > 0 || hBuffKeys > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j6A)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_j6A);
			return true;
		}

		bool ok = false;
		unsigned short action = SokuLib::ACTION_j5A;

		/*switch (this->frameState.actionId) {
		case SokuLib::ACTION_j5A:
			action = SokuLib::ACTION_j5AA;
			ok = true;
			break;
		case SokuLib::ACTION_j5AA:
			return false;
		}*/

		if (this->gameData.sequenceData->actionLock <= this->getMoveLock(action) || ok) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(action);
			return true;
		}
	} else {
		if (
			(this->inputData.keyInput.verticalAxis < 0 || this->inputData.bufferedKeyInput.verticalAxis < 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_j8A_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_j8A_HAMMER);
			return true;
		}
		if (
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_j2A_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_j2A_HAMMER);
			return true;
		}

		if (
			(hKeys > 0 || hBuffKeys > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_j6A_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_j6A_HAMMER);
			return true;
		}

		if (this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_j5A_HAMMER)) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_j5A_HAMMER);
			return true;
		}
	}
	return false;
}

bool Tewi::_processBAirborne()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;

	if ((this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)) || this->currentSpirit < 200)
		return false;

	if (this->_hammer) {
		if (
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j2B)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_j2B);
			return true;
		}

		/*if (
			(hKeys > 0 || hBuffKeys > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j6B)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_j6B);
			return true;
		}*/

		if (this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j5B)) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_j5B);
			return true;
		}
	} else {
		if (
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_j2B_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_j2B_HAMMER);
			return true;
		}

		/*if (
			(hKeys > 0 || hBuffKeys > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_j6B_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_j6B_HAMMER);
			return true;
		}*/

		if (this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_j5B_HAMMER)) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_j5B_HAMMER);
			return true;
		}
	}
	return false;
}

bool Tewi::_processCAirborne()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;

	if (this->_hammer)
		return false;

	if (
		(
			this->frameState.actionId >= SokuLib::ACTION_5A &&
			(this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)
		) ||
		this->currentSpirit < 200
	)
		return false;

	if (
		(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j2C)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_j2C);
		return true;
	}

	if (
		(hKeys > 0 || hBuffKeys > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j6C)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_j6C);
		return true;
	}

	if (this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j5C)) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_j5C);
		return true;
	}
	return false;
}

bool Tewi::_useSkill(bool input, unsigned char id, unsigned short action)
{
	if (!input)
		return false;
	if (this->skilledSkillLevel[id] < 0)
		return false;
	if (this->gameData.sequenceData->actionLock >= 150)
		return false;
	if (this->gameData.sequenceData->actionLock > this->getMoveLock(action) && this->skillCancelsUsed[id % 4])
		return false;
	this->skillCancelCount++;
	this->skillCancelsUsed[id % 4] = true;
	this->renderInfos.zRotation = 0.0;
	this->eventSkillUse();
	this->useSkill(action, this->gameData.sequenceData->moveLock);
	this->skillIndex = id;
	return true;
}

bool Tewi::_processSkillsAirborne()
{
	if (
		(
			this->frameState.actionId >= SokuLib::ACTION_5A &&
			(this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)
		) ||
		this->currentSpirit < 200
	)
		return false;

	if (this->_hammer) {
		if (this->_useSkill(this->inputData.commandCombination._623b, 0, ACTION_jd623B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623c, 0, ACTION_jd623C))
			return true;

		if (this->_useSkill(this->inputData.commandCombination._236b, 1, ACTION_jd236B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236c, 1, ACTION_jd236C))
			return true;
		if (this->effectiveSkillLevel[5] >= 2 && this->_useSkill(this->inputData.commandCombination._236b, 5, ACTION_ja1_236B))
			return true;
		if (this->effectiveSkillLevel[5] >= 2 && this->_useSkill(this->inputData.commandCombination._236c, 5, ACTION_ja1_236C))
			return true;

		if (this->_useSkill(this->inputData.commandCombination._22b,  3, ACTION_jd22B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c,  3, ACTION_jd22C))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22b, 11, ACTION_ja2_22B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c, 11, ACTION_ja2_22C))
			return true;
	} else {
		if (this->_useSkill(this->inputData.commandCombination._623b, 0, ACTION_jd623B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623c, 0, ACTION_jd623C_HAMMER))
			return true;

		if (this->_useSkill(this->inputData.commandCombination._236b, 1, ACTION_jd236B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236c, 1, ACTION_jd236C_HAMMER))
			return true;
		if (this->effectiveSkillLevel[5] >= 2 && this->_useSkill(this->inputData.commandCombination._236b, 5, ACTION_ja1_236B_HAMMER))
			return true;
		if (this->effectiveSkillLevel[5] >= 2 && this->_useSkill(this->inputData.commandCombination._236c, 5, ACTION_ja1_236C_HAMMER))
			return true;

		if (this->_useSkill(this->inputData.commandCombination._22b,  3, ACTION_jd22B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c,  3, ACTION_jd22C_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22b,  7, ACTION_ja1_22B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c,  7, ACTION_ja1_22C_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22b, 11, ACTION_ja2_22B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c, 11, ACTION_ja2_22C_HAMMER))
			return true;
	}
	return false;
}

void Tewi::_processInputsAirborne()
{
	if (
		!this->handInfo.hand.empty() &&
		((this->inputData.keyInput.spellcard != 0 && this->inputData.keyInput.spellcard < 3) || this->inputData.bufferedKeyInput.spellcard != 0) &&
		this->confusionDebuffTimer == 0 &&
		this->unknown836 == 0 &&
		this->canActivateCard(0) &&
		this->_canUseCard(this->handInfo.hand[0].id)
	) {
		auto &card = this->handInfo.hand[0];

		if (100 <= card.id && card.id < 200 && this->_useSkillCard(card.id))
			return;
		if (200 <= card.id && card.id < 300 && this->_useSpellCard(card.id))
			return;
	}

	if (this->_tryPickUpHammer())
		return;
	if (!this->confusionDebuffTimer && this->_processSkillsAirborne())
		return;
	if (
		((this->inputData.keyUpC != 0 && this->inputData.keyUpC <= 2) || this->inputData.keyInput.c == 2 || this->inputData.bufferedKeyInput.c != 0) &&
		this->_processCAirborne()
	)
		return;
	if (
		((this->inputData.keyUpB != 0 && this->inputData.keyUpB <= 2) || this->inputData.keyInput.b == 2 || this->inputData.bufferedKeyInput.b != 0) &&
		this->_processBAirborne()
	)
		return;
	if (
		((this->inputData.keyUpA != 0 && this->inputData.keyUpA <= 2) || this->inputData.keyInput.a == 2 || this->inputData.bufferedKeyInput.a != 0) &&
		this->_processAAirborne()
	)
		return;
}

bool Tewi::_processAGrounded()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;
	short dashAction = this->_hammer ? SokuLib::ACTION_66A : ACTION_66A_HAMMER;

	if (
		this->frameState.actionId == SokuLib::ACTION_FORWARD_DASH &&
		this->inputData.keyInput.verticalAxis == 0 &&
		(hKeys > 0 || hBuffKeys > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(dashAction)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(dashAction);
		return true;
	}

	if (this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL))
		return false;

	if (this->_hammer) {
		if (
			(hKeys > 0 || hBuffKeys > 0) &&
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_3A)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_3A);
			return true;
		}

		if (
			std::abs(this->position.x - this->gameData.opponent->position.x) >= NO_HAMMER_FAR_RANGE &&
			!(hKeys < 0 || hBuffKeys < 0) &&
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_f2A)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_f2A);
			return true;
		} else if (
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			(this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_2A) || this->frameState.actionId == SokuLib::ACTION_2A)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_2A);
			return true;
		}

		if (
			(hKeys > 0 || hBuffKeys > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_6A)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_6A);
			return true;
		}

		if (
			(hKeys < 0 || hBuffKeys < 0) &&
			(this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_4A) || this->frameState.actionId == SokuLib::ACTION_4A)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_4A);
			return true;
		}

		short action = std::abs(this->position.x - this->gameData.opponent->position.x) >= NO_HAMMER_FAR_RANGE ? SokuLib::ACTION_f5A : SokuLib::ACTION_5A;
		bool ok = false;

		switch (this->frameState.actionId) {
		case SokuLib::ACTION_5A:
			action = SokuLib::ACTION_5AA;
			ok = true;
			break;
		case SokuLib::ACTION_5AA:
			action = SokuLib::ACTION_5AAA;
			ok = true;
			break;
		case SokuLib::ACTION_5AAA:
			if (hKeys <= 0 && hBuffKeys <= 0)
				action = SokuLib::ACTION_5AAAA;
			else if (this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0)
				action = SokuLib::ACTION_5AAA3A;
			else
				action = ACTION_5AAA6A;
			ok = true;
			break;
		case SokuLib::ACTION_5AAAA:
		case ACTION_5AAA6A:
		case SokuLib::ACTION_5AAA3A:
			return false;
		}

		if (this->gameData.sequenceData->actionLock <= this->getMoveLock(action) || ok) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(action);
			return true;
		}
	} else {
		if (
			(hKeys > 0 || hBuffKeys > 0) &&
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_3A_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_3A_HAMMER);
			return true;
		}

		if (
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_2A_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_2A_HAMMER);
			return true;
		}

		if (
			(hKeys > 0 || hBuffKeys > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_6A_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_6A_HAMMER);
			return true;
		}

		if (
			(hKeys < 0 || hBuffKeys < 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_4A_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_4A_HAMMER);
			return true;
		}

		short action = std::abs(this->position.x - this->gameData.opponent->position.x) >= HAMMER_FAR_RANGE ? ACTION_f5A_HAMMER : ACTION_5A_HAMMER;
		bool ok = false;

		switch (this->frameState.actionId) {
		case ACTION_5A_HAMMER:
			action = ACTION_5AA_HAMMER;
			ok = true;
			break;
		case ACTION_5AA_HAMMER:
			action = ACTION_5AAA_HAMMER;
			ok = this->frameState.poseId > 7;
			break;
		case ACTION_5AAA_HAMMER:
			return false;
		}

		if (this->gameData.sequenceData->actionLock <= this->getMoveLock(action) || ok) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(action);
			return true;
		}
	}
	return false;
}

bool Tewi::_processBGrounded()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;
	short dashAction = this->_hammer ? SokuLib::ACTION_66B : ACTION_66B_HAMMER;

	if (
		this->frameState.actionId == SokuLib::ACTION_FORWARD_DASH &&
		this->inputData.keyInput.verticalAxis == 0 &&
		(hKeys > 0 || hBuffKeys > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(dashAction)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(dashAction);
		return true;
	}

	if ((this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)) || this->currentSpirit < 200)
		return false;

	if (this->_hammer) {
		if (
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_2B)
			) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_2B);
			return true;
		}

		if (
			(hKeys > 0 || hBuffKeys > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_6B)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_6B);
			return true;
		}

		if (this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_5B)) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_5B);
			return true;
		}
	} else {
		if (
			(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_2B_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_2B_HAMMER);
			return true;
		}

		if (
			(hKeys > 0 || hBuffKeys > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_6B_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_6B_HAMMER);
			return true;
		}

		if (this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_5B_HAMMER)) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_5B_HAMMER);
			return true;
		}
	}
	return false;
}

bool Tewi::_processCGrounded()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;
	short dashAction = this->_hammer ? SokuLib::ACTION_66C : ACTION_66C_HAMMER;

	if (
		this->frameState.actionId == SokuLib::ACTION_FORWARD_DASH &&
		this->inputData.keyInput.verticalAxis == 0 &&
		(hKeys > 0 || hBuffKeys > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(dashAction)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(dashAction);
		return true;
	}

	if (this->_hammer)
		return false;

	if (
		(
			this->frameState.actionId >= SokuLib::ACTION_5A &&
			(this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)
		) ||
		this->currentSpirit < 200
	)
		return false;

	if (
		(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_2C)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_2C);
		return true;
	}

	if (
		(hKeys > 0 || hBuffKeys > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_6C)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_6C);
		return true;
	}

	if (
		(hKeys < 0 || hBuffKeys < 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_4C)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_4C);
		return true;
	}

	if (this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_5C)) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_5C);
		return true;
	}
	return false;
}

bool Tewi::_processSkillsGrounded()
{
	if (
		(
			this->frameState.actionId >= SokuLib::ACTION_5A &&
			(this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)
		) ||
		this->currentSpirit < 200
	)
		return false;

	if (this->_hammer) {
		if (this->_useSkill(this->inputData.commandCombination._623b, 0, ACTION_d623B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623c, 0, ACTION_d623C))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623b, 4, ACTION_a1_623B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623c, 4, ACTION_a1_623C))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623b, 8, ACTION_a2_623B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623c, 8, ACTION_a2_623C))
			return true;

		if (this->_useSkill(this->inputData.commandCombination._236b, 1, ACTION_d236B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236c, 1, ACTION_d236C))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236b, 5, ACTION_a1_236B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236c, 5, ACTION_a1_236C))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236b, 9, ACTION_a2_236B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236c, 9, ACTION_a2_236C))
			return true;

		if (this->_useSkill(this->inputData.commandCombination._214b, 2, ACTION_d214B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._214c, 2, ACTION_d214C))
			return true;
		if (
			this->_rabbitsStored < maxRabbits(this) &&
			this->_useSkill(this->inputData.commandCombination._214b, 6, ACTION_a1_214B)
		)
			return true;
		if (this->_rabbitsStored && this->_useSkill(this->inputData.commandCombination._214c, 6, ACTION_a1_214C))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._214b, 10, ACTION_a2_214B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._214c, 10, ACTION_a2_214C))
			return true;

		if (this->_useSkill(this->inputData.commandCombination._22b,  3, ACTION_d22B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c,  3, ACTION_d22C))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22b,  7, ACTION_a1_22B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c,  7, ACTION_a1_22C))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22b, 11, ACTION_a2_22B))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c, 11, ACTION_a2_22C))
			return true;
	} else {
		if (this->_useSkill(this->inputData.commandCombination._623b, 0, ACTION_d623B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623c, 0, ACTION_d623C_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623b, 4, ACTION_a1_623B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623c, 4, ACTION_a1_623C_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623b, 8, ACTION_a2_623B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._623c, 8, ACTION_a2_623C_HAMMER))
			return true;

		if (this->_useSkill(this->inputData.commandCombination._236b, 1, ACTION_d236B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236c, 1, ACTION_d236C_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236b, 5, ACTION_a1_236B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236c, 5, ACTION_a1_236C_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236b, 9, ACTION_a2_236B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._236c, 9, ACTION_a2_236C_HAMMER))
			return true;

		if (this->_useSkill(this->inputData.commandCombination._214b, 2, ACTION_d214B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._214c, 2, ACTION_d214C_HAMMER))
			return true;
		if (
			this->_rabbitsStored < 1 + this->effectiveSkillLevel[6] &&
			this->_useSkill(this->inputData.commandCombination._214b, 6, ACTION_a1_214B_HAMMER)
		)
			return true;
		if (this->_rabbitsStored && this->_useSkill(this->inputData.commandCombination._214c, 6, ACTION_a1_214C_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._214b, 10, ACTION_a2_214B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._214c, 10, ACTION_a2_214C_HAMMER))
			return true;

		if (this->_useSkill(this->inputData.commandCombination._22b,  3, ACTION_d22B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c,  3, ACTION_d22C_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22b,  7, ACTION_a1_22B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c,  7, ACTION_a1_22C_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22b, 11, ACTION_a2_22B_HAMMER))
			return true;
		if (this->_useSkill(this->inputData.commandCombination._22c, 11, ACTION_a2_22C_HAMMER))
			return true;
	}
	return false;
}

bool Tewi::_canUseCard(int id)
{
	if (id < 100)
		return true;

	switch (id) {
	case 100:
	case 101:
	case 103:
	case 107:
	case 111:
	case 201:
	case 208:
		return true;
	case 105:
		if (this->effectiveSkillLevel[5] != 0 || SokuLib::activeWeather == SokuLib::WEATHER_SPRINKLE)
			return true;
		return this->isGrounded();
	case 102:
	case 104:
	case 106:
	case 108:
	case 109:
	case 110:
	case 200:
	case 202:
	case 203:
	case 206:
	case 210:
		return this->isGrounded();
	case 205:
		if (!this->isGrounded())
			return false;
		for (int x = this->position.x + 200 * this->direction - 180; x < this->position.x + 200 * this->direction + 180; x++) {
			if (x < 0 || x >= 1280)
				continue;
			if (SokuLib::v2::groundHeight[x] != 0)
				return false;
		}
		return true;
	case 204:
		return this->_hammer == nullptr;
	case 207:
		return this->_hammer != nullptr;
	default:
		return false;
	}
}

bool Tewi::_useSkillCard(int id)
{
	if (!this->_canUseCard(id))
		return false;
	if (this->gameData.sequenceData->actionLock > 50)
		return false;
	if (
		this->frameState.actionId >= SokuLib::ACTION_5A &&
		(this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)
	)
		return false;
	if (this->inputData.keyInput.verticalAxis || this->inputData.keyInput.horizontalAxis) {
		if (!this->isGrounded())
			return false;
		this->setAction(SokuLib::ACTION_SKILL_CARD);
		return true;
	}

	int index = id - 100;
	int slot = index % 4;
	int action = ACTION_d623B + index * 8;

	if (!this->_hammer)
		action += 4;
	if (!this->isGrounded())
		action += 2;
	if (this->gameData.sequenceData->actionLock > this->getMoveLock(action))
		return false;
	if (!this->skillCancelsUsed[slot]) {
		this->skillCancelCount++;
		this->skillCancelsUsed[slot] = true;
	}
	this->eventSkillUse();
	this->onSkillUpgrade();
	this->useSkill(action, this->gameData.sequenceData->moveLock);
	return true;
}

bool Tewi::_useSpellCard(int id)
{
	if (this->gameData.sequenceData->actionLock > 100)
		return false;
	if (this->inputData.keyInput.horizontalAxis != 0)
		return false;

	if (
		this->frameState.actionId >= SokuLib::ACTION_5A &&
		(this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)
	)
		return false;

	id += 400;
	if (!this->_hammer)
		id += 20;
	if (!this->isGrounded())
		id += 50;
	this->_offset = 0;
	this->_cropLimit = 400;
	this->useSpellCard(id, this->gameData.sequenceData->moveLock);
	return true;
}

void Tewi::_processInputsGrounded()
{
	if (
		!this->handInfo.hand.empty() &&
		((this->inputData.keyInput.spellcard != 0 && this->inputData.keyInput.spellcard < 3) || this->inputData.bufferedKeyInput.spellcard != 0) &&
		this->confusionDebuffTimer == 0 &&
		this->unknown836 == 0 &&
		this->canActivateCard(0) &&
		this->_canUseCard(this->handInfo.hand[0].id)
	) {
		auto &card = this->handInfo.hand[0];

		if (0   <= card.id && card.id < 100 && this->useSystemCard(this->gameData.sequenceData->actionLock))
			return;
		if (100 <= card.id && card.id < 200 && this->_useSkillCard(card.id))
			return;
		if (200 <= card.id && card.id < 300 && this->_useSpellCard(card.id))
			return;
	}
	if (!this->confusionDebuffTimer && this->_processSkillsGrounded())
		return;

	if (this->_tryPickUpHammer())
		return;
	if (
		((this->inputData.keyUpC != 0 && this->inputData.keyUpC <= 2) || this->inputData.keyInput.c == 2 || this->inputData.bufferedKeyInput.c != 0) &&
		this->_processCGrounded()
	)
		return;
	if (
		((this->inputData.keyUpB != 0 && this->inputData.keyUpB <= 2) || this->inputData.keyInput.b == 2 || this->inputData.bufferedKeyInput.b != 0) &&
		this->_processBGrounded()
	)
		return;
	if (
		((this->inputData.keyUpA != 0 && this->inputData.keyUpA <= 2) || this->inputData.keyInput.a == 2 || this->inputData.bufferedKeyInput.a != 0) &&
		this->_processAGrounded()
	)
		return;
}

void Tewi::handleInputs()
{
	auto frameFlags = this->gameData.frameData->frameFlags;

	if (!this->_revive && this->handleCardSwitch())
		return;
	if (this->_rabbitAnimation)
		return;

	if (frameFlags.cancellable || frameFlags.highJumpCancellable) {
		auto uVar10 = *(unsigned *)&this->inputData.commandCombination;

		// For story mode
		if (699 < uVar10 && uVar10 < 800) {
			this->setAction(uVar10);
			return this->FUN_0046d950();
		}

		if (this->unknown4C4 == 0 && this->isOnGround()) {
			if (frameFlags.highJumpCancellable && this->gameData.sequenceData->actionLock > 9 && this->handleHJ())
				return;
			if (this->handleGroundBE())
				return;
			if (this->handleHJInput(this->gameData.sequenceData->actionLock, frameFlags.highJumpCancellable))
				return;
			if (this->handleGroundDash(this->gameData.sequenceData->actionLock, frameFlags.highJumpCancellable))
				return;
		} else {
			if (this->handleAirBE())
				return;

			auto allowedAirMoves = (this->weatherId == SokuLib::WEATHER_TEMPEST) + 2;

			if (this->handleFwdAirDash(this->gameData.sequenceData->actionLock, frameFlags.highJumpCancellable, allowedAirMoves, 2))
				return;
			if (this->handleBackAirDash(this->gameData.sequenceData->actionLock, frameFlags.highJumpCancellable, allowedAirMoves, 2))
				return;
			if (this->handleNormalFlight(this->gameData.sequenceData->actionLock, frameFlags.highJumpCancellable, allowedAirMoves))
				return;
		}
	}
	if (this->stopwatchTimeLeft)
		return;
	if (!frameFlags.cancellable) {
		this->_tryPickUpHammer();
		return;
	}
	if (this->isGrounded())
		this->_processInputsGrounded();
	else
		this->_processInputsAirborne();
}

void Tewi::VUnknown58()
{
}

void Tewi::VUnknown5C()
{
}

bool Tewi::VUnknown60(int a)
{
	return false;
}

constexpr auto operator_bracket = &SokuLib::Deque<SokuLib::Card>::operator[];

void __declspec(naked) tewiRevivePreventDeath()
{
	__asm {
		// if (
		//     defender->gameData.ally == defender &&
		//     defender->characterIndex == SokuLib::CHARACTER_TEWI)
		// ) {
		//     if (
		//         defender->HP <= 0 &&
		//         defender->confusionDebuffTimer == 0 &&
		//         !defender->handInfo.hand.empty() &&
		//         defender->handInfo.hand[0]->id == 211 &&
		//         defender->handInfo.hand[0]->cost <= defender->handInfo.hand.size()
		//     ) {
		//         ((Tewi *)defender)->revive = true;
		//         defender->HP = 1;
		//         defender->untech = 900;
		//         if (hitAnimation <= 70 || hitAnimation >= 200)
		//             hitAnimation = 71;
		//     } else {
		//         ((Tewi *)defender)->revive = false;
		//     }
		// }

		// if (this->gameData.ally == this) -> Check if the object is a player
		CMP [ESI + 0x16C], ESI
		JNZ noChange
		// if (defender->characterIndex == SokuLib::CHARACTER_TEWI)
		CMP [ESI + 0x34C], 35
		JNZ noChange

		// if (defender->HP <= 0)
		CMP word ptr [ESI + 0x184], 0
		JG noChange

		// if (defender->confusionDebuffTimer == 0)
		CMP word ptr [ESI + 0x524], 0
		JNZ noChangeTewi

		// if (!this->handInfo.hand.empty())
		CMP [ESI + 0x5F8], 0
		JZ noChangeTewi

		// this->handInfo.hand[0]
		PUSH ECX
		PUSH 0
		LEA ECX, [ESI + 0x5E8]
		MOV EAX, [operator_bracket]
		CALL EAX
		POP ECX

		// if (this->handInfo.hand[0]->id == 211)
		CMP word ptr [EAX], 211
		JNZ noChangeTewi

		// if (this->handInfo.hand[0]->cost <= this->handInfo.hand.size())
		MOVZX EAX, word ptr [EAX + 2]
		CMP EAX, [ESI + 0x5F8]
		JG noChangeTewi

	doIt:
		// ((Tewi *)defender)->revive = true;
		MOV byte ptr [ESI + 0x890], 1
		// defender->HP = 1;
		MOV word ptr [ESI + 0x184], 1
		// defender->untech = 900;
		MOV word ptr [ESI + 0x4BA], 900

		TEST ECX, ECX
		JZ noChange
		CMP dword ptr [ESP + 0x28], 70
		JLE forceHitAnimation
		CMP dword ptr [ESP + 0x28], 200
		JL noChange

	forceHitAnimation:
		MOV dword ptr [ESP + 0x28], 71
		JMP noChange

	noChangeTewi:
		// ((Tewi *)defender)->revive = false;
		MOV byte ptr [ESI + 0x890], 0

	noChange:
		MOVZX EAX, word ptr [ESI + 0x184]
		RET
	}
}

void __declspec(naked) tewiRevivePreventDeath2()
{
	__asm {
		PUSH ESI
		MOV ESI, EDI
		XOR ECX, ECX
		CALL tewiRevivePreventDeath
		POP ESI
		RET
	}
}

void Tewi::hook()
{
	DWORD old;

	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	SokuLib::TamperNearCall(0x47B123, tewiRevivePreventDeath);
	*(char *)0x47B128 = 0x90;
	*(char *)0x47B129 = 0x90;
	SokuLib::TamperNearCall(0x47B329, tewiRevivePreventDeath2);
	*(char *)0x47B32E = 0x90;
	*(char *)0x47B32F = 0x90;

	*(char *)0x48894C = 0xEB;
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);
}

void Tewi::unhook()
{
	DWORD old;
	unsigned char oldData[] = {0x0F, 0xB7, 0x86, 0x84, 0x01, 0x00, 0x00};
	unsigned char oldData2[] = {0x0F, 0xB7, 0x87, 0x84, 0x01, 0x00, 0x00};

	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	*(char *)0x48894C = 0x74;
	memcpy((void *)0x47B123, oldData, sizeof(oldData));
	memcpy((void *)0x47B329, oldData2, sizeof(oldData2));
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);
}

SokuLib::v2::GameObject *Tewi::getHammer() const
{
	return this->_hammer;
}

void Tewi::setRabbitAnimation()
{
	this->_rabbitAnimation = true;
}

void Tewi::render()
{
	SokuLib::SpriteEx::Coord old[4];
	SokuLib::DxVertex old2[4];

	memcpy(old, this->sprite.baseCoords, sizeof(this->sprite.baseCoords));
	memcpy(old2, this->sprite.vertices, sizeof(this->sprite.vertices));
	this->sprite.baseCoords[2].y = min(this->sprite.baseCoords[2].y, this->_cropLimit);
	this->sprite.baseCoords[3].y = min(this->sprite.baseCoords[3].y, this->_cropLimit);
	this->sprite.vertices[2].v *= this->sprite.baseCoords[2].y / old[2].y;
	this->sprite.vertices[3].v *= this->sprite.baseCoords[3].y / old[3].y;
	for (auto &coord : this->sprite.baseCoords)
		coord.y += old[3].y - this->sprite.baseCoords[3].y + this->_offset;
	AnimationObject::render();
	memcpy(this->sprite.vertices, old2, sizeof(this->sprite.vertices));
	memcpy(this->sprite.baseCoords, old, sizeof(this->sprite.baseCoords));
	hammer.tint = this->_hammer == nullptr ? SokuLib::Color::White : SokuLib::Color{0x80, 0x00, 0x00};
	hammer.tint.a = this->renderInfos.color.a;
	hammer.setPosition(SokuLib::Vector2f{this->position.x - hammer.getSize().x / 2, -this->position.y - 200}.to<int>());
	hammer.draw();
	if (this->teamId == 0 && __practiceEffect != -1) {
		faces[__practiceEffect].draw();
		leaf.draw();
	}
}

unsigned Tewi::getHammerPickTimer()
{
	return this->_hammerPickTimer;
}
