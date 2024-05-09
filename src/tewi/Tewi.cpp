//
// Created by PinkySmile on 08/05/24.
//

#define _USE_MATH_DEFINES
#include "Tewi.hpp"
#include "TewiObjectFactory.hpp"
#include "GameObjectList.hpp"

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
		this->speed.y = JUMP_SPEED_Y;
		this->gravity = {0, JUMP_GRAVITY};
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
		this->updateGroundMovement(-WALK_SPEED);
		this->advanceFrame();
		break;
	case SokuLib::ACTION_NEUTRAL_JUMP:
		this->_jumpUpdate(0);
		break;
	case SokuLib::ACTION_FORWARD_JUMP:
		this->_jumpUpdate(JUMP_SPEED_X);
		break;
	case SokuLib::ACTION_BACKWARD_JUMP:
		this->_jumpUpdate(-JUMP_SPEED_X);
		break;
	case SokuLib::ACTION_FALLING:
		if (this->gravity.y == 0)
			this->gravity.y = FALLING_GRAVITY;
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
	case SokuLib::ACTION_FORWARD_DASH:
		this->applyGroundMechanics();
		this->groundDashCount = 1;
		if (TOP_DASH_SPEED < this->speed.x) {
			this->speed.x -= 0.5;
			if (this->speed.x < TOP_DASH_SPEED)
				this->speed.x = TOP_DASH_SPEED;
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
			this->dashTimer++;
			if ((this->direction * this->inputData.keyInput.horizontalAxis < 1 && this->dashTimer > 5) || this->dashTimer > MAX_DASH_HOLD) {
				this->setAction(SokuLib::ACTION_GROUNDDASH_RECOVERY);
				return;
			}
		}
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->updateGroundMovement(INITIAL_DASH_SPEED);
			this->createEffect(0x7d, (float)(this->direction * 0x50) + this->position.x, this->position.y + 80, this->direction,1);
			this->createEffect(0x7e, this->position.x, this->position.y + 80, this->direction, 1);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
		}
		break;
	case SokuLib::ACTION_BACKDASH:
		if (this->frameState.sequenceId == 0 || frameState.sequenceId == 3)
			this->applyGroundMechanics();
		if (this->frameState.sequenceId == 3) {
			this->speed.x += 2;
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
			this->updateGroundMovement(-17.0);
			this->speed.y = 3.5;
			this->gravity.y = 0.75;
			this->createEffect(0x7d, this->position.x, this->position.y + 80.0, -this->direction, 1);
			this->createEffect(0x7e, this->position.x, this->position.y + 80.0, -this->direction, 1);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
			return;
		}
		break;
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
			this->updateGroundMovement(FAD_SPEED_X);
			this->speed.y = FAD_SPEED_Y;
			this->gravity.y = FAD_GRAVITY;
			this->createEffect(0x7D, (float)(this->direction * 0x50) + this->position.x, this->position.y + 110.0, this->direction,1);
			this->createEffect(0x7E, this->position.x, this->position.y + 110.0, this->direction, 1);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
		}
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 2) {
			this->Unknown487C20();
			return;
		}
		break;
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
			this->updateGroundMovement(BAD_SPEED_X);
			this->speed.y = BAD_SPEED_Y;
			this->gravity.y = BAD_GRAVITY;
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
					this->flightAngle += FLIGHT_TURN_SUNNY;
				else
					this->flightAngle += FLIGHT_TURN;
			}
			if (this->flightAngleDiff < 0) {
				if (this->weatherId == SokuLib::WEATHER_SUNNY)
					this->flightAngle -= FLIGHT_TURN_SUNNY;
				else
					this->flightAngle -= FLIGHT_TURN;
			}
			this->speed.x = cos(this->flightAngle * M_PI / 180) * this->flightSpeed;
			this->speed.y = sin(this->flightAngle * M_PI / 180) * this->flightSpeed;
			if (this->position.y > 680 && this->speed.y > 0)
				this->speed.y = 0.0;
			this->flightSpeed += 0.3;
			if (this->flightSpeed > FLIGHT_SPEED)
				this->flightSpeed = FLIGHT_SPEED;
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
	case SokuLib::ACTION_NEUTRAL_HIGH_JUMP:
		this->_highJumpUpdate(0, HIGH_JUMP_SPEED_NEUTRAL_Y, HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_FORWARD_HIGH_JUMP:
		this->_highJumpUpdate(HIGH_JUMP_SPEED_X, HIGH_JUMP_SPEED_Y, HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_BACKWARD_HIGH_JUMP:
		this->_highJumpUpdate(-HIGH_JUMP_SPEED_X, HIGH_JUMP_SPEED_Y, HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH:
		this->_highJumpUpdate(N_HIGH_JUMP_FD_SPEED_X, HIGH_JUMP_SPEED_NEUTRAL_Y, HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH:
		this->_highJumpUpdate(F_HIGH_JUMP_FD_SPEED_X, HIGH_JUMP_SPEED_Y, HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_2C:
		this->applyGroundMechanics();
		if (this->frameState.sequenceId < 2 && this->inputData.keyInput.c == 0)
			this->chargedAttack = false;
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
		}
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 1) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseFrame == 0) {
			/*if (this->frameState.poseId == 2 && this->chargedAttack) {
				this->nextSequence();
				this->createEffect(0x3e,(float)(this->direction * 0xc) + this->position.x, this->position.y + 172.0, this->direction,1);
			}*/
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 4) {
				float hammerParams[2] = {80, 30};

				this->consumeSpirit(200, 0x3c);
				this->collisionType = 1;
				this->addCardMeter(50);
				this->playSFX(0);
				this->createObject(800, (this->direction * 40) + this->position.x, this->position.y + 150, this->direction, 1, hammerParams, 2);
			}
		}/*
		if (this->frameState.sequenceId != 1)
			return;
		if (this->frameState.poseFrame != 0)
			return;
		if (this->frameState.poseId != 3)
			return;
		FUN_0047a9e0(this,200,0x3c);
		(this->base).field11_0x190 = 1;
		FUN_00487870(0x32);
		pPVar5 = (this->base).gameData.opponent;
		fVar19 = (float10)FUN_00409760((pPVar5->base).base.state.position.y - this->state.position.y,(float)(int)this->state.direction * ((pPVar5->base).base.state.position.x - this->state.position.x));
		lVar20 = floatToInteger(fVar19);
		*(short *)&this->field_0x7d4 = -(short)lVar20;
		if ((short)-(short)lVar20 < -10) {
			*(undefined2 *)&this->field_0x7d4 = 0xfff6;
		}
		if (10 < *(short *)&this->field_0x7d4) {
			*(undefined2 *)&this->field_0x7d4 = 10;
		}
		Player_mPlayPlayerSFX(this,2);
		fStack_350 = 50.0;
		uStack_34c = 0xc0800000;
		uStack_348 = 0;
		bVar3 = this->state.direction;
		mmObjectHandler_mSpawnBullet(this,0x321,(float)((char)bVar3 * 0x74) + this->state.position.x,this->state.position.y + 105.0,(uint)bVar3,'\x01',&fStack_350,3);
		fStack_350 = -50.0;
		uStack_34c = 0x40800000;
		uStack_348 = 0;
		bVar3 = this->state.direction;
		mmObjectHandler_mSpawnBullet(this,0x321,(float)((char)bVar3 * 0x74) + this->state.position.x,this->state.position.y + 105.0,(uint)bVar3,'\x01',&fStack_350,3);*/
		break;
	default:
		this->updateDefaultBehavior();
	}
}

