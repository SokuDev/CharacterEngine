//
// Created by PinkySmile on 08/05/24.
//

#include <complex>
#include <map>
#include "Tewi.hpp"
#include "TewiObjectFactory.hpp"
#include "GameObjectList.hpp"

#define ACTION_IDLE_NO_HAMMER                               11
#define ACTION_CROUCHING_NO_HAMMER                          12
#define ACTION_CROUCHED_NO_HAMMER                           13
#define ACTION_STANDING_UP_NO_HAMMER                        14
#define ACTION_WALK_FORWARD_NO_HAMMER                       15
#define ACTION_WALK_BACKWARD_NO_HAMMER                      16
#define ACTION_NEUTRAL_JUMP_NO_HAMMER                       17
#define ACTION_FORWARD_JUMP_NO_HAMMER                       18
#define ACTION_BACKWARD_JUMP_NO_HAMMER                      19
#define ACTION_FALLING_NO_HAMMER                            20
#define ACTION_LANDING_NO_HAMMER                            21
#define ACTION_NEUTRAL_HIGH_JUMP_NO_HAMMER                  216
#define ACTION_FORWARD_HIGH_JUMP_NO_HAMMER                  217
#define ACTION_BACKWARD_HIGH_JUMP_NO_HAMMER                 218
#define ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER 227
#define ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER 228
#define ACTION_BE2_NO_HAMMER                                229
#define ACTION_BE1_NO_HAMMER                                230
#define ACTION_BE6_NO_HAMMER                                231
#define ACTION_BE4_NO_HAMMER                                232

#define ACTION_5AAA6A                          ACTION_5AAAAA
#define ACTION_5A_HAMMER                       310
#define ACTION_5AA_HAMMER                      319
#define ACTION_5AAA_HAMMER                     312
#define ACTION_4A_HAMMER                       313
#define ACTION_66A_HAMMER                      314
#define ACTION_2A_HAMMER                       315
#define ACTION_3A_HAMMER                       316
#define ACTION_6A_HAMMER                       317
#define ACTION_f5A_HAMMER                      318
#define ACTION_j5A_HAMMER                      331
#define ACTION_j6A_HAMMER                      332
#define ACTION_j2A_HAMMER                      333
#define ACTION_j8A_HAMMER                      334
#define ACTION_66B_HAMMER                      327
#define ACTION_66C_HAMMER                      328

#define ACTION_5B_HAMMER                       401
#define ACTION_2B_HAMMER                       403
#define ACTION_j5B_HAMMER                      405
#define ACTION_j2B_HAMMER                      407

#define ACTION_STAND_PICKUP_HAMMER_FROM_AIR    420
#define ACTION_STAND_PICKUP_HAMMER_FROM_GROUND 421
#define ACTION_AIR_PICKUP_HAMMER_FROM_AIR      422
#define ACTION_AIR_PICKUP_HAMMER_FROM_GROUND   423

