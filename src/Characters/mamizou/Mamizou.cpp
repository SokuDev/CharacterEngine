//
// Created by PinkySmile on 08/05/24.
//

#include <complex>
#include <map>
#include <dinput.h>
#include "Mamizou.hpp"
#include "MamizouObjectFactory.hpp"
#include "GameObjectList.hpp"

#ifndef _DEBUG
#define printf(...)
#endif

class MamizouGameObjectList : public GameObjectList<MamizouObjectFactory> {
private:
	SokuLib::List<SokuLib::v2::GameObject *> _mergedList;
	SokuLib::v2::Player &_other;
	Mamizou &_me;

public:
	inline MamizouGameObjectList(Mamizou &player, SokuLib::v2::Player &other) :
		GameObjectList<MamizouObjectFactory>(&player),
		_other(other),
		_me(player)
	{
	}
	~MamizouGameObjectList() override = default;

	void restoreAlly(SokuLib::v2::Player *p)
	{
		for (auto obj : this->_other.objectList->getList())
			obj->gameData.ally = p;
	}

	void clear() override
	{
		GameObjectList<MamizouObjectFactory>::clear();
		this->_other.objectList->clear();
	}

	void updatePhysics() override
	{
		GameObjectList<MamizouObjectFactory>::updatePhysics();
		this->_other.objectList->updatePhysics();
	}

	void update() override
	{
		GameObjectList<MamizouObjectFactory>::update();
		this->restoreAlly(&this->_other);
		if (this->_me._transformed)
			this->_me._preTransformCall();
		this->_other.objectList->update();
		if (this->_me._transformed)
			this->_me._postTransformCall();
		this->restoreAlly(this->_player);
	}

	void render1(char layer) override
	{
		GameObjectList<MamizouObjectFactory>::render1(layer);
		this->_other.objectList->render1(layer);
	}

	void render2() override
	{
		GameObjectList<MamizouObjectFactory>::render2();
		this->_other.objectList->render2();
	}

	void updateCamera() override
	{
		GameObjectList<MamizouObjectFactory>::updateCamera();
		this->_other.objectList->updateCamera();
	}

	void replaceOpponent(SokuLib::v2::Player *a0, SokuLib::v2::Player *a1) override
	{
		GameObjectList<MamizouObjectFactory>::replaceOpponent(a0, a1);
		this->_other.objectList->replaceOpponent(a0, a1);
	}

	SokuLib::List<SokuLib::v2::GameObject *> &getList() override
	{
		auto &myList = GameObjectList<MamizouObjectFactory>::getList();
		auto &otherList = this->_other.objectList->getList();

		for (size_t i = this->_mergedList.size(); i < myList.size() + otherList.size(); i++)
			this->_mergedList.push_back(nullptr);
		for (size_t i = this->_mergedList.size(); i > myList.size() + otherList.size(); i--)
			this->_mergedList.pop_back();

		auto it = this->_mergedList.begin();

		for (auto t : myList) {
			*it = t;
			it++;
		}
		for (auto t : otherList) {
			*it = t;
			it++;
		}
		return this->_mergedList;
	}
};

Mamizou::Mamizou(SokuLib::PlayerInfo &info) :
	SokuLib::v2::Player(info)
{
	if (info.palette >= 0) {
		SokuLib::PlayerInfo extra = info.isRight ? SokuLib::leftPlayerInfo : SokuLib::rightPlayerInfo;

		extra.isRight = info.isRight;
		extra.palette = -info.palette - 1;
		extra.effectiveDeck.clear();
		this->_transformPlayer = SokuLib::v2::GameDataManager::createPlayer(extra);
		this->_transformPlayer->initialize();
		this->_transformPlayer->setAction(SokuLib::ACTION_IDLE);
		this->objectList = new MamizouGameObjectList(*this, *this->_transformPlayer);
	} else {
		this->objectList = new GameObjectList<MamizouObjectFactory>(this);
		this->_init = true;
	}
}

bool Mamizou::_checkDashSlide()
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

void Mamizou::_jumpUpdate(float xSpeed)
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

void Mamizou::_highJumpUpdate(float xSpeed, float ySpeed, float gravity)
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