bool Tewi::initializeAction()
{
	switch (this->frameState.actionId) {
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
	default:
		Player::initializeAction();
		break;
	}
	return true;
}

void Tewi::processInputsGrounded()
{
	if ((this->inputData.keyInput.spellcard != 0 && this->inputData.keyInput.spellcard < 3) || this->inputData.bufferedKeyInput.spellcard != 0) {
		if (this->canActivateCard(0) && this->unknown836 == 0) {
			if (this->useSystemCard(this->gameData.sequenceData->moveLock))
				return;
		}
	}
	if ((this->inputData.keyInput.c != 0 && this->inputData.keyInput.c < 3) || this->inputData.bufferedKeyInput.c != 0) {
		this->setAction(SokuLib::ACTION_2C);
	}
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

		if (frameFlags.cancellable || frameFlags.highJumpCancellable) {
			if (this->unknown4C4 == 0 && this->isOnGround()) {
				if (frameFlags.highJumpCancellable && this->gameData.sequenceData->moveLock > 9 && this->handleHJ())
					return;
				if (this->handleGroundBE())
					return;
				if (this->handleHJInput(this->gameData.sequenceData->moveLock, frameFlags.highJumpCancellable))
					return;
				if (this->handleGroundDash(this->gameData.sequenceData->moveLock, frameFlags.highJumpCancellable))
					return;
			} else {
				if (this->handleAirBE())
					return;

				auto allowedAirMoves = (this->weatherId == SokuLib::WEATHER_TEMPEST) + 2;

				if (this->handleFwdAirDash(this->gameData.sequenceData->moveLock, frameFlags.highJumpCancellable, allowedAirMoves, 2))
					return;
				if (this->handleBackAirDash(this->gameData.sequenceData->moveLock, frameFlags.highJumpCancellable, allowedAirMoves, 2))
					return;
				if (this->handleNormalFlight(this->gameData.sequenceData->moveLock, frameFlags.highJumpCancellable, allowedAirMoves))
					return;
			}
		}
	}
	if (this->stopwatchTimeLeft != 0)
		return;
	if (!frameFlags.cancellable)
		return;
	if (this->unknown524 == 0) {
		if (this->isGrounded()) {
			this->processInputsGrounded();
		}
	}
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