const std::map<unsigned short, unsigned short> systemToHammer{
	{ SokuLib::ACTION_IDLE,                                  ACTION_IDLE_NO_HAMMER },
	{ SokuLib::ACTION_CROUCHING,                             ACTION_CROUCHING_NO_HAMMER },
	{ SokuLib::ACTION_CROUCHED,                              ACTION_CROUCHED_NO_HAMMER },
	{ SokuLib::ACTION_STANDING_UP,                           ACTION_STANDING_UP_NO_HAMMER },
	{ SokuLib::ACTION_WALK_FORWARD,                          ACTION_WALK_FORWARD_NO_HAMMER },
	{ SokuLib::ACTION_WALK_BACKWARD,                         ACTION_WALK_BACKWARD_NO_HAMMER },
	{ SokuLib::ACTION_NEUTRAL_JUMP,                          ACTION_NEUTRAL_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_FORWARD_JUMP,                          ACTION_FORWARD_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_BACKWARD_JUMP,                         ACTION_BACKWARD_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_FALLING,                               ACTION_FALLING_NO_HAMMER },
	{ SokuLib::ACTION_LANDING,                               ACTION_LANDING_NO_HAMMER },
	{ SokuLib::ACTION_NEUTRAL_HIGH_JUMP,                     ACTION_NEUTRAL_HIGH_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_FORWARD_HIGH_JUMP,                     ACTION_FORWARD_HIGH_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_BACKWARD_HIGH_JUMP,                    ACTION_BACKWARD_HIGH_JUMP_NO_HAMMER },
	{ SokuLib::ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH,    ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER },
	{ SokuLib::ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH,    ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER },
	{ SokuLib::ACTION_BE2,                                   ACTION_BE2_NO_HAMMER },
	{ SokuLib::ACTION_BE1,                                   ACTION_BE1_NO_HAMMER },
	{ SokuLib::ACTION_BE6,                                   ACTION_BE6_NO_HAMMER },
	{ SokuLib::ACTION_BE4,                                   ACTION_BE4_NO_HAMMER },
};
const std::map<unsigned short, unsigned short> hammerToSystem{
	{ ACTION_IDLE_NO_HAMMER,                                  SokuLib::ACTION_IDLE },
	{ ACTION_CROUCHING_NO_HAMMER,                             SokuLib::ACTION_CROUCHING },
	{ ACTION_CROUCHED_NO_HAMMER,                              SokuLib::ACTION_CROUCHED },
	{ ACTION_STANDING_UP_NO_HAMMER,                           SokuLib::ACTION_STANDING_UP },
	{ ACTION_WALK_FORWARD_NO_HAMMER,                          SokuLib::ACTION_WALK_FORWARD },
	{ ACTION_WALK_BACKWARD_NO_HAMMER,                         SokuLib::ACTION_WALK_BACKWARD },
	{ ACTION_NEUTRAL_JUMP_NO_HAMMER,                          SokuLib::ACTION_NEUTRAL_JUMP },
	{ ACTION_FORWARD_JUMP_NO_HAMMER,                          SokuLib::ACTION_FORWARD_JUMP },
	{ ACTION_BACKWARD_JUMP_NO_HAMMER,                         SokuLib::ACTION_BACKWARD_JUMP },
	{ ACTION_FALLING_NO_HAMMER,                               SokuLib::ACTION_FALLING },
	{ ACTION_LANDING_NO_HAMMER,                               SokuLib::ACTION_LANDING },
	{ ACTION_NEUTRAL_HIGH_JUMP_NO_HAMMER,                     SokuLib::ACTION_NEUTRAL_HIGH_JUMP },
	{ ACTION_FORWARD_HIGH_JUMP_NO_HAMMER,                     SokuLib::ACTION_FORWARD_HIGH_JUMP },
	{ ACTION_BACKWARD_HIGH_JUMP_NO_HAMMER,                    SokuLib::ACTION_BACKWARD_HIGH_JUMP },
	{ ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER,    SokuLib::ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH },
	{ ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER,    SokuLib::ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH },
	{ ACTION_BE2_NO_HAMMER,                                   SokuLib::ACTION_BE2 },
	{ ACTION_BE1_NO_HAMMER,                                   SokuLib::ACTION_BE1 },
	{ ACTION_BE6_NO_HAMMER,                                   SokuLib::ACTION_BE6 },
	{ ACTION_BE4_NO_HAMMER,                                   SokuLib::ACTION_BE4 },
};