void Mamizou::update()
{
	if (!this->_init) {
		this->_init = true;
		this->_transformPlayer->gameData.opponent = this->gameData.opponent;
	}
	//if (SokuLib::checkKeyOneshot(DIK_F1, false, false, false)) {
	//	if (!this->_transformed) {
	//		this->setAction(SokuLib::ACTION_IDLE);
	//		this->_transform();
	//	} else
	//		this->_unTransform();
	//}
	if (this->_transformPlayer && !this->_transformed && (this->frameState.actionId < 50 || this->frameState.actionId >= 200)) {
		this->setAction(SokuLib::ACTION_IDLE);
		this->_transform();
	}
	if (this->_transformed) {
		this->_preTransformCall();
		this->_transformPlayer->update();
		this->_postTransformCall();
		return;
	}
	this->spinRotationCenter.x = 0;
	this->spinRotationCenter.y = 57;
	if (this->hitStop)
		return;
	if (this->gameData.opponent->timeStop)
		return;

	switch (this->frameState.actionId) {
	case SokuLib::ACTION_IDLE:
		this->applyGroundMechanics();
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
		this->updateGroundMovement(FRONT_WALK_SPEED);
		this->advanceFrame();
		break;
	case SokuLib::ACTION_WALK_BACKWARD:
		this->applyGroundMechanics();
		this->updateGroundMovement(BACK_WALK_SPEED);
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
			this->speed.x = 12;
			this->gravity.y = 1;
			this->speed.y = 10;
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
			this->speed.x = -12;
			this->gravity.y = 1;
			this->speed.y = 10;
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
			if (TOP_BE6_SPEED < this->speed.x) {
				this->speed.x -= 0.25;
				if (this->speed.x < TOP_BE6_SPEED)
					this->speed.x = TOP_BE6_SPEED;
			}
		} else if (TOP_DASH_SPEED < this->speed.x) {
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
			if (this->inputData.keyInput.verticalAxis < 0 && this->frameState.actionId == SokuLib::ACTION_FORWARD_DASH) {
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
				this->updateGroundMovement(INITIAL_BE6_SPEED);
			else
				this->updateGroundMovement(INITIAL_DASH_SPEED);
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
				this->speed.x += BE4_DECEL;
			else
				this->speed.x += BACKDASH_DECEL;
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
				this->updateGroundMovement(BE4_IMPULSE);
				this->speed.y = BE4_IMPULSE_UP;
				this->gravity.y = BE4_GRAVITY;
			} else {
				this->updateGroundMovement(BACKDASH_IMPULSE);
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
					this->flightAngle -= FLIGHT_TURN ;
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
	case SokuLib::ACTION_BE2:
	case SokuLib::ACTION_NEUTRAL_HIGH_JUMP:
		this->_highJumpUpdate(0, HIGH_JUMP_SPEED_NEUTRAL_Y, HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_FORWARD_HIGH_JUMP:
		this->_highJumpUpdate(HIGH_JUMP_SPEED_X, HIGH_JUMP_SPEED_Y, HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_BE1:
	case SokuLib::ACTION_BACKWARD_HIGH_JUMP:
		this->_highJumpUpdate(-HIGH_JUMP_SPEED_X, HIGH_JUMP_SPEED_Y, HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH:
		this->_highJumpUpdate(N_HIGH_JUMP_FD_SPEED_X, HIGH_JUMP_SPEED_NEUTRAL_Y, HIGH_JUMP_GRAVITY);
		break;
	case SokuLib::ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH:
		this->_highJumpUpdate(F_HIGH_JUMP_FD_SPEED_X, HIGH_JUMP_SPEED_Y, HIGH_JUMP_GRAVITY);
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

	default: {
		int old = this->frameState.poseFrame;

		this->updateDefaultBehavior();
		// Fail-safe to prevent softlocks
		if (old == this->frameState.poseFrame && this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
	}}
}

bool Mamizou::setAction(short action)
{
	printf("Mamizou::setAction(%i)\n", action);
	if (this->_transformed && action >= 50 && action < 150)
		this->_unTransform();
	if (this->_transformed) {
		if (this->_forward)
			this->_transformPlayer->setAction(action);
		return true;
	}
	return SokuLib::v2::Player::setAction(action);
}

void Mamizou::initializeAction()
{
	if (this->_transformed) {
		if (this->frameState.actionId >= 50 && this->frameState.actionId < 150)
			Player::initializeAction();
		if (this->_forward) {
			this->_preTransformCall();
			this->_transformPlayer->initializeAction();
			this->_postTransformCall();
		}
		return;
	}
	switch (this->frameState.actionId) {
	case SokuLib::ACTION_WALK_FORWARD:
		this->speed.x = FRONT_WALK_SPEED;;
		break;
	case SokuLib::ACTION_WALK_BACKWARD:
		this->speed.x = BACK_WALK_SPEED;
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
	default:
		Player::initializeAction();
		break;
	}
}

bool Mamizou::_processAAirborne()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;

	if (this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL))
		return false;

	return false;
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
	return false;
}

bool Mamizou::_processBAirborne()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;

	if ((this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)) || this->currentSpirit < 200)
		return false;

	return false;
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
	return false;
}

bool Mamizou::_processCAirborne()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;

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

bool Mamizou::_useSkill(bool input, unsigned char id, unsigned short action)
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

bool Mamizou::_processSkillsAirborne()
{
	if (
		(
			this->frameState.actionId >= SokuLib::ACTION_5A &&
			(this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)
		) ||
		this->currentSpirit < 200
	)
		return false;
	return false;
}

void Mamizou::_processInputsAirborne()
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

bool Mamizou::_processAGrounded()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;
	short dashAction = SokuLib::ACTION_66A;

	return false;

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
		std::abs(this->position.x - this->gameData.opponent->position.x) >= FAR_RANGE &&
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

	short action = std::abs(this->position.x - this->gameData.opponent->position.x) >= FAR_RANGE ? SokuLib::ACTION_f5A : SokuLib::ACTION_5A;
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
		action = SokuLib::ACTION_5AAAA;
		ok = true;
		break;
	case SokuLib::ACTION_5AAAA:
		return false;
	}

	if (this->gameData.sequenceData->actionLock <= this->getMoveLock(action) || ok) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(action);
		return true;
	}
	return false;
}

bool Mamizou::_processBGrounded()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;
	short dashAction = SokuLib::ACTION_66B;

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

	return false;

	if ((this->frameState.actionId >= SokuLib::ACTION_5A && (this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)) || this->currentSpirit < 200)
		return false;

	if (
		(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_2B)
		) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_2B);
		return true;
	}

/*	if (
		(hKeys > 0 || hBuffKeys > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(ACTION_6B)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(ACTION_6B);
		return true;
	}*/

	if (this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_5B)) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_5B);
		return true;
	}
	return false;
}

bool Mamizou::_processCGrounded()
{
	auto hKeys = this->inputData.keyInput.horizontalAxis * this->direction;
	auto hBuffKeys = this->inputData.bufferedKeyInput.horizontalAxis * this->direction;
	short dashAction = SokuLib::ACTION_66C;

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

bool Mamizou::_processSkillsGrounded()
{
	if (
		(
			this->frameState.actionId >= SokuLib::ACTION_5A &&
			(this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)
		) ||
		this->currentSpirit < 200
	)
		return false;
	return false;
}

bool Mamizou::_canUseCard(int id)
{
	if (id < 100)
		return true;

	return false;
}

bool Mamizou::_useSkillCard(int id)
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

bool Mamizou::_useSpellCard(int id)
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
	if (!this->isGrounded())
		id += 50;
	this->useSpellCard(id, this->gameData.sequenceData->moveLock);
	return true;
}

void Mamizou::_processInputsGrounded()
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

void Mamizou::handleInputs()
{
	auto frameFlags = this->gameData.frameData->frameFlags;

	if (this->handleCardSwitch())
		return;

	if (this->_transformed) {
		this->_preTransformCall();
		this->_transformPlayer->handleInputs();
		this->_postTransformCall();
		return;
	}
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
	if (!frameFlags.cancellable)
		return;
	if (this->isGrounded())
		this->_processInputsGrounded();
	else
		this->_processInputsAirborne();
}

void Mamizou::VUnknown58()
{
	if (this->_transformed) {
		this->_preTransformCall();
		this->_transformPlayer->VUnknown58();
		this->_postTransformCall();
		return;
	}
}

void Mamizou::VUnknown5C()
{
	if (this->_transformed) {
		this->_preTransformCall();
		this->_transformPlayer->VUnknown5C();
		this->_postTransformCall();
		return;
	}
}

bool Mamizou::VUnknown60(int a)
{
	if (this->_transformed) {
		this->_preTransformCall();
		bool b = this->_transformPlayer->VUnknown60(a);
		this->_postTransformCall();
		return b;
	}
	return false;
}

void Mamizou::render()
{
	if (this->_transformed)
		this->_transformPlayer->render();
	else
		SokuLib::v2::Player::render();
}

void Mamizou::updatePhysics()
{
	if (this->_transformed) {
		this->_preTransformCall();
		this->_transformPlayer->updatePhysics();
		this->_postTransformCall();
	}
	SokuLib::v2::Player::updatePhysics();
}

void Mamizou::render2()
{
	if (this->_transformed)
		this->_transformPlayer->render2();
	else
		AnimationObject::render2();
}

void Mamizou::onRenderEnd()
{
	if (this->_transformed)
		this->_transformPlayer->onRenderEnd();
	else
		AnimationObject::onRenderEnd();
}