Tewi::Tewi(SokuLib::PlayerInfo &info) :
	SokuLib::v2::Player(info)
{
	this->objectList = new GameObjectList<TewiObjectFactory>(this);
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
		printf("%i %i\n", this->frameState.poseId, this->chargedAttack);
		if (this->frameState.poseId == 4 && this->chargedAttack) {
			this->nextSequence();
			this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
		}
		if (this->frameState.poseId >= 6 && this->frameState.poseId < 11) {
			float carrotParams[3] = {angle + SokuLib::rand(40) - 20.f, speed, slowDown};
			auto obj = this->createObject(801, (this->direction * 40) + this->position.x, this->position.y + yOffset, this->direction, 1, carrotParams);

			if (this->frameState.poseId == 6) {
				this->consumeSpirit(200, 45);
				this->addCardMeter(20);
			}
			this->collisionType = 1;
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
			auto obj1 = this->createObject(801, (this->direction * 40) + this->position.x, this->position.y + yOffset, this->direction, 1, carrotParams, 3);
			auto obj2 = this->createObject(801, (this->direction * 40) + this->position.x, this->position.y + yOffset, this->direction, 1, carrotParams + 3, 3);

			if (this->frameState.poseId == 6) {
				this->consumeSpirit(200, 45);
				this->addCardMeter(40);
			}
			this->collisionType = 1;
			this->playSFX(1);
			obj1->collisionLimit = 1;
			obj2->collisionLimit = 1;
		}
	}
}

void Tewi::update()
{
	this->unknown88C = 0;
	this->unknown88E = 0x65;
	if (
		SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN <= this->frameState.actionId && this->frameState.actionId <= SokuLib::ACTION_AIR_GUARD &&
		!this->blockObjectSpawned
	) {
		float yOffset;

		this->blockObjectSpawned = true;
		if (this->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_HIGH_HUGE_BLOCKSTUN)
			yOffset = 94;
		else if (this->frameState.actionId == SokuLib::ACTION_AIR_GUARD)
			yOffset = 100;
		else //if (SokuLib::ACTION_RIGHTBLOCK_LOW_SMALL_BLOCKSTUN <= this->frameState.actionId && this->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_LOW_HUGE_BLOCKSTUN)
			yOffset = 60;
		this->createObject(998, (float)(this->direction * 57) + this->position.x, this->position.y + yOffset, this->direction, 1, nullptr, 0);
	}
	if (this->hitStop)
		return;
	if (this->gameData.opponent->timeStop)
		return;

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
		if (!this->_hammer) {
			this->applyGroundMechanics();
			this->updateGroundMovement(FRONT_WALK_SPEED);
			this->advanceFrame();
			break;
		}
		switch (this->frameState.sequenceId) {
		case 0:
			this->applyGroundMechanics();
			this->advanceFrame();
			if (this->frameState.sequenceId == 1) {
				this->speed.y = FORWARD_WALK_SPEED_Y;
				this->updateGroundMovement(FORWARD_WALK_SPEED_X);
				this->gravity = {0, FORWARD_WALK_GRAVITY};
			}
			break;
		case 1:
			this->speed -= this->gravity;
			this->advanceFrame();
			break;
		case 2:
			this->speed -= this->gravity;
			if (this->isGrounded()) {
				SokuLib::playSEWaveBuffer(SokuLib::SFX_LAND);
				this->nextSequence();
			} else
				this->advanceFrame();
			break;
		case 3:
			this->speed -= this->gravity;
			this->applyGroundMechanics();
			if (this->speed.x != 0)
				this->speed.x -= 1.5;
		case 4:
			this->advanceFrame();
		}
		break;
	case SokuLib::ACTION_WALK_BACKWARD:
		this->applyGroundMechanics();
		this->updateGroundMovement(this->_hammer ? HAMMER_BACK_WALK_SPEED : BACK_WALK_SPEED);
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
			if (this->inputData.unknown7CC == 2) {
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
			this->speed.y = 15;
			this->speed.x = 10;
			this->gravity.y = 1;
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
			if (this->inputData.unknown7CC == 2) {
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
			this->speed.y = 15;
			this->speed.x = -10.0;
			this->gravity.y = 1;
		}
		break;
	case SokuLib::ACTION_NEUTRAL_TECH:
		this->applyGroundMechanics();
		if (this->advanceFrame()) {
			if (this->inputData.unknown7CC == 2) {
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

			int minTimer = this->frameState.actionId == SokuLib::ACTION_FORWARD_DASH ? 5 : 15;

			this->dashTimer++;
			if ((this->direction * this->inputData.keyInput.horizontalAxis < 1 && this->dashTimer > minTimer) || this->dashTimer > MAX_DASH_HOLD) {
				this->setAction(SokuLib::ACTION_GROUNDDASH_RECOVERY);
				return;
			}
		}
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->updateGroundMovement(this->_hammer ? INITIAL_DASH_SPEED : HAMMER_INITIAL_DASH_SPEED);
			this->createEffect(0x7d, (float)(this->direction * 0x50) + this->position.x, this->position.y + 80, this->direction,1);
			this->createEffect(0x7e, this->position.x, this->position.y + 80, this->direction, 1);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
		}
		break;
	case SokuLib::ACTION_BE4:
	case SokuLib::ACTION_BACKDASH:
		if (this->frameState.sequenceId == 0 || frameState.sequenceId == 3)
			this->applyGroundMechanics();
		if (this->frameState.sequenceId == 3) {
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
			this->updateGroundMovement(this->_hammer ? BACKDASH_IMPULSE : HAMMER_BACKDASH_IMPULSE);
			this->speed.y = 3.5;
			this->gravity.y = 0.75;
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
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		break;
	case SokuLib::ACTION_j8A:
		if (this->frameState.sequenceId != 1 || this->frameState.poseId > 2)
			this->speed -= this->gravity;
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
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2 && this->chargedAttack) {
			this->nextSequence();
			this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 2) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed.y = 9;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed.y = 5;
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
			if (this->speed.y > 2)
				this->speed.y -= 0.5;
			else if (this->speed.y < 2)
				this->speed.y += 0.5;
			if (this->speed.x > 4)
				this->speed.x -= 0.5;
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
		if (this->frameState.sequenceId == 0 && this->frameState.poseId < 3)
			this->speed = {0, 0};
		else if (this->frameState.sequenceId == 0 && this->frameState.poseId >= 4 && this->speed.x > 0)
			this->speed.x -= 0.5;
		else {
			if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->speed = {12, 0};
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
	case SokuLib::ACTION_5AAA6A:
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
		if (this->advanceFrame())
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
		case 11:
			if (this->frameState.poseFrame == 0) {
				this->speed.x = 0;
				this->speed.y = 0;
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
				this->collisionType = 0;
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
		case 14:
			if (this->frameState.poseFrame == 0)
				this->speed.y = this->gravity.y * (8 + 8 + 8 + 4 + 4) / 2.f - this->gravity.y * (8 + 8 + 8 + 4);
		case 8:
		case 9:
		case 10:
		case 15:
			this->speed.y -= this->gravity.y;
			break;
		case 16:
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
				this->collisionType = 0;
			}
		}
		break;
	case ACTION_66C_HAMMER:
		if (!this->gameData.frameData->frameFlags.airborne)
			this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_CROUCHED);
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
				this->collisionType = 0;
			}
			break;
		case 7:
			if (this->speed.x > 0)
				this->speed.x -= 0.5;
			if (this->frameState.poseFrame == 0) {
				float hammerParams[3] = {90.f, 30, 0};

				this->consumeSpirit(200, 60);
				this->addCardMeter(50);
				this->_hammerPickTimer = 20;
				this->_hammer = this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams);
				this->_hammer->collisionType = 1;
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
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 2 && this->chargedAttack) {
			this->nextSequence();
			this->createEffect(0x3E, (float)(this->direction * 12) + this->position.x, this->position.y + 122.0, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 5)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		break;


	case ACTION_j2B_HAMMER:
		this->_bSeriesUpdate(-45, HAMMER_B_BULLET_SPEED, HAMMER_B_BULLET_SLOW_DOWN, 100, true);
		break;
	case SokuLib::ACTION_j2B:
		this->_bSeriesUpdate(-45, B_BULLET_SPEED, B_BULLET_SLOW_DOWN, 100, true);
		break;
	case ACTION_j5B_HAMMER:
		this->_bSeriesUpdate(0, HAMMER_B_BULLET_SPEED, HAMMER_B_BULLET_SLOW_DOWN, 130, true);
		break;
	case SokuLib::ACTION_j5B:
		this->_bSeriesUpdate(0, B_BULLET_SPEED, B_BULLET_SLOW_DOWN, 130, true);
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
				float hammerParams[3] = {90.f - 45 * this->direction, 20, 0};

				this->consumeSpirit(200, 60);
				this->collisionType = 1;
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
				float hammerParams[3] = {90.f - 80 * this->direction, 25};

				this->consumeSpirit(200, 60);
				this->collisionType = 1;
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
				float hammerParams[3] = {90.f - 10 * this->direction, 30};

				this->consumeSpirit(200, 60);
				this->collisionType = 1;
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
			float hammerParams[3] = {-90.f + 45.f * this->direction, 30, 1};

			if (this->frameState.actionId == SokuLib::ACTION_j6C) {
				hammerParams[0] = 90.f - 75.f * this->direction;
				hammerParams[2] = 0;
			} else if (this->frameState.actionId == SokuLib::ACTION_j2C)
				hammerParams[0] = -90.f;

			this->consumeSpirit(200, 60);
			this->collisionType = 1;
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

	case SokuLib::ACTION_USING_SC_ID_200:
		this->applyGroundMechanics();
		if (this->frameState.sequenceId == 0) {
			this->advanceFrame();
			if (this->frameState.sequenceId == 1)
				this->setAction(SokuLib::ACTION_IDLE);
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3) {
				this->unknown4A6 = 40;
				this->playSpellBackground(0, 120);
				SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
				this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
				this->consumeCard(0, 0, 0x3c);
				this->eventSpellUse();
				this->eventWeatherCycle();
			}
			if (this->collisionType == 1) {
				this->gameData.opponent->setAction(SokuLib::ACTION_GRABBED);
				this->gameData.opponent->speed.x = 0;
				this->gameData.opponent->speed.y = 0;
				this->collisionType = 0;
				this->collisionLimit = 1;
				this->nextSequence();
			}
		} else if (this->frameState.sequenceId == 1) {
			this->advanceFrame();
			this->playSpellBackground(0, 30);
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 4) {
				this->gameData.opponent->setAction(SokuLib::ACTION_AIR_HIT_BIG_HITSTUN);
				this->gameData.opponent->untech = 300;
				this->gameData.opponent->speed.x = 0;
				this->gameData.opponent->speed.y = 45;
				this->gameData.opponent->gravity.y = 0.5;
				SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
			}
			if (this->frameState.poseId != 10)
				break;
			if (
				this->inputData.keyInput.a == 1 ||
				this->inputData.keyInput.b == 1 ||
				this->inputData.keyInput.c == 1 ||
				this->inputData.keyInput.d == 1 ||
				this->inputData.keyInput.spellcard == 1
			)
				this->nextSequence();
		} else if (this->frameState.sequenceId == 2) {
			if (this->advanceFrame())
				this->setAction(SokuLib::ACTION_IDLE);
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 2)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
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
	default: {
		int old = this->frameState.poseFrame;

		this->updateDefaultBehavior();
		// Fail safe to prevent softlocks
		if (old == this->frameState.poseFrame && this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
	}}
}