void Mamizou::setActionSequence(short action, short sequence)
{
	if (this->_transformed)
		this->_transformPlayer->setActionSequence(action, sequence);
	else
		GameObjectBase::setActionSequence(action, sequence);
}

void Mamizou::setSequence(short sequence)
{
	if (this->_transformed)
		this->_transformPlayer->setSequence(sequence);
	else
		GameObjectBase::setSequence(sequence);
}

void Mamizou::resetSequence()
{
	if (this->_transformed)
		this->_transformPlayer->resetSequence();
	else
		GameObjectBase::resetSequence();
}

bool Mamizou::nextSequence()
{
	if (this->_transformed)
		return this->_transformPlayer->nextSequence();
	else
		return GameObjectBase::nextSequence();
}

void Mamizou::prevSequence()
{
	if (this->_transformed)
		this->_transformPlayer->prevSequence();
	else
		GameObjectBase::prevSequence();
}

void Mamizou::setPose(short pose)
{
	if (this->_transformed)
		this->_transformPlayer->setPose(pose);
	else
		GameObjectBase::setPose(pose);
}

bool Mamizou::nextPose()
{
	if (this->_transformed)
		return this->_transformPlayer->nextPose();
	else
		return GameObjectBase::nextPose();
}

void Mamizou::prevPose()
{
	if (this->_transformed)
		this->_transformPlayer->prevPose();
	else
		GameObjectBase::prevPose();
}

void Mamizou::applyTransform()
{
	if (this->_transformed)
		this->_transformPlayer->applyTransform();
	else
		Player::applyTransform();
}

void Mamizou::initialize()
{
	if (this->_transformed) {
		this->_preTransformCall();
		this->_transformPlayer->initialize();
		this->_postTransformCall();
	} else
		Player::initialize();
}

bool Mamizou::VUnknown48()
{
	if (this->_transformed) {
		this->_preTransformCall();
		bool b = this->_transformPlayer->VUnknown48();
		this->_postTransformCall();
		return b;
	} else
		return Player::VUnknown48();
}

bool Mamizou::VUnknown4C(int a)
{
	if (this->_transformed) {
		this->_preTransformCall();
		bool b = this->_transformPlayer->VUnknown4C(a);
		this->_postTransformCall();
		return b;
	} else
		return Player::VUnknown4C(a);
}

void Mamizou::_preTransformCall()
{
	memcpy(&this->_transformPlayer->inputData.keyInput, &this->inputData.keyInput, sizeof(this->inputData.keyInput));
	memcpy(&this->_transformPlayer->inputData.bufferedKeyInput, &this->inputData.bufferedKeyInput, sizeof(this->inputData.bufferedKeyInput));
	this->_transformPlayer->inputData.keyUpA = this->inputData.keyUpA;
	this->_transformPlayer->inputData.keyUpB = this->inputData.keyUpB;
	this->_transformPlayer->inputData.keyUpC = this->inputData.keyUpC;
	this->_transformPlayer->inputData.keyUpD = this->inputData.keyUpD;
	this->_transformPlayer->inputData.keyUpE = this->inputData.keyUpE;
	this->_transformPlayer->inputData.keyUpF = this->inputData.keyUpF;
	this->_transformPlayer->inputData.bufferTimer = this->inputData.bufferTimer;
	this->_transformPlayer->inputData.unknown7AD[0] = this->inputData.unknown7AD[0];
	this->_transformPlayer->inputData.unknown7AD[1] = this->inputData.unknown7AD[1];
	this->_transformPlayer->inputData.unknown7AD[2] = this->inputData.unknown7AD[2];
	this->_transformPlayer->inputData.commandInputBuffer = this->inputData.commandInputBuffer;
	this->_transformPlayer->inputData.movementCombination = this->inputData.movementCombination;
	this->_transformPlayer->inputData.commandCombination = this->inputData.commandCombination;
	this->_transformPlayer->hitStop = this->hitStop;
	this->_transformPlayer->weatherId = this->weatherId;

	this->_transformPlayer->HP = this->HP;
	this->_transformPlayer->redHP = this->redHP;
	this->_transformPlayer->spiritRegenDelay = this->spiritRegenDelay;
	this->_transformPlayer->currentSpirit = this->currentSpirit;
	this->_transformPlayer->maxSpirit = this->maxSpirit;
	this->_transformPlayer->direction = this->direction;
	this->_transformPlayer->collisionType = this->collisionType;
	this->_transformPlayer->collisionLimit = this->collisionLimit;
	this->_transformPlayer->gravity = this->gravity;
	this->_transformPlayer->speed = this->speed;
	this->_transformPlayer->position = this->position;
}