bool Tewi::setAction(short action)
{
	printf("Tewi::setAction(%i)\n", action);

	// You can cancel h.5AA(1) right after the hitstop, where you are still considered on the ground
	// into a grounded bullet (h.xB or h.xC). However, you keep the vertical momentum which causes you to go in the air
	// and the cancel the grounded bullet immediately. It allows for some funky stuff which isn't a problem by itself
	// but the spirit cost is applied when the bullets spawn, not when the move is started. Here we just make sure
	// that doing this still costs one orb.
	if (
		this->frameState.actionId == ACTION_5AA_HAMMER &&
		this->frameState.sequenceId == 0 &&
		this->frameState.poseId == 7 &&
		this->frameState.poseFrame == 1 &&
		action >= SokuLib::ACTION_5A
	)
		this->consumeSpirit(200, 60);

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
}

bool Tewi::initializeAction()
{
	auto it = hammerToSystem.find(this->frameState.actionId);

	if (it != hammerToSystem.end())
		this->frameState.actionId = it->second;
	switch (this->frameState.actionId) {
	case SokuLib::ACTION_WALK_FORWARD:
		this->speed.x = FRONT_WALK_SPEED;
		break;
	case SokuLib::ACTION_WALK_BACKWARD:
		this->speed.x = this->_hammer ? HAMMER_BACK_WALK_SPEED : BACK_WALK_SPEED;
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
		this->renderInfos.zRotation = 0;
		this->renderInfos.yRotation = 0;
		this->renderInfos.xRotation = 0;
		break;
	case ACTION_5AA_HAMMER:
		this->collisionType = 0;
		this->collisionLimit = 2;
		this->speed.x = 0.0;
		break;
	case SokuLib::ACTION_f5A:
		this->speed.x = 6;
		this->collisionType = 0;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_5AAA:
		this->speed.x = 10;
		this->collisionType = 0;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_66A:
	case SokuLib::ACTION_66B:
	case SokuLib::ACTION_66C:
	case SokuLib::ACTION_5AAAA:
	case SokuLib::ACTION_5AAA6A:
	case SokuLib::ACTION_5AAA3A:
		this->speed.x = 14;
		this->collisionType = 0;
		this->collisionLimit = 1;
		break;
	case ACTION_66A_HAMMER:
		this->speed.x = 10;
		this->collisionType = 0;
		this->collisionLimit = 1;
		break;
	case ACTION_66B_HAMMER:
		this->speed.x = 10;
		this->collisionType = 0;
		this->collisionLimit = 7;
		break;
	case ACTION_66C_HAMMER:
		this->speed.x = 10;
		this->collisionType = 0;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_j2A:
	case SokuLib::ACTION_j8A:
		this->chargedAttack = true;
	case SokuLib::ACTION_j6A:
	case SokuLib::ACTION_j5A:
		this->gravity.y = FALLING_GRAVITY;
		this->collisionType = 0;
		this->collisionLimit = 1;
		break;
	case ACTION_j5A_HAMMER:
	case ACTION_j8A_HAMMER:
	case ACTION_j2A_HAMMER:
	case ACTION_j6A_HAMMER:
		this->gravity.y = HAMMER_FALLING_GRAVITY;
	case SokuLib::ACTION_USING_SC_ID_200:
		this->collisionType = 0;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_6A:
	case SokuLib::ACTION_3A:
	case ACTION_3A_HAMMER:
	case ACTION_f5A_HAMMER:
	case ACTION_2A_HAMMER:
	case ACTION_6A_HAMMER:
	case ACTION_5AAA_HAMMER:
		this->chargedAttack = true;
	case ACTION_4A_HAMMER:
	case ACTION_5A_HAMMER:
		this->speed.x = 0.0;
	case SokuLib::ACTION_2A:
	case SokuLib::ACTION_4A:
	case SokuLib::ACTION_5A:
	case SokuLib::ACTION_f2A:
	case SokuLib::ACTION_5AA:
		this->collisionType = 0;
		this->collisionLimit = 1;
		if (this->groundDashCount == 0)
			this->speed.x = 0.0;
		break;
	case ACTION_2B_HAMMER:
	case SokuLib::ACTION_2B:
	case ACTION_5B_HAMMER:
	case SokuLib::ACTION_5B:
		this->chargedAttack = true;
	case SokuLib::ACTION_5C:
	case SokuLib::ACTION_2C:
	case SokuLib::ACTION_6C:
		this->collisionType = 0;
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
		this->collisionType = 0;
		break;
	default:
		Player::initializeAction();
		break;
	}
	return true;
}

bool Tewi::_canPickUpHammer()
{
	if (SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN <= this->frameState.actionId && this->frameState.actionId <= SokuLib::ACTION_NEUTRAL_TECH)
		return false;
	if (this->_hammer->frameState.sequenceId == 0) {
		if (std::abs(this->_hammer->position.x - this->position.x) > 100)
			return false;
		if (this->_hammer->position.y + 50 < this->position.y)
			return false;
		if (this->_hammer->position.y - 200 > this->position.y)
			return false;
	} else {
		if (std::abs(this->_hammer->position.x - this->position.x) > 75)
			return false;
		if (std::abs(this->_hammer->position.y - this->position.y) > 150)
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
	this->_hammer->lifetime = 0;
	this->_hammer = nullptr;
	SokuLib::playSEWaveBuffer(SokuLib::SFX_SLAP_HIT);
	return true;
}

bool Tewi::_processAAirborne()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;


	if (this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == 0 || this->collisionType == 3))
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

	if ((this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == 0 || this->collisionType == 3)) || this->currentSpirit < 200)
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
			(this->collisionType == 0 || this->collisionType == 3)
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

void Tewi::_processInputsAirborne()
{
	if (this->_tryPickUpHammer())
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

	if (this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == 0 || this->collisionType == 3))
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
				action = SokuLib::ACTION_5AAA6A;
			ok = true;
			break;
		case SokuLib::ACTION_5AAAA:
		case SokuLib::ACTION_5AAA6A:
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

	if ((this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == 0 || this->collisionType == 3)) || this->currentSpirit < 200)
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

		/*if (
			(hKeys > 0 || hBuffKeys > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_6B)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(SokuLib::ACTION_6B);
			return true;
		}*/

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

		/*if (
			(hKeys > 0 || hBuffKeys > 0) &&
			this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_6B_HAMMER)
		) {
			this->renderInfos.zRotation = 0.0;
			this->setAction(ACTION_6B_HAMMER);
			return true;
		}*/

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
			(this->collisionType == 0 || this->collisionType == 3)
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

	if (this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_5C)) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_5C);
		return true;
	}
	return false;
}