void Mamizou::_postTransformCall()
{
	if (this->_transformPlayer->frameState.actionId != this->frameState.actionId) {
		this->_forward = false;
		this->setAction(this->_transformPlayer->frameState.actionId);
		this->_forward = true;
	}
	this->gameData.sequenceData = this->_transformPlayer->gameData.sequenceData;
	this->gameData.frameData = this->_transformPlayer->gameData.frameData;
	this->frameState.actionId = this->_transformPlayer->frameState.actionId;
	this->skillIndex = this->_transformPlayer->skillIndex;

	this->HP = this->_transformPlayer->HP;
	this->redHP = this->_transformPlayer->redHP;
	this->spiritRegenDelay = this->_transformPlayer->spiritRegenDelay;
	this->currentSpirit = this->_transformPlayer->currentSpirit;
	this->maxSpirit = this->_transformPlayer->maxSpirit;
	this->direction = this->_transformPlayer->direction;
	this->speed = this->_transformPlayer->speed;
	this->position = this->_transformPlayer->position;
	this->gravity = this->_transformPlayer->gravity;
	this->collisionType = this->_transformPlayer->collisionType;
	this->collisionLimit = this->_transformPlayer->collisionLimit;
}

void Mamizou::_transform()
{
	this->_transformed = true;
	this->_savedFrameData = this->gameData.frameData;
	this->_savedSequenceData = this->gameData.sequenceData;
	if (this->isGrounded())
		this->_transformPlayer->setAction(SokuLib::ACTION_IDLE);
	else
		this->_transformPlayer->setAction(SokuLib::ACTION_FALLING);
	for (int i = 0; i < 50; i++) {
		float params[5];
		float x = this->position.x + SokuLib::rand(150) - 75;
		float y = this->position.y + SokuLib::rand(200);

		params[0] = std::atan2(this->position.y + 100 - y, this->position.x - x) + M_PI;
		params[1] = SokuLib::rand(200) / 100.f + 0.5;
		params[2] = SokuLib::rand(4);
		params[3] = SokuLib::rand(10) + 10;
		params[4] = SokuLib::rand(40) - 20;
		this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
	}
	this->playSFX(0);
}

void Mamizou::_unTransform()
{
	this->_transformed = false;
	this->gameData.frameData = this->_savedFrameData;
	this->gameData.sequenceData = this->_savedSequenceData;
	this->skillIndex = -1;
	for (int i = 0; i < 50; i++) {
		float params[5];
		float x = this->position.x + SokuLib::rand(150) - 75;
		float y = this->position.y + SokuLib::rand(200);

		params[0] = std::atan2(this->position.y + 100 - y, this->position.x - x) + M_PI;
		params[1] = SokuLib::rand(200) / 100.f + 0.5;
		params[2] = SokuLib::rand(4);
		params[3] = SokuLib::rand(10) + 10;
		params[4] = SokuLib::rand(40) - 20;
		this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
	}
	this->playSFX(0);
}

bool Mamizou::isTransformed() const
{
	return this->_transformed;
}

SokuLib::v2::Player *Mamizou::getTransformPlayer() const
{
	return this->_transformPlayer;
}

static void (__fastcall *og_FUN_0046a240)(SokuLib::v2::Player *);

void __fastcall FUN_0046a240(SokuLib::v2::Player *This)
{
	og_FUN_0046a240(This);
	if (This->characterIndex != SokuLib::CHARACTER_MAMIZOU)
		return;

	auto mamizou = reinterpret_cast<Mamizou *>(This);

	if (!mamizou->isTransformed())
		return;

	auto transformPlayer = mamizou->getTransformPlayer();

	og_FUN_0046a240(transformPlayer);
	memcpy(This->effectiveSkillLevel, transformPlayer->effectiveSkillLevel, sizeof(This->effectiveSkillLevel));
}

void Mamizou::hook()
{
	DWORD old;

	VirtualProtect((void *)0x46E07E, 5, PAGE_EXECUTE_READWRITE, &old);
	og_FUN_0046a240 = SokuLib::TamperNearJmpOpr(0x46E07E, FUN_0046a240);
	VirtualProtect((void *)0x46E07E, 5, old, &old);
}

void Mamizou::unhook()
{
	DWORD old;

	VirtualProtect((void *)0x46E07E, 5, PAGE_EXECUTE_READWRITE, &old);
	SokuLib::TamperNearJmpOpr(0x46E07E, og_FUN_0046a240);
	VirtualProtect((void *)0x46E07E, 5, old, &old);
}