bool Tewi::_canUseCard(int id)
{
	if (id == 200 && this->isGrounded() && !this->_hammer)
		return true;
	return false;
}

bool Tewi::_useSkillCard(int id)
{
	if (!this->_canUseCard(id))
		return false;
	if (this->gameData.sequenceData->actionLock > 50)
		return false;
	/*((((((uVar8 = (param_1->base).base.frameState.actionId, 299 < (short)uVar8 && (iVar10 = (param_1->base).field11_0x190, iVar10 != 3)) && (iVar10 != 0)) || ((short)uVar8 < 300)))))) {
		pCVar9 = HandData_operator[]((mHandData *)pDVar1,0);
		switch(pCVar9->id) {
		case 100:
			if (param_1->field_0x800 == '\0') {
				*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
				param_1->field_0x800 = 1;
			}
			FUN_00483ce0();
			onSkillUpgrade(param_1);
			cancelToSkill(param_1,500,uVar12);
			return;
		case 0x65:
			if (param_1->field_0x801 == '\0') {
				*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
				param_1->field_0x801 = 1;
			}
			FUN_00483ce0();
			onSkillUpgrade(param_1);
			cancelToSkill(param_1,0x208,uVar12);
			return;
		case 0x66:
			if (param_1->field_0x802 == '\0') {
				*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
				param_1->field_0x802 = 1;
			}
			FUN_00483ce0();
			onSkillUpgrade(param_1);
			cancelToSkill(param_1,0x21c,uVar12);
			return;
		case 0x67:
			if (0 < *(short *)((int)&param_1[1].base.base.vtable + 2)) {
				if (param_1->field_0x803 == '\0') {
					*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
					param_1->field_0x803 = 1;
				}
				FUN_00483ce0();
				onSkillUpgrade(param_1);
				cancelToSkill(param_1,0x230,uVar12);
				return;
			}
			break;
		case 0x68:
			if (param_1->field_0x800 == '\0') {
				*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
				param_1->field_0x800 = 1;
			}
			FUN_00483ce0();
			onSkillUpgrade(param_1);
			cancelToSkill(param_1,0x1f9,uVar12);
			return;
		case 0x69:
			if (param_1->field_0x801 == '\0') {
				*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
				param_1->field_0x801 = 1;
			}
			FUN_00483ce0();
			onSkillUpgrade(param_1);
			cancelToSkill(param_1,0x20d,uVar12);
			return;
		case 0x6a:
			if (param_1->field_0x802 == '\0') {
				*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
				param_1->field_0x802 = 1;
			}
			FUN_00483ce0();
			onSkillUpgrade(param_1);
			cancelToSkill(param_1,0x221,uVar12);
			return;
		case 0x6b:
			if (param_1->field_0x803 == '\0') {
				*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
				param_1->field_0x803 = 1;
			}
			FUN_00483ce0();
			onSkillUpgrade(param_1);
			if (2 < *(short *)&param_1[1].base.base.vtable) {
				cancelToSkill(param_1,0x236,uVar12);
				return;
			}
			cancelToSkill(param_1,0x238,uVar12);
			return;
		case 0x6c:
			if (param_1->field_0x800 == '\0') {
				*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
				param_1->field_0x800 = 1;
			}
			FUN_00483ce0();
			onSkillUpgrade(param_1);
			cancelToSkill(param_1,0x1fe,uVar12);
			return;
		case 0x6d:
			if (param_1->field_0x801 == '\0') {
				*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
				param_1->field_0x801 = 1;
			}
			FUN_00483ce0();
			onSkillUpgrade(param_1);
			cancelToSkill(param_1,0x212,uVar12);
			return;
		case 0x6e:
			if (param_1->field_0x802 == '\0') {
				*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
				param_1->field_0x802 = 1;
			}
			FUN_00483ce0();
			onSkillUpgrade(param_1);
			cancelToSkill(param_1,0x226,uVar12);
			return;
		case 0x6f:
			if (0 < *(short *)((int)&param_1[1].base.base.vtable + 2)) {
				if (param_1->field_0x803 == '\0') {
					*(int *)&param_1->field_0x4c8 = *(int *)&param_1->field_0x4c8 + 1;
					param_1->field_0x803 = 1;
				}
				FUN_00483ce0();
				onSkillUpgrade(param_1);
				cancelToSkill(param_1,0x23a,uVar12);
				return;
			}
		}*/
	return false;
}

bool Tewi::_useSpellCard(int id)
{
	if (this->gameData.sequenceData->actionLock > 100)
		return false;
	if (this->inputData.keyInput.horizontalAxis != 0)
		return false;
	if (this->inputData.keyInput.verticalAxis != 0)
		return false;

	if (
		this->frameState.actionId >= SokuLib::ACTION_5A &&
		(this->collisionType == 0 || this->collisionType == 3)
	)
		return false;

	this->useSpellCard(400 + id, this->frameState.actionId);
	return true;
}

void Tewi::_processInputsGrounded()
{
	if (
		(this->inputData.keyInput.spellcard != 0 && this->inputData.keyInput.spellcard < 3) ||
		this->inputData.bufferedKeyInput.spellcard != 0 &&
		this->unknown524 == 0 &&
		this->canActivateCard(0) &&
		this->unknown836 == 0 &&
		this->_canUseCard(this->handInfo.hand[0].id)
	) {
		if (this->useSystemCard(this->gameData.sequenceData->actionLock))
			return;

		auto &card = this->handInfo.hand[0];

		if (100 <= card.id && card.id < 200 && this->_useSkillCard(card.id))
			return;
		if (200 <= card.id && card.id < 300 && this->_useSpellCard(card.id))
			return;
	}

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

	if (this->handleCardSwitch())
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
