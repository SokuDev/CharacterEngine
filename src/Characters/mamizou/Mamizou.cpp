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

#define dashTimer gpShort[0]
#define flightTargetAngle gpShort[1]
#define flightAngleDiff gpShort[2]
#define flightTimer gpShort[3]
#define flightSpeed gpFloat[0]
#define flightAngle gpFloat[1]

#define SPD_5SC_BOOST 1
#define ATK_5SC_BOOST 1.1
#define ATK_1COST_BOOST 2
#define ATK_2COST_BOOST 1.75
#define ATK_3COST_BOOST 1.5
#define ATK_4COST_BOOST 1.25
#define ATK_5COST_BOOST 1
#define NB_ANIMALS 19

int __practiceEffect = -1;

static SokuLib::Action meleeActions[] = {
	SokuLib::ACTION_5A,
	SokuLib::ACTION_f5A,
	SokuLib::ACTION_6A,
	SokuLib::ACTION_2A,
	SokuLib::ACTION_3A,
	SokuLib::ACTION_66A,
	SokuLib::ACTION_j5A,
	SokuLib::ACTION_j6A,
	SokuLib::ACTION_j2A,
	SokuLib::ACTION_j8A,
	SokuLib::ACTION_4A,
};

class MamizouGameObjectList : public GameObjectList<MamizouObjectFactory> {
private:
	const std::set<std::pair<unsigned short, unsigned short>> &_restingActions;
	SokuLib::List<SokuLib::v2::GameObject *> _mergedList;
	SokuLib::v2::Player &_other;
	Mamizou &_me;

public:
	inline MamizouGameObjectList(Mamizou &player, SokuLib::v2::Player &other, const std::set<std::pair<unsigned short, unsigned short>> &restingActions) :
		GameObjectList<MamizouObjectFactory>(&player),
		_restingActions(restingActions),
		_other(other),
		_me(player)
	{
	}
	~MamizouGameObjectList() override = default;

	void restoreAlly(SokuLib::v2::Player *p) const
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
		if (this->_me._transformed) {
			this->_me._preTransformCall();
			this->_other.objectList->update();
			this->_me._postTransformCall();
		} else {
			this->_me._preUntransformCall();
			this->_other.objectList->update();
			this->_me._postUntransformCall();
		}
		this->restoreAlly(this->_player);
	}

	void render1(char layer) override
	{
		GameObjectList<MamizouObjectFactory>::render1(layer);
		if (this->_me._transformed)
			return this->_other.objectList->render1(layer);

		for (auto &o : this->_other.objectList->getList()) {
			if (this->_restingActions.contains({o->frameState.actionId, o->frameState.sequenceId}))
				continue;
			if (o->layer != layer)
				continue;
			if (o->unknown154 != nullptr)
				((void (__thiscall *)(void *))0x4389D0)(o->unknown154);
			if (o->tail)
				o->tail->render();
			o->render();
			if (o->unknown154 != nullptr)
				((void (__thiscall *)(void *))0x438B90)(o->unknown154);
		}
	}

	void render2() override
	{
		GameObjectList<MamizouObjectFactory>::render2();
		if (this->_me._transformed)
			return this->_other.objectList->render2();
		for (auto o : this->_list)
			if (!this->_restingActions.contains({o->frameState.actionId, o->frameState.sequenceId}))
				o->render2();
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

	void clearMyList()
	{
		for (auto obj : this->_list) {
			if (obj->frameState.actionId < 900)
				obj->lifetime = 0;
		}
		this->_list.erase(std::remove_if(this->_list.begin(), this->_list.end(), [](SokuLib::v2::GameObject *a){
			return a->lifetime == 0;
		}), this->_list.end());
		this->_objects.erase(std::remove_if(this->_objects.begin(), this->_objects.end(), [](std::unique_ptr<SokuLib::v2::GameObject> &a){
			return a->lifetime == 0;
		}), this->_objects.end());
	}
};

Mamizou::Mamizou(SokuLib::PlayerInfo &info) :
	SokuLib::v2::Player(info)
{
	if (info.palette >= 0) {
		SokuLib::PlayerInfo extra = info.isRight ? SokuLib::leftPlayerInfo : SokuLib::rightPlayerInfo;

		extra.isRight = info.isRight;
		if (extra.character != info.character)
			extra.palette = -info.palette - 1;
		else
			extra.palette = -extra.palette - 1;
		extra.effectiveDeck.clear();
		this->_transformPlayer = SokuLib::v2::GameDataManager::createPlayer(extra);
		for (auto obj : this->_transformPlayer->objectList->getList())
			this->_restingActions.emplace(obj->frameState.actionId, obj->frameState.sequenceId);
		this->objectList = new MamizouGameObjectList(*this, *this->_transformPlayer, this->_restingActions);
	} else {
		this->objectList = new GameObjectList<MamizouObjectFactory>(this);
		this->_init = true;
	}
	if (info.isRight == 0)
		__practiceEffect = -1;
}

Mamizou::~Mamizou()
{
	delete this->_transformPlayer;
	for (auto &card : this->_opponentSpellCards)
		SokuLib::textureMgr.remove(card.second);
	for (auto &card : this->_allOpponentSpellCards)
		SokuLib::textureMgr.remove(card.tex);
	for (auto &pair : this->_opponentSkillCards)
		for (auto tx : pair.second)
			if (tx)
				SokuLib::textureMgr.remove(tx);
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

void Mamizou::_transformCard(SokuLib::Card &card)
{
	char buffer[1024];
	std::pair<unsigned char, int> id = {200, 0};

	if (!this->_opponentSpellCards.empty()) {
		if (SokuLib::mainMode == SokuLib::BATTLE_MODE_PRACTICE && SokuLib::subMode == SokuLib::BATTLE_SUBMODE_PLAYING1 && __practiceEffect != -1) {
			id = this->_opponentSpellCards[__practiceEffect];
		} else {
			auto index = SokuLib::rand(this->_opponentSpellCards.size());

			if (index >= this->_opponentSpellCards.size())
				index = this->_opponentSpellCards.size() - 1;
			id = this->_opponentSpellCards[index];
		}
	}

	card.id = id.first + 100;
	card.cost = 2;
	card.sprite.setTexture2(id.second, 0, 0, 41, 65);
}

void Mamizou::_updateOpponentDebuffed()
{
	auto op = this->gameData.opponent;

	switch (op->frameState.actionId % 2) {
	case 0:
		op->advanceFrame();
		op->checkTurnAround();
		if (!op->isOnGround()) {
			op->speed.y -= op->gravity.y;
			if (op->inputData.keyInput.verticalAxis < 0 && op->speed.y < -5)
				op->speed.y = -5;
			if (op->inputData.keyInput.verticalAxis > 0)
				op->speed.y -= op->gravity.y;
			if (op->inputData.keyInput.horizontalAxis * op->direction > 0) {
				if (op->speed.x < 5)
					op->speed.x = 5;
			} else if (op->inputData.keyInput.horizontalAxis * op->direction < 0) {
				if (op->speed.x > -5)
					op->speed.x = -5;
			}
			op->renderInfos.zRotation = 0;
		} else {
			if (op->inputData.keyInput.verticalAxis < 0) {
				op->speed.y = 25;
				op->gravity.y = 0.8;
				if (op->inputData.keyInput.horizontalAxis * op->direction > 0)
					op->speed.x = 10;
				else if (op->inputData.keyInput.horizontalAxis * op->direction < 0)
					op->speed.x = -10;
				else
					op->speed.x = 0;
			} else {
				op->speed.y = 0;
				if (op->inputData.keyInput.horizontalAxis * op->direction > 0) {
					op->updateGroundMovement(7.5);
					op->renderInfos.zRotation = 10;
				} else if (op->inputData.keyInput.horizontalAxis * op->direction < 0) {
					op->updateGroundMovement(-7.5);
					op->renderInfos.zRotation = -10;
				} else {
					op->speed.x = 0;
					op->renderInfos.zRotation = 0;
				}
			}
		}
		break;
	case 1:
		if (op->hitStop) {
			op->gravity.y = 0.5;
			break;
		}
		if (op->frameState.currentFrame == 0) {
			op->speed.x = -op->unknown1A4;
			op->speed.y = op->unknown1A8;
		}
		if (op->advanceFrame())
			op->setAction(op->frameState.actionId - 1);
		op->speed.y -= op->gravity.y;
		break;
	}
}

void Mamizou::update()
{
	float params[3];

	if (!this->_init) {
		this->_init = true;
		this->_transformPlayer->gameData.opponent = this->gameData.opponent;
	}
	if (this->gameData.opponent->frameState.actionId >= 2000)
		this->_updateOpponentDebuffed();
	this->riverMistAdditionalSpeed = 0;
	//if (!this->_transformed && (this->frameState.actionId < 50 || this->frameState.actionId >= 150))
	//	this->_transform();
	if (SokuLib::checkKeyOneshot(DIK_F9, false, false, false) && SokuLib::mainMode == SokuLib::BATTLE_MODE_PRACTICE && SokuLib::subMode == SokuLib::BATTLE_SUBMODE_PLAYING1) {
		SokuLib::playSEWaveBuffer(SokuLib::SFX_MENU_CONFIRM);
		__practiceEffect = (__practiceEffect + 2) % (this->_opponentSpellCards.size() + 1) - 1;
		for (auto &card : this->handInfo.hand)
			if (card.id >= 300)
				card.id = 201;
	}
	for (auto &card : this->handInfo.hand)
		if (card.id == 201)
			this->_transformCard(card);
	if (this->_transformKind == KIND_DEBUFF_TIMER) {
		if (this->_transformTimer != 0)
			this->_transformTimer--;
		if (this->_transformTimer == 0 && this->frameState.actionId < SokuLib::ACTION_5A)
			this->_cleanupOpponentDebuff();
	}
	if (this->_transformed) {
		auto oldAction = this->frameState.actionId;

		if (this->_transformKind == KIND_SPELL_TIMER) {
			float params[9];
			float x = this->position.x + SokuLib::rand(100) - 50;
			float y = this->position.y + SokuLib::rand(60) + 15;

			params[0] = -M_PI_4 + SokuLib::rand(90) / 180.f * M_PI;
			params[1] = SokuLib::rand(250) / 200.f + 0.25;
			params[2] = SokuLib::rand(4);
			params[3] = SokuLib::rand(10) + 10;
			params[4] = SokuLib::rand(40) - 20;
			params[5] = 0.5;
			params[6] = 0.5;
			params[7] = 0;
			params[8] = 0;
			this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
			this->handInfo.cardGauge = 0;
			if (this->_transformTimer != 0)
				this->_transformTimer--;
		} else if (this->_transformKind == KIND_TIMER) {
			this->_transformTimer--;
			if (this->_transformTimer == 0) {
				this->_unTransform();
				this->setAction(this->isOnGround() ? ACTION_FORCE_TIMER_UNTRANSFORM : ACTION_FORCE_TIMER_UNTRANSFORM_AIR);
				this->_transformTimerDelay = 0;
				if (SokuLib::mainMode != SokuLib::BATTLE_MODE_PRACTICE || SokuLib::subMode != SokuLib::BATTLE_SUBMODE_PLAYING1)
					this->_transformTimer = TIMER_COOLDOWN_MAX;
				else
					this->_transformTimer = 1;
				this->_transformedCooldown = true;
				return;
			}
		} else if (this->_transformKind == KIND_SINGLE_HIT) {
			auto frameFlags = this->_transformPlayer->gameData.frameData->frameFlags;

			if (
				this->_transformPlayer->frameState.actionId >= SokuLib::ACTION_5A &&
				this->_transformPlayer->frameState.actionId &&
				frameFlags.cancellable &&
				this->_transformPlayer->collisionType != COLLISION_TYPE_NONE &&
				this->_transformPlayer->collisionType != COLLISION_TYPE_INVUL
			) {
				this->_unTransform();
				this->setAction(this->isOnGround() ? ACTION_a1_22b_UNTRANSFORM : ACTION_ja1_22b_UNTRANSFORM);
				return;
			}
		}
		this->_preTransformCall();
		this->_transformPlayer->update();
		this->_postTransformCall();
		if (this->_transformed && this->_transformKind == KIND_SINGLE_HIT && oldAction > this->frameState.actionId && oldAction >= SokuLib::ACTION_5A) {
			this->_unTransform();
			this->setAction(this->isOnGround() ? ACTION_a1_22b_UNTRANSFORM : ACTION_ja1_22b_UNTRANSFORM);
		}
		return;
	} else if (this->_transformPlayer) {
		this->_transformPlayer->position = this->position;
		this->_transformPlayer->speed = {0, 0};
		this->_preUntransformCall();
		this->_transformPlayer->update();
		this->_postUntransformCall();
		if (this->_neverTransformed)
			for (auto obj : this->_transformPlayer->objectList->getList())
				this->_restingActions.emplace(obj->frameState.actionId, obj->frameState.sequenceId);
	}
	if (
		SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN <= this->frameState.actionId && this->frameState.actionId <= SokuLib::ACTION_AIR_GUARD &&
		!this->blockObjectSpawned
	) {
		float yOffset;

		this->blockObjectSpawned = true;
		if (this->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_HIGH_HUGE_BLOCKSTUN)
			yOffset = 100;
		else if (this->frameState.actionId == SokuLib::ACTION_AIR_GUARD)
			yOffset = 150;
		else //if (SokuLib::ACTION_RIGHTBLOCK_LOW_SMALL_BLOCKSTUN <= this->frameState.actionId && this->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_LOW_HUGE_BLOCKSTUN)
			yOffset = 40;
		this->createObject(998, (float)(this->direction * 57) + this->position.x, this->position.y + yOffset, this->direction, 1);
	}
	this->spinRotationCenter.x = 0;
	this->spinRotationCenter.y = 115;

	if (this->_transformKind != KIND_DEBUFF_TIMER) {
		if (this->_transformedCooldown) {
			this->_transformTimer--;
			if (this->_transformTimer == 0) {
				this->_transformedCooldown = false;
				this->_transformTimerDelay = 0;
				this->_transformTimer = TIMER_MAX;
			}
		} else if (this->_transformTimerDelay)
			this->_transformTimerDelay--;
		else if (this->_transformTimer < TIMER_MAX)
			this->_transformTimer++;
	}

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
	case SokuLib::ACTION_BACKWARD_TECH:
		if (this->frameState.sequenceId == 0 || this->frameState.sequenceId == 3)
			this->applyGroundMechanics();
		else
			this->speed.y -= this->gravity.y;
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
		if (this->frameState.sequenceId == 1 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
			if (this->frameState.actionId == SokuLib::ACTION_FORWARD_TECH)
				this->speed.x = 11;
			else
				this->speed.x = -10;
			this->gravity.y = 0.70;
			this->speed.y = 9;
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
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
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
			this->checkTurnAround();
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
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
			this->updateGroundMovement(BAD_SPEED_X);
			this->speed.y = BAD_SPEED_Y;
			this->gravity.y = BAD_GRAVITY;
			this->createEffect(0x7D, (float)(this->direction * 0x50) + this->position.x, this->position.y + 110.0, this->direction,1);
			this->createEffect(0x7E, this->position.x, this->position.y + 110.0, this->direction, 1);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
		}
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 2) {
			this->checkTurnAround();
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
		if (
			this->frameState.currentFrame == 0 &&
			this->frameState.poseFrame == 0 &&
			this->frameState.poseId == 0
		) {
			if (this->frameState.sequenceId == 1) {
				SokuLib::playSEWaveBuffer(SokuLib::SFX_DASH);
				if (this->flightAngle >= 135.0 || this->flightAngle <= -135.0) {
					this->setSequence(6);
					return;
				}
				if (this->flightAngle > 45.0 || this->flightAngle < -45.0) {
					this->setSequence(4);
					return;
				}
			}
			if (this->frameState.sequenceId == 9)
				this->setSequence(5);
			if (this->frameState.sequenceId == 10 || this->frameState.sequenceId == 11)
				this->setAction(SokuLib::ACTION_FALLING);
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

		if (this->frameState.sequenceId == 9 || this->frameState.sequenceId == 10 || this->frameState.sequenceId == 11)
			this->speed.y = this->speed.y - this->gravity.y;
		if (this->frameState.sequenceId < 1 || this->frameState.sequenceId > 8) {
			if (!this->applyAirMechanics())
				return;
			this->position.y = this->getGroundHeight();
			this->speed.y = 0.0;
			this->gravity.y = 0.0;
			if (8 < (short)this->frameState.sequenceId) {
				this->setAction(SokuLib::ACTION_LANDING);
				this->resetForces();
				return;
			}
			this->resetRenderInfo();
			this->setAction(SokuLib::ACTION_HARDLAND);
			return;
		}
		this->flightTimer++;
		this->flightAngleDiff = this->flightTargetAngle - this->flightAngle;
		if (this->flightAngleDiff > 180)
			this->flightAngleDiff -= 360;
		if (this->flightAngleDiff < -180)
			this->flightAngleDiff += 360;
		if (this->flightAngle > 180)
			this->flightAngle -= 360;
		if (this->flightAngle < -180)
			this->flightAngle += 360;
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
		if (this->weatherId == SokuLib::WEATHER_SUNNY)
			this->consumeSpirit(4, 1);
		else
			this->consumeSpirit(8, 1);
		if (
			(this->frameState.sequenceId == 1 || this->frameState.sequenceId == 2) &&
			(this->flightAngle > 60.0 || this->flightAngle < -60.0)
		) {
			this->setSequence(3);
			return;
		}
		if (this->frameState.sequenceId == 4 || this->frameState.sequenceId == 5) {
			if (this->flightAngle <= 45.0 && this->flightAngle >= -45.0) {
				this->setSequence(1);
				return;
			}
			if (this->flightAngle >= 135.0 || this->flightAngle <= -135.0) {
				this->setSequence(6);
				return;
			}
		}
		if (
			(this->frameState.sequenceId == 6 || this->frameState.sequenceId == 7) &&
			this->flightAngle < 120.0 &&
			this->flightAngle > -120.0
		) {
			this->setSequence(8);
			return;
		}
		if (
			(this->inputData.keyInput.d != 0 || this->flightTimer <= 10) &&
			this->currentSpirit > 0
		)  {
			if (!this->applyAirMechanics())
				return;
			this->position.y = this->getGroundHeight();
			this->speed.y = 0.0;
			this->gravity.y = 0.0;
			if (8 < (short)this->frameState.sequenceId) {
				this->setAction(SokuLib::ACTION_LANDING);
				this->resetForces();
				return;
			}
			this->resetRenderInfo();
			this->setAction(SokuLib::ACTION_HARDLAND);
			return;
		}
		if (this->frameState.sequenceId == 1 || this->frameState.sequenceId == 2) {
			this->checkTurnAround();
			this->setSequence(9);
			return;
		}
		if (this->frameState.sequenceId == 3) {
			this->gpShort[0] = this->frameState.poseId;
			this->checkTurnAround();
			this->setSequence(9);
			this->setPose(this->gpShort[0]);
			return;
		}
		if (this->frameState.sequenceId == 4 || this->frameState.sequenceId == 5) {
			this->checkTurnAround();
			this->setSequence(10);
			return;
		}
		if (this->frameState.sequenceId == 6 || this->frameState.sequenceId == 7) {
			this->checkTurnAround();
			this->setSequence(11);
			return;
		}
		if (this->frameState.sequenceId == 8) {
			this->gpShort[0] = this->frameState.poseId;
			this->checkTurnAround();
			this->setSequence(11);
			this->setPose(this->gpShort[0]);
			return;
		}
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

	case SokuLib::ACTION_4A:
	case SokuLib::ACTION_5A:
	case SokuLib::ACTION_5AA:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		break;
	case SokuLib::ACTION_3A:
	case SokuLib::ACTION_5AAAA:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.a == 0)
			this->chargedAttack = false;
		if (this->frameState.sequenceId == 0) {
			if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0 && this->chargedAttack) {
				this->nextSequence();
				this->createEffect(0x3E, (float)(this->direction * 80) + this->position.x, this->position.y + 22.0, this->direction, 1);
			}
			if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
				this->playSFX(3);
				this->createObject(803, this->position.x + this->direction * 134, this->position.y, this->direction, 1);
			}
		} else if (this->frameState.sequenceId == 1) {
			if (this->frameState.poseId == 0 && this->frameState.currentFrame == 0) {
				this->setAction(SokuLib::ACTION_CROUCHED);
				return;
			}
			if (this->frameState.poseId == 1 && this->frameState.poseFrame == 0) {
				this->playSFX(3);
				this->createObject(803, this->position.x + this->direction * 134, this->position.y, this->direction, 1);
			}
		}
		break;
	case SokuLib::ACTION_6A:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.a == 0)
			this->chargedAttack = false;
		if (this->frameState.sequenceId == 0) {
			if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0 && this->chargedAttack) {
				this->nextSequence();
				this->createEffect(0x3E, (float)(this->direction * -80) + this->position.x, this->position.y + 122.0, this->direction, 1);
			}
			if (this->frameState.poseId == 6 && this->frameState.poseFrame == 0) {
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
				this->speed.x = 12.5;
			}
			if (this->frameState.poseId >= 6 && this->speed.x != 0)
				this->speed.x -= 0.25;
			if (this->frameState.poseId >= 7 && this->speed.x != 0)
				this->speed.x -= 0.25;
		} else if (this->frameState.sequenceId == 1) {
			if (this->frameState.poseId == 0 && this->frameState.currentFrame == 0) {
				this->setAction(SokuLib::ACTION_IDLE);
				return;
			}
			if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
				this->speed.x = 17.5;
			}
			if (this->frameState.poseId >= 3 && this->speed.x != 0)
				this->speed.x -= 0.25;
			if (this->frameState.poseId >= 4 && this->speed.x != 0)
				this->speed.x -= 0.25;
		}
		break;
	case SokuLib::ACTION_2A:
		this->applyGroundMechanics();
		if (this->_checkDashSlide())
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_LIGHT_ATTACK);
		break;
	case SokuLib::ACTION_5AAA:
	case SokuLib::ACTION_f5A:
	case SokuLib::ACTION_66A:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		if (this->speed.x > 0)
			this->speed.x -= 0.70;
		if (this->speed.x < 0)
			this->speed.x = 0;
		break;
	case SokuLib::ACTION_5AAAAA:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (!this->_transformPlayer)
			this->chargedAttack = false;
		if (this->frameState.sequenceId == 0 && this->frameState.poseId < 2 && this->frameState.poseFrame < 4 && this->inputData.keyInput.a == 0)
			this->chargedAttack = false;
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 1 && this->frameState.poseFrame == 4) {
			if (this->chargedAttack)
				this->createEffect(0x3E, (float)(this->direction * -80) + this->position.x, this->position.y + 122.0, this->direction, 1);
			else
				this->nextPose();
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
			params[0] = 0;
			params[1] = 15;
			for (int i = 0; i < 8; i++) {
				params[2] = SokuLib::rand(400) / 100;
				if (params[2] >= 4)
					params[2] = 3;
				this->createObject(806, this->position.x + SokuLib::cos(params[0]) * 50, this->position.y + 100 - SokuLib::sin(params[0]) * 50, this->direction, 1, params);
				params[0] += 45;
			}
			if (this->chargedAttack) {
				this->_transform(false);
				this->_transformKind = KIND_SINGLE_HIT;
			} else
				this->playSFX(0);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 20) {
			auto old = this->frameState;

			this->nextSequence();
			this->setPose(old.poseId);
			this->frameState.poseFrame = old.poseFrame;
		}
		if (this->frameState.sequenceId == 2 && this->frameState.currentFrame == 12)
			this->nextSequence();
		break;
	case SokuLib::ACTION_j5A:
		if (this->applyAirMechanics())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		this->speed.y -= this->gravity.y;
		break;
	case SokuLib::ACTION_j6A:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.currentFrame == 1) {
			params[0] = SokuLib::ACTION_j6A;
			params[1] = 9;
			params[2] = 0;
			this->createObject(805, this->position.x , this->position.y, this->direction, 1, params);
			params[2] = 1;
			this->createObject(805, this->position.x , this->position.y, this->direction, -1, params);
			this->playSFX(3);
		}
		this->speed.y -= this->gravity.y;
		if (this->getGroundHeight() >= this->position.y)
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		break;
	case SokuLib::ACTION_j2A:
		if (this->gravity.y <= 0)
			this->gravity.y = FALLING_GRAVITY;
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		else if (this->frameState.sequenceId == 0) {
			this->speed.y -= this->gravity.y;
			if (this->getGroundHeight() >= this->position.y && this->frameState.poseId < 5)
				this->setAction(SokuLib::ACTION_IDLE);
		} else if (this->frameState.sequenceId == 1) {
			this->speed.y -= this->gravity.y;
			if (this->getGroundHeight() >= this->position.y + this->speed.y && this->frameState.currentFrame != 0) {
				this->setSequence(2);
				this->playSFX(2);
				this->position.y = this->getGroundHeight();
				this->speed = {0, 0};
			}
		}
		if (this->frameState.sequenceId == 0) {
			if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
				this->createObject(804, this->position.x , this->position.y, this->direction, -1);
				this->playSFX(3);
			}
		}
		if (this->frameState.sequenceId == 2) {
			if (this->frameState.poseId > 2)
				this->speed.y -= this->gravity.y;
			else if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
				this->speed.x = -5;
				this->speed.y = 10;
				this->gravity.y = 0.75;
			}
		} else if (this->frameState.sequenceId == 3) {
			this->speed.y -= this->gravity.y;
			this->collisionType = COLLISION_TYPE_HIT;
		}
		break;
	case SokuLib::ACTION_j8A:
		if (this->applyAirMechanics())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 1 && this->frameState.poseFrame == 0)
			this->createObject(809, this->position.x, this->position.y, this->direction, -1);
		if (this->frameState.sequenceId == 1 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
			if (this->inputData.keyInput.a == 0 && this->frameState.currentFrame != 0) {
				this->nextSequence();
				return;
			}
			this->playSFX(6);
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = 1;

			auto old = this->speed.y;

			if (this->inputData.keyInput.verticalAxis < 0)
				this->speed.y += 13;
			else if (this->inputData.keyInput.verticalAxis == 0)
				this->speed.y += 10;
			else
				this->speed.y += 7;
			if (this->speed.y >= 20)
				this->speed.y = max(old, 20);
		}
		if (this->position.y >= 680)
			this->speed.y = 0;
		this->speed.y -= this->gravity.y;
		break;

	case SokuLib::ACTION_5B:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.b == 0)
			this->chargedAttack = false;
		if (this->frameState.sequenceId == 0) {
			if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0 && this->chargedAttack) {
				this->nextSequence();
				this->createEffect(0x3E, (float)(this->direction * 50) + this->position.x, this->position.y + 90, this->direction, 1);
			}
			if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
				float params[5] = {
					0, 10, 0,
					this->position.x + (float)(this->direction * 150),
					this->position.y + 90
				};

				this->consumeSpirit(200, 40);
				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				this->playSFX(4);
			}
			if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0) {
				float params[5] = {
					0, 10, 0,
					this->position.x + (float)(this->direction * 130),
					this->position.y + 130
				};

				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				params[4] = this->position.y + 50;
				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				this->playSFX(4);
			}
		} else if (this->frameState.sequenceId == 1) {
			if (this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
				this->setAction(SokuLib::ACTION_IDLE);
				break;
			}
			if (this->frameState.poseId == 1 && this->frameState.poseFrame == 0) {
				float params[5] = {
					0, 10, 0,
					this->position.x + (float)(this->direction * 150),
					this->position.y + 90
				};

				this->consumeSpirit(200, 40);
				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				this->playSFX(4);
			}
			if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
				float params[5] = {
					0, 10, 0,
					this->position.x + (float)(this->direction * 130),
					this->position.y + 130
				};

				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				params[4] = this->position.y + 50;
				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				params[4] = this->position.y + 90;
				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				this->playSFX(4);
			}
			if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
				float params[5] = {
					0, 10, 0,
					this->position.x + (float)(this->direction * 110),
					this->position.y + 170
				};

				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				params[4] = this->position.y + 10;
				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				params[4] = this->position.y + 50;
				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				params[4] = this->position.y + 130;
				this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
				this->playSFX(4);
			}
		}
		break;
	case SokuLib::ACTION_2B:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_CROUCHED);
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			float params[5] = {
				-90, 10, 0,
				this->position.x + (float)(this->direction * 90),
				this->position.y + 30
			};

			this->consumeSpirit(200, 40);
			this->createObject(800, this->position.x + (this->direction * 40), this->position.y + 48, this->direction, 1, params);
			this->playSFX(4);
		}
		if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0) {
			float params[5] = {
				-90, 10, 0,
				this->position.x + (float)(this->direction * 140),
				this->position.y + 30
			};

			this->createObject(800, this->position.x + (this->direction * 40), this->position.y + 48, this->direction, 1, params);
			this->playSFX(4);
		}
		if (this->frameState.poseId == 6 && this->frameState.poseFrame == 0) {
			float params[5] = {
				-90, 10, 0,
				this->position.x + (float)(this->direction * 190),
				this->position.y + 30
			};

			this->createObject(800, this->position.x + (this->direction * 40), this->position.y + 48, this->direction, 1, params);
			this->playSFX(4);
		}
		break;
	case SokuLib::ACTION_6B:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			float params[5] = {
				0, 10, 3,
				this->position.x + (float)(this->direction * 150),
				this->position.y + 90
			};

			this->collisionType = COLLISION_TYPE_HIT;
			this->consumeSpirit(200, 40);
			this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 90, this->direction, 1, params);
			this->playSFX(4);
		}
		break;

	case SokuLib::ACTION_j5B:
		if (this->advanceFrame() || this->applyAirMechanics())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId < 1 && this->inputData.keyInput.b == 0)
			this->chargedAttack = false;
		if (this->frameState.sequenceId == 0) {
			if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0 && this->chargedAttack) {
				this->nextSequence();
				this->createEffect(0x3E, (float)(this->direction * 30) + this->position.x, this->position.y + 140, this->direction, 1);
			}
			if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
				float params[5] = {
					0, 10, 0,
					this->position.x + (float)(this->direction * 150),
					this->position.y + 140
				};

				this->consumeSpirit(200, 40);
				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				this->playSFX(4);
			}
			if (this->frameState.poseId == 4 && this->frameState.poseFrame == 4) {
				float params[5] = {
					0, 10, 0,
					this->position.x + (float)(this->direction * 130),
					this->position.y + 190
				};

				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				params[4] = this->position.y + 90;
				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				this->playSFX(4);
			}
			if (this->frameState.poseId >= 7)
				this->speed.y -= this->gravity.y;
		} else if (this->frameState.sequenceId == 1) {
			if (this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
				this->setAction(SokuLib::ACTION_FALLING);
				break;
			}
			if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
				float params[5] = {
					0, 10, 0,
					this->position.x + (float)(this->direction * 150),
					this->position.y + 140
				};

				this->consumeSpirit(200, 40);
				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				this->playSFX(4);
			}
			if (this->frameState.poseId == 2 && this->frameState.poseFrame == 4) {
				float params[5] = {
					0, 10, 0,
					this->position.x + (float)(this->direction * 130),
					this->position.y + 190
				};

				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				params[4] = this->position.y + 90;
				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				params[4] = this->position.y + 140;
				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				this->playSFX(4);
			}
			if (this->frameState.poseId == 2 && this->frameState.poseFrame == 8) {
				float params[5] = {
					0, 10, 0,
					this->position.x + (float)(this->direction * 110),
					this->position.y + 260
				};

				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				params[4] = this->position.y + 30;
				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				params[4] = this->position.y + 90;
				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				params[4] = this->position.y + 190;
				this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
				this->playSFX(4);
			}
			if (this->frameState.poseId >= 5)
				this->speed.y -= this->gravity.y;
		}
		break;
	case SokuLib::ACTION_j2B:
		if (this->advanceFrame() || this->applyAirMechanics())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			float params[5] = {
				55, 10, 0,
				this->position.x + (float)(this->direction * 30),
				this->position.y + 90
			};

			this->consumeSpirit(200, 40);
			this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
			this->playSFX(4);
		}
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 4) {
			float params[5] = {
				55, 10, 0,
				this->position.x + (float)(this->direction * 90),
				this->position.y + 90
			};

			this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
			this->playSFX(4);
		}
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 8) {
			float params[5] = {
				55, 10, 0,
				this->position.x + (float)(this->direction * 150),
				this->position.y + 90
			};

			this->createObject(800, this->position.x + (this->direction * 30), this->position.y + 140, this->direction, 1, params);
			this->playSFX(4);
		}
		if (this->frameState.poseId >= 7)
			this->speed.y -= this->gravity.y;
		break;
	case SokuLib::ACTION_j6B:
		if (this->advanceFrame() || this->applyAirMechanics())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			float params[5] = {
				0, 10, 3,
				this->position.x + (float)(this->direction * 150),
				this->position.y + 190
			};

			this->consumeSpirit(200, 40);
			this->createObject(800, this->position.x + (this->direction * 50), this->position.y + 140, this->direction, 1, params);
			this->playSFX(4);
		}
		if (this->frameState.poseId >= 7)
			this->speed.y -= this->gravity.y;
		break;
	case SokuLib::ACTION_66B:
		if (this->frameState.sequenceId != 1) {
			if (this->_checkDashSlide()) {
				this->setAction(SokuLib::ACTION_IDLE);
				break;
			}
		} else if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
			break;
		}
		if (!this->gameData.frameData->frameFlags.airborne && this->applyGroundMechanics())
			break;
		if (this->frameState.sequenceId == 1) {
			this->speed.y -= this->gravity.y;
			if (this->frameState.currentFrame == 0) {
				this->speed = {15, 10};
				this->gravity.y = 1;
				this->playSFX(6);
			}
			if (this->applyAirMechanics()) {
				this->speed.x = 7.5;
				this->nextSequence();
			}
		}
		break;

	case SokuLib::ACTION_2C:
		this->applyGroundMechanics();
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_CROUCHED);
			return;
		}
		if (this->frameState.poseId == 1 && this->frameState.poseFrame == 0)
			this->createObject(807, this->position.x + (this->direction * 70), this->position.y - 160, this->direction, 1);
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			this->playSFX(5);
			this->consumeSpirit(200, 60);
			this->collisionType = COLLISION_TYPE_HIT;
		}
		break;
	case SokuLib::ACTION_6C:
		this->applyGroundMechanics();
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.poseId == 1 && this->frameState.poseFrame == 0)
			this->createObject(801, this->position.x + (this->direction * 200), this->position.y + 700, this->direction, 1);
		if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0) {
			this->playSFX(6);
			this->collisionType = COLLISION_TYPE_HIT;
			this->consumeSpirit(200, 60);
		}
		break;
	case SokuLib::ACTION_5C:
		this->applyGroundMechanics();
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_IDLE);
			return;
		}
		if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0) {
			this->playSFX(5);
			this->collisionType = COLLISION_TYPE_HIT;
			params[0] = 0;
			params[1] = 15;
			params[2] = 0.5;
			this->consumeSpirit(200, 60);
			this->createObject(808, this->position.x + (this->direction * 50), this->position.y + 100, this->direction, 1, params);
		}
		break;
	case SokuLib::ACTION_j2C:
		if (this->frameState.sequenceId >= 2 && this->applyAirMechanics())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId >= 2 && this->applyAirMechanics())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
			this->playSFX(5);
			this->collisionType = COLLISION_TYPE_HIT;
			params[0] = 45;
			params[1] = 15;
			params[2] = 0.5;
			this->consumeSpirit(200, 60);
			this->createObject(808, this->position.x + (this->direction * 50), this->position.y + 100, this->direction, 1, params);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 8)
			this->nextSequence();
		if (this->frameState.sequenceId == 2 && this->frameState.currentFrame == 12)
			this->nextSequence();
		if (this->frameState.sequenceId == 3)
			this->speed.y -= this->gravity.y;
		break;
	case SokuLib::ACTION_j6C:
		if (this->frameState.sequenceId >= 2 && this->applyAirMechanics())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 1 && this->frameState.poseFrame == 0)
			this->createObject(801, this->position.x + (this->direction * 200), this->position.y + 650, this->direction, 1);
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
			this->playSFX(6);
			this->consumeSpirit(200, 60);
			this->collisionType = COLLISION_TYPE_HIT;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 8)
			this->nextSequence();
		if (this->frameState.sequenceId == 2 && this->frameState.currentFrame == 12)
			this->nextSequence();
		if (this->frameState.sequenceId == 3)
			this->speed.y -= this->gravity.y;
		break;
	case SokuLib::ACTION_j5C:
		if (this->frameState.sequenceId >= 2 && this->applyAirMechanics())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			return;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
			this->playSFX(5);
			this->collisionType = COLLISION_TYPE_HIT;
			params[0] = 0;
			params[1] = 15;
			params[2] = 0.5;
			this->consumeSpirit(200, 60);
			this->createObject(808, this->position.x + (this->direction * 50), this->position.y + 100, this->direction, 1, params);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 8)
			this->nextSequence();
		if (this->frameState.sequenceId == 2 && this->frameState.currentFrame == 12)
			this->nextSequence();
		if (this->frameState.sequenceId == 3)
			this->speed.y -= this->gravity.y;
		break;
	case SokuLib::ACTION_66C:
		// Normally not possible, so let's just go back to idle
		if (this->isGrounded())
			this->setAction(SokuLib::ACTION_IDLE);
		else
			this->setAction(SokuLib::ACTION_FALLING);
		break;

	case ACTION_d623b:
		this->skillIndex = 0;
		if (this->frameState.sequenceId != 4)
			this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.poseId == 0 && this->frameState.currentFrame == 0)
				this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			if (this->frameState.currentFrame == 20)
				this->nextSequence();
			if (this->collisionType == COLLISION_TYPE_INVUL) {
				this->setSequence(4);
				for (int i = 0; i < 50; i++) {
					float params[9];
					float x = this->position.x + SokuLib::rand(150) - 75;
					float y = this->position.y + SokuLib::rand(200);

					params[0] = std::atan2(this->position.y + 100 - y, this->position.x - x) + M_PI;
					params[1] = SokuLib::rand(200) / 100.f + 0.5;
					params[2] = SokuLib::rand(4);
					params[3] = SokuLib::rand(10) + 10;
					params[4] = SokuLib::rand(40) - 20;
					params[5] = 1;
					params[6] = 1;
					params[7] = 0;
					params[8] = 0;
					this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
				}
				this->playSFX(0);
				this->position.x = this->gameData.opponent->position.x;
				this->position.y = this->gameData.opponent->position.y + 500;
				this->speed.y = -20;
				this->collisionType = COLLISION_TYPE_NONE;
				this->collisionLimit = 1;
				this->renderInfos.shaderType = 1;
				this->gameData.opponent->hitStop = 10;
			}
		} else if (this->frameState.sequenceId == 2) {
			if (this->frameState.currentFrame == 20 - this->effectiveSkillLevel[0] * 4)
				this->nextSequence();
		} else if (this->frameState.sequenceId == 4) {
			if (this->frameState.poseId == 0 && this->frameState.currentFrame == 0) {
				this->setAction(SokuLib::ACTION_IDLE);
				return;
			}
			this->speed.y -= 1;
			if (this->isGrounded()) {
				this->nextSequence();
				this->playSFX(1);
				this->renderInfos.shaderType = 0;
				SokuLib::camera.shake = 5;
				params[0] = 0;
				params[1] = 20;
				params[2] = 0;
				for (int i = 0; i < 8; i++) {
					params[0] = i * 45.0 + SokuLib::rand(30);
					this->createObject(810, this->position.x, this->position.y, this->direction, (params[0] < 0.0 || params[0] > 180.0) ? -1 : 1, params);
				}
			}
		}
		break;

	case ACTION_d22b:
	case ACTION_d22b_UNTRANSFORM:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame >= 10 - this->effectiveSkillLevel[1] * 2) {
			this->setAction(SokuLib::ACTION_IDLE);
			this->_transform();
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
		}
		break;

	case ACTION_FORCE_TIMER_UNTRANSFORM:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.currentFrame >= 60 - this->effectiveSkillLevel[1] * 5)
			this->nextSequence();
		break;

	case ACTION_FORCE_TIMER_UNTRANSFORM_AIR:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.currentFrame >= 60 - this->effectiveSkillLevel[1] * 5)
			this->nextSequence();
		break;

	case ACTION_d214b:
	case ACTION_d214c:
	case ACTION_jd214b:
	case ACTION_jd214c:
		this->skillIndex = 2;
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			break;
		}
		if (this->frameState.sequenceId == 0) {
			if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
				this->createObject(820, this->position.x + (this->direction * 50), this->position.y + 80, this->direction, -1);
				this->playSFX(3);
			}
		} else if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame == 0) {
				this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
				if (this->frameState.actionId % 2 == 0) {
					this->speed.x = 12.5;
					this->speed.y = 15;
				} else {
					this->speed.x = 7.5;
					this->speed.y = 20;
				}
			}
			this->speed.y -= this->gravity.y;
			if (this->getGroundHeight() >= this->position.y + this->speed.y && this->frameState.currentFrame != 0) {
				this->nextSequence();
				this->playSFX(2);
				this->position.y = this->getGroundHeight();
				this->speed = {0, 0};
			}
		} else if (this->frameState.sequenceId == 2) {
			if (this->frameState.poseId > 2)
				this->speed.y -= this->gravity.y;
			else if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
				this->speed.x = -5;
				this->speed.y = 10;
				this->gravity.y = 0.75;
			}
		} else if (this->frameState.sequenceId == 3) {
			this->speed.y -= this->gravity.y;
			this->collisionType = COLLISION_TYPE_HIT;
		} else if (this->frameState.sequenceId == 4) {
			if (this->applyAirMechanics())
				this->setAction(SokuLib::ACTION_FALLING);
		}
		break;

	case ACTION_d236b:
	case ACTION_d236c:
		this->skillIndex = 3;
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			break;
		}
		this->applyGroundMechanics();
		if (this->collisionType == COLLISION_TYPE_GRAZED) {
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = 1;
		}
		if (this->frameState.poseId == 8)
			this->collisionType = COLLISION_TYPE_HIT;
		if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
			if (this->frameState.actionId == ACTION_d236b)
				params[0] = 0;
			else
				params[0] = -35;
			params[1] = 0;
			params[2] = 0;
			this->createObject(825, this->position.x, this->position.y + 110, this->direction, -1, params);
			params[2] = 1;
			this->createObject(825, this->position.x, this->position.y + 110, this->direction, 1, params);
			this->playSFX(5);
		}
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			if (this->frameState.actionId == ACTION_d236b)
				params[0] = 70;
			else
				params[0] = 35;
			params[1] = 0;
			params[2] = 2;
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->createObject(825, this->position.x + this->direction * 30, this->position.y + 110, this->direction, 1, params);
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
		}
		break;

	case ACTION_jd236b:
	case ACTION_jd236c:
		this->skillIndex = 3;
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			break;
		}
		if (this->applyAirMechanics()) {
			this->setAction(SokuLib::ACTION_LANDING);
			break;
		}
		if (this->collisionType == COLLISION_TYPE_GRAZED) {
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = 1;
		}
		if (this->frameState.sequenceId == 0 && this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			if (this->frameState.actionId == ACTION_jd236b)
				params[0] = 50;
			else
				params[0] = -35;
			params[1] = 0;
			params[2] = 0;
			this->createObject(825, this->position.x, this->position.y + 110, this->direction, -1, params);
			params[2] = 1;
			this->createObject(825, this->position.x, this->position.y + 110, this->direction, 1, params);
			this->playSFX(5);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.poseId == 0 && this->frameState.poseFrame == 0) {
			if (this->frameState.actionId == ACTION_jd236b)
				params[0] = 120;
			else
				params[0] = 35;
			params[1] = 0;
			params[2] = 2;
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			this->createObject(825, this->position.x + this->direction * 30, this->position.y + 110, this->direction, 1, params);
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
		}
		if (this->frameState.sequenceId == 2 && this->frameState.currentFrame == 10)
			this->collisionType = COLLISION_TYPE_HIT;
		if ((this->frameState.sequenceId == 2 && this->frameState.currentFrame >= 10) || this->frameState.sequenceId == 3)
			this->speed.y -= this->gravity.y;
		if (this->frameState.sequenceId == 2 && this->frameState.currentFrame == 20)
			this->nextSequence();
		break;

	case ACTION_a1_22b:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame >= 20 - this->effectiveSkillLevel[5] * 4) {
			this->setAction(SokuLib::ACTION_IDLE);
			this->_transform();
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
		}
		break;
	case ACTION_ja1_22b:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame >= 20 - this->effectiveSkillLevel[5] * 4) {
			this->setAction(SokuLib::ACTION_FALLING);
			this->_transform();
			this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
		}
		break;

	case ACTION_a1_22b_UNTRANSFORM:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 0 && this->frameState.currentFrame >= 9 - this->effectiveSkillLevel[5] * 2) {
			auto old = this->frameState;

			this->nextSequence();
			this->setPose(old.poseId);
			this->frameState.poseFrame = old.poseFrame;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 30)
			this->nextSequence();
		break;
	case ACTION_ja1_22b_UNTRANSFORM:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_FALLING);
		if (this->frameState.sequenceId == 0 && this->frameState.currentFrame >= 9 - this->effectiveSkillLevel[5] * 2) {
			auto old = this->frameState;

			this->nextSequence();
			this->setPose(old.poseId);
			this->frameState.poseFrame = old.poseFrame;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 30)
			this->nextSequence();
		break;

	case ACTION_a2_22b:
		this->applyGroundMechanics();
	case ACTION_ja2_22b:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame < 30) {
				if (this->frameState.currentFrame % 10 == 0) {
					this->playSFX(0);
					for (int i = 0; i < 4; i++) {
						float params[9];
						float x = this->position.x + SokuLib::rand(150) - 75;
						float y = this->position.y + SokuLib::rand(200);

						params[0] = std::atan2(this->position.y + 100 - y, this->position.x - x) + M_PI;
						params[1] = SokuLib::rand(200) / 100.f + 0.5;
						params[2] = SokuLib::rand(4);
						params[3] = SokuLib::rand(10) + 10;
						params[4] = SokuLib::rand(40) - 20;
						params[5] = 1;
						params[6] = 1;
						params[7] = 0;
						params[8] = 0;
						this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
					}
				}
			} else if (this->frameState.currentFrame < 60) {
				if (this->frameState.currentFrame % 6 == 0) {
					this->playSFX(0);
					for (int i = 0; i < 6; i++) {
						float params[9];
						float x = this->position.x + SokuLib::rand(150) - 75;
						float y = this->position.y + SokuLib::rand(200);

						params[0] = std::atan2(this->position.y + 100 - y, this->position.x - x) + M_PI;
						params[1] = SokuLib::rand(200) / 100.f + 0.5;
						params[2] = SokuLib::rand(4);
						params[3] = SokuLib::rand(10) + 10;
						params[4] = SokuLib::rand(40) - 20;
						params[5] = 1;
						params[6] = 1;
						params[7] = 0;
						params[8] = 0;
						this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
					}
				}
			} else if (this->frameState.currentFrame < 75) {
				if (this->frameState.currentFrame % 3 == 0) {
					this->playSFX(0);
					for (int i = 0; i < 8; i++) {
						float params[9];
						float x = this->position.x + SokuLib::rand(150) - 75;
						float y = this->position.y + SokuLib::rand(200);

						params[0] = std::atan2(this->position.y + 100 - y, this->position.x - x) + M_PI;
						params[1] = SokuLib::rand(200) / 100.f + 0.5;
						params[2] = SokuLib::rand(4);
						params[3] = SokuLib::rand(10) + 10;
						params[4] = SokuLib::rand(40) - 20;
						params[5] = 1;
						params[6] = 1;
						params[7] = 0;
						params[8] = 0;
						this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
					}
				}
			} else if (this->frameState.currentFrame == 75) {
				this->_transformStacks--;
				this->setAction(SokuLib::ACTION_IDLE);
				this->_transform();
				this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			}
		}
		break;
	case ACTION_a2_22c:
		this->applyGroundMechanics();
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId == 1) {
			if (this->frameState.currentFrame == 0)
				this->consumeSpirit(200 / (this->skillCancelCount + 1), 120);
			if (this->frameState.currentFrame >= 20 && !this->inputData.keyInput.c) {
				this->nextSequence();
				this->collisionType = COLLISION_TYPE_HIT;
				break;
			}
			this->_transformStackCharge += this->effectiveSkillLevel[9] / 4 + 1;
			if (this->frameState.currentFrame % 4 < this->effectiveSkillLevel[9] % 4)
				this->_transformStackCharge++;
			if (this->_transformStacks >= MAX_STACKS)
				this->_transformStackCharge = 0;
			if (this->_transformStackCharge >= METER_PER_STACK) {
				SokuLib::playSEWaveBuffer(SokuLib::SFX_CARD_DRAWN);
				this->_transformStacks++;
				this->_transformStackCharge -= METER_PER_STACK;
			}
		}
		break;

	case SokuLib::ACTION_USING_SC_ID_200:
		if (this->frameState.sequenceId == 5 && this->frameState.currentFrame == 0) {
			this->speed.x = -6;
			this->speed.y = 6;
			this->gravity.y = 0.7;
		}
		if (this->advanceFrame()) {
			this->setAction(SokuLib::ACTION_FALLING);
			break;
		}
		if (this->frameState.sequenceId == 4 && this->frameState.currentFrame == 0)
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.sequenceId < 4)
			this->applyGroundMechanics();
		else
			this->speed.y -= this->gravity.y;
		if (this->frameState.sequenceId == 4) {
			this->gpShort[0]++;
			if (this->gpShort[0] < 12)
				this->renderInfos.color.a = this->gpShort[0] * 20;
			else
				this->renderInfos.color.a = 255;
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
			this->spellStopCounter = 40;
			this->playSpellBackground(4, 120);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
			this->createObject(850, this->position.x + this->direction * 300, this->position.y, this->direction, 1);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 40)
			this->nextSequence();
		if (this->frameState.sequenceId == 2 && this->frameState.currentFrame == 60)
			this->nextSequence();
		break;

	case SokuLib::ACTION_USING_SC_ID_202:
		if (this->frameState.sequenceId == 0 && this->frameState.currentFrame == 0) {
			params[0] = 0;
			this->createObject(852, this->position.x, this->position.y, this->direction, -1, params);
		}
		if (this->frameState.sequenceId == 2 && this->frameState.currentFrame == 0) {
			this->spellStopCounter = 40;
			this->playSpellBackground(2, 400);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.sequenceId == 3 && this->frameState.currentFrame == 0)
			this->playSFX(9);
		this->advanceFrame();
		if (this->frameState.sequenceId <= 3)
			SokuLib::camera.shake = 5;
		else {
			if (this->applyAirMechanics())
				this->setAction(SokuLib::ACTION_LANDING);
			this->speed.y -= this->gravity.y;
		}
		break;

	case SokuLib::ACTION_USING_SC_ID_203:
		if (this->advanceFrame())
			this->setAction(SokuLib::ACTION_IDLE);
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			this->spellStopCounter = 40;
			this->playSpellBackground(6, 80);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->consumeCard();
			this->eventSpellUse();
			this->eventWeatherCycle();
		}
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == this->frameState.poseDuration - 1)
			reinterpret_cast<MamizouGameObjectList *>(this->objectList)->clearMyList();
		if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0) {
			float params[9] = {
				0, 5,
				0, 11, 3,
				1, 1, 0, 0
			};
			auto x = this->position.x + 125 * this->direction;
			auto y = this->position.y + 100;

			this->createObject(802, x, y, 1, 1, params);
			params[0] = M_PI_4;
			this->createObject(802, x, y, 1, 1, params);
			params[0] = M_PI_4 + M_PI_2;
			params[2] = 1;
			this->createObject(802, x, y, 1, 1, params);
			params[0] = M_PI_2;
			this->createObject(802, x, y, 1, 1, params);
			params[0] = -M_PI_4;
			params[2] = 2;
			this->createObject(802, x, y, -1, 1, params);
			params[0] = -M_PI_2;
			this->createObject(802, x, y, -1, 1, params);
			params[0] = -M_PI_4 - M_PI_2;
			params[2] = 3;
			this->createObject(802, x, y, -1, 1, params);
			params[0] = M_PI;
			this->createObject(802, x, y, -1, 1, params);
			this->playSFX(0);
		}
		if (this->collisionType == COLLISION_TYPE_HIT || this->collisionType == COLLISION_TYPE_ARMORED) {
			this->_setupOpponentDebuff();
			this->collisionType = COLLISION_TYPE_NONE;
		}
		break;

	case SokuLib::ACTION_USING_SC_ID_204:
		this->applyGroundMechanics();
	case SokuLib::ACTION_SC_ID_204_ALT_EFFECT:
		this->advanceFrame();
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
			this->spellStopCounter = 60;
			this->playSpellBackground(0, 120);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SPELL_ACTIVATE);
			this->createEffect(115, this->position.x - this->direction * 34.f, this->position.y + 114.f, this->direction, 1);
			this->eventSpellUse();
			this->eventWeatherCycle();
			this->_selectedCards.clear();
			this->_currentSelection = 0;

			float params[1] = {0};

			this->createObject(994, this->position.x - 50, this->position.y + 100, 1, 2, params);
			params[0] = 1;
			this->createObject(994, this->position.x, this->position.y + 100, 1, 2, params);
			params[0] = 2;
			this->createObject(994, this->position.x + 50, this->position.y + 100, 1, 2, params);
		}
		if (this->frameState.currentFrame % 8 == 0)
			this->playSFX(0);
		for (int i = 0; i < 2; i++) {
			float params[9];
			float x = this->position.x + SokuLib::rand(150) - 75;
			float y = this->position.y + SokuLib::rand(200);

			params[0] = std::atan2(this->position.y + 100 - y, this->position.x - x) + M_PI;
			params[1] = SokuLib::rand(200) / 100.f + 0.5;
			params[2] = SokuLib::rand(4);
			params[3] = SokuLib::rand(10) + 10;
			params[4] = SokuLib::rand(40) - 20;
			params[5] = 1;
			params[6] = 1;
			params[7] = 0.1;
			params[8] = 0.1;
			this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
		}
		if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 60)
			this->nextSequence();
		if (this->frameState.sequenceId >= 1 && this->_currentSelection >= this->_opponentSkillCards.size()) {
			this->_transform();
			this->_transformKind = KIND_SPELL_TIMER;
			this->_transformTimer = SPELL_TIMER_MAX;
			this->_currentCard = SokuLib::rand(this->_opponentSpellCards.size());
			if (this->_currentCard >= this->_opponentSpellCards.size())
				this->_currentCard = this->_opponentSpellCards.size() - 1;
			this->_fillHand();
			memset(&this->inputData.keyInput, 0, sizeof(this->inputData.keyInput));
			memset(&this->inputData.bufferedKeyInput, 0, sizeof(this->inputData.bufferedKeyInput));
		} else if (this->frameState.sequenceId >= 1) {
			auto &arr = this->_opponentSkillCards[this->_currentSelection];

			if (arr.second[0] && (this->inputData.keyInput.a == 1 || this->inputData.keyInput.horizontalAxis == -1)) {
				this->_selectedCards.push_back(0);
				this->_currentSelection++;
				this->createEffect(134, this->position.x, this->position.y, 1, 1);
				this->createEffect(71, this->position.x, this->position.y + 100, 1, 1);
			} else if (arr.second[1] && (this->inputData.keyInput.b == 1 || this->inputData.keyInput.verticalAxis == -1 || this->inputData.keyInput.verticalAxis == 1)) {
				this->_selectedCards.push_back(1);
				this->_currentSelection++;
				this->createEffect(134, this->position.x, this->position.y, 1, 1);
				this->createEffect(71, this->position.x, this->position.y + 100, 1, 1);
			} else if (arr.second[2] && (this->inputData.keyInput.c == 1 || this->inputData.keyInput.horizontalAxis == 1)) {
				this->_selectedCards.push_back(2);
				this->_currentSelection++;
				this->createEffect(134, this->position.x, this->position.y, 1, 1);
				this->createEffect(71, this->position.x, this->position.y + 100, 1, 1);
			}
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
	if (this->_transformed && action >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN && action < SokuLib::ACTION_FORWARD_AIRTECH) {
		// TODO: For blocking, though something is still wrong here.
		//       We get pushed normally on block, but for some reason,
		//       the opponent doesn't when we are in the corner.
		//       Normally, the opponent takes the push back we would have
		//       took if we are in the corner, but that doesn't happen here.
		if (
			this->_transformKind == KIND_SPELL_TIMER ||
			action >= SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN || (
				this->_transformKind == KIND_STACK && (this->_transformStacks || (
					this->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
					this->frameState.actionId < SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN
				))
			)
		) {
			if (action < SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN && (
				this->frameState.actionId < SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN ||
				this->frameState.actionId >= SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN
			) && this->_transformKind == KIND_STACK)
				this->_transformStacks--;
			this->_transformPlayer->unknown1A4 = this->unknown1A4;
			this->_transformPlayer->unknown1A8 = this->unknown1A8;

			bool b = this->_transformPlayer->setAction(action);

			return SokuLib::v2::Player::setAction(action) && b;
		}
		this->_transformPlayer->setAction(action);
		this->_unTransform();
	}
	if (this->_transformed && this->_forward) {
		if (action < SokuLib::ACTION_5A || this->_stayTransformed) {
			this->_transformPlayer->setAction(action);
			return true;
		}
		this->_unTransform();
	}
	if (!this->_transformed && action == SokuLib::ACTION_66C) {
		if (this->_transformPlayer == nullptr)
			return Player::setAction(SokuLib::ACTION_66B);
		this->_transform();
		this->_preTransformCall();
		this->_transformKind = KIND_FULL_MOVE;
		if (this->_transformPlayer->characterIndex == SokuLib::CHARACTER_REMILIA) {
			action = SokuLib::ACTION_3A;
			this->_transformPlayer->setAction(SokuLib::ACTION_3A);
			this->_transformPlayer->grazeTimer = 20;
			this->grazeTimer = 20;
		} else if (this->_transformPlayer->characterIndex == SokuLib::CHARACTER_FLANDRE) {
			this->_transformPlayer->setAction(SokuLib::ACTION_ALT2_SKILL1_B);
			action = SokuLib::ACTION_ALT2_SKILL1_B;
		} else if (this->_transformPlayer->characterIndex == SokuLib::CHARACTER_MAMIZOU) {
			this->_transformPlayer->setAction(SokuLib::ACTION_66B);
			action = SokuLib::ACTION_66B;
		} else {
			this->_transformPlayer->setAction(SokuLib::ACTION_FORWARD_DASH);
			this->_transformPlayer->setAction(SokuLib::ACTION_66C);
		}
		this->frameState.actionId = action;
		this->_postTransformCall();
		return true;
	}
	if (action == SokuLib::ACTION_USING_SC_ID_201)
		action = SokuLib::ACTION_USING_SC_ID_200;
	return Player::setAction(action);
}

void Mamizou::initializeAction()
{
	float params[3];

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
		this->speed.x = FRONT_WALK_SPEED;
		break;
	case SokuLib::ACTION_WALK_BACKWARD:
		this->speed.x = BACK_WALK_SPEED;
		break;
	case ACTION_FORCE_TIMER_UNTRANSFORM_AIR:
		this->speed = {0, 0};
		this->gravity.y = JUMP_GRAVITY;
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
		this->gpFloat[4] = 0;
		break;
	case SokuLib::ACTION_j8A:
		this->gravity.y = 0.4;
	case SokuLib::ACTION_j6A:
	case SokuLib::ACTION_j2A:
	case SokuLib::ACTION_j5A:
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_j5B:
		this->chargedAttack = true;
	case SokuLib::ACTION_j5C:
	case SokuLib::ACTION_j2C:
	case SokuLib::ACTION_j6C:
	case SokuLib::ACTION_j2B:
	case SokuLib::ACTION_j6B:
		this->collisionType = COLLISION_TYPE_HIT;
		this->collisionLimit = 1;
		if (this->speed.x > 3)
			this->speed.x = 3;
		if (this->speed.x < -3)
			this->speed.x = -3;
		if (this->speed.y > 3)
			this->speed.y = 3;
		if (this->speed.y < -3)
			this->speed.y = -3;
		this->gravity.y = FALLING_GRAVITY;
		break;
	case SokuLib::ACTION_5B:
	case SokuLib::ACTION_5AAAAA:
		this->chargedAttack = true;
	case SokuLib::ACTION_6C:
	case SokuLib::ACTION_5C:
	case SokuLib::ACTION_2C:
	case SokuLib::ACTION_2B:
	case SokuLib::ACTION_6B:
		this->collisionType = COLLISION_TYPE_HIT;
		this->speed = {0, 0};
		break;
	case SokuLib::ACTION_6A:
		this->chargedAttack = true;
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		this->speed = {0, 0};
		break;
	case SokuLib::ACTION_5AAAA:
		this->chargedAttack = false;
		this->speed = {0, 0};
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_3A:
		this->chargedAttack = true;
		this->speed = {0, 0};
	case SokuLib::ACTION_5A:
	case SokuLib::ACTION_5AA:
	case SokuLib::ACTION_4A:
	case SokuLib::ACTION_2A:
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		break;
	case SokuLib::ACTION_66B:
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		this->speed.x = 10;
		break;
	case SokuLib::ACTION_66A:
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		this->speed = {15, 0};
		break;
	case SokuLib::ACTION_f5A:
	case SokuLib::ACTION_5AAA:
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		this->speed = {12, 0};
		break;
	case ACTION_d623b:
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		this->speed = {0, 0};
		break;
	case ACTION_d22b:
		this->_transformKind = KIND_TIMER;
		this->speed = {0, 0};
		break;
	case ACTION_d236b:
	case ACTION_d236c:
	case ACTION_jd236b:
	case ACTION_jd236c:
		this->gravity.y = 0.6;
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		this->speed = {0, 0};
		break;
	case ACTION_d214b:
	case ACTION_d214c:
	case ACTION_jd214b:
	case ACTION_jd214c:
		this->skillIndex = 3;
		this->gravity.y = 1;
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		this->speed = {0, 0};
		break;
	case ACTION_a1_22b:
	case ACTION_ja1_22b:
		this->_transformKind = KIND_SINGLE_HIT;
	case ACTION_a1_22b_UNTRANSFORM:
	case ACTION_ja1_22b_UNTRANSFORM:
	case ACTION_a2_22c:
		this->speed = {0, 0};
		break;
	case ACTION_a2_22b:
	case ACTION_ja2_22b:
		this->_transformKind = KIND_STACK;
		this->speed = {0, 0};
		break;
	case SokuLib::ACTION_USING_SC_ID_200:
	case SokuLib::ACTION_USING_SC_ID_202:
	case SokuLib::ACTION_USING_SC_ID_203:
	case SokuLib::ACTION_USING_SC_ID_204:
	case SokuLib::ACTION_SC_ID_204_ALT_EFFECT:
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		this->speed = {0, 0};
		this->gravity.y = FALLING_GRAVITY;
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

	if (
		(hKeys == 0 || hBuffKeys == 0) &&
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
		(this->inputData.keyInput.verticalAxis == 0 || this->inputData.bufferedKeyInput.verticalAxis == 0) &&
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

	if (
		(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j2B)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_j2B);
		return true;
	}

	if (
		(hKeys > 0 || hBuffKeys > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_j6B)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_j6B);
		return true;
	}

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

	if (!this->_transformPlayer)
		return false;

	if (this->inputData.commandCombination._214b && this->_useSkill(true, 2, ACTION_jd214b))
		return true;
	if (this->inputData.commandCombination._214c && this->_useSkill(true, 2, ACTION_jd214c))
		return true;

	if (this->inputData.commandCombination._236b && this->_useSkill(true, 2, ACTION_jd236b))
		return true;
	if (this->inputData.commandCombination._236c && this->_useSkill(true, 2, ACTION_jd236c))
		return true;

	bool used22 = this->inputData.commandCombination._22b || this->inputData.commandCombination._22c;

	if (!this->_transformed) {
		if (used22 && this->_useSkill(true, 5, ACTION_ja1_22b))
			return true;
		if (this->_transformStacks && used22 && this->_useSkill(true, 9, ACTION_ja2_22b))
			return true;
	}
	return false;
}

void Mamizou::_processInputsAirborne()
{
	if (
		this->_transformKind != KIND_DEBUFF_TIMER &&
		!this->handInfo.hand.empty() &&
		((this->inputData.keyInput.spellcard != 0 && this->inputData.keyInput.spellcard < 3) || this->inputData.bufferedKeyInput.spellcard != 0) &&
		this->confusionDebuffTimer == 0 &&
		this->unknown836 == 0 &&
		this->canActivateCard(0)
	) {
		auto &card = this->handInfo.hand[0];

		if (100 <= card.id && card.id < 200 && this->_useSkillCard(card.id))
			return;
		if (200 <= card.id && card.id < 300 && this->_useSpellCard(card.id))
			return;
		if (300 <= card.id && card.id < 400 && this->_useTransformedCard(card.id - 100))
			return;
	}

	if (!this->confusionDebuffTimer && this->_processSkillsAirborne())
		return;
	if (
		this->_transformKind != KIND_DEBUFF_TIMER &&
		((this->inputData.keyUpC != 0 && this->inputData.keyUpC <= 2) || this->inputData.keyInput.c == 2 || this->inputData.bufferedKeyInput.c != 0) &&
		this->_processCAirborne()
	)
		return;
	if (
		this->_transformKind != KIND_DEBUFF_TIMER &&
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

	/*if (
		std::abs(this->position.x - this->gameData.opponent->position.x) >= FAR_RANGE &&
		!(hKeys < 0 || hBuffKeys < 0) &&
		(this->inputData.keyInput.verticalAxis > 0 || this->inputData.bufferedKeyInput.verticalAxis > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_f2A)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_f2A);
		return true;
	} else */
	if (
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
		(this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_4A) /*|| this->frameState.actionId == SokuLib::ACTION_4A*/)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_4A);
		return true;
	}

	short action = std::abs(this->position.x - this->gameData.opponent->position.x) >= FAR_RANGE ? SokuLib::ACTION_f5A : SokuLib::ACTION_5A;
	bool ok = false;

	switch (this->frameState.actionId) {
	case SokuLib::ACTION_5A:
		if (this->_transformKind != KIND_DEBUFF_TIMER) {
			action = SokuLib::ACTION_5AAA;
			ok = true;
		}
		break;
	/*case SokuLib::ACTION_5AA:
		action = SokuLib::ACTION_5AAA;
		ok = true;
		break;*/
	case SokuLib::ACTION_5AAA:
		action = SokuLib::ACTION_5AAAA;
		ok = true;
		break;
	case SokuLib::ACTION_5AAAA:
		action = SokuLib::ACTION_5AAAAA;
		ok = true;
		break;
	case SokuLib::ACTION_5AAAAA:
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

	if (
		(hKeys > 0 || hBuffKeys > 0) &&
		this->gameData.sequenceData->actionLock <= this->getMoveLock(SokuLib::ACTION_6B)
	) {
		this->renderInfos.zRotation = 0.0;
		this->setAction(SokuLib::ACTION_6B);
		return true;
	}

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
	if (this->inputData.commandCombination._623b && this->_useSkill(true, 0, ACTION_d623b))
		return true;
	if (this->inputData.commandCombination._623c && this->_useSkill(true, 0, ACTION_d623b))
		return true;
	if (this->inputData.commandCombination._214b && this->_useSkill(true, 2, ACTION_d214b))
		return true;
	if (this->inputData.commandCombination._214c && this->_useSkill(true, 2, ACTION_d214c))
		return true;
	if (this->inputData.commandCombination._236b && this->_useSkill(true, 2, ACTION_d236b))
		return true;
	if (this->inputData.commandCombination._236c && this->_useSkill(true, 2, ACTION_d236c))
		return true;

	if (!this->_transformPlayer)
		return false;

	bool used22 = this->inputData.commandCombination._22b || this->inputData.commandCombination._22c;

	if (this->_transformed) {
		if (used22 && this->_useSkill(true, 1, ACTION_d22b_UNTRANSFORM))
			return true;
	} else {
		if (!this->_transformedCooldown && used22 && this->_useSkill(true, 1, ACTION_d22b))
			return true;
		if (used22 && this->_useSkill(true, 5, ACTION_a1_22b))
			return true;
		if (this->_transformStacks && this->inputData.commandCombination._22b && this->_useSkill(true, 9, ACTION_a2_22b))
			return true;
		if (this->_transformStacks < MAX_STACKS && this->inputData.commandCombination._22c && this->_useSkill(true, 9, ACTION_a2_22c))
			return true;
	}
	return false;
}

bool Mamizou::_canUseCard(int id)
{
	if (id < 100)
		return true;

	switch (id) {
	case 100:
	case 101:
	case 200:
	case 202:
	case 203:
		return this->isGrounded();
	case 102:
	case 103:
	case 105:
	case 109:
	case 204:
		return true;
	default:
		return false;
	}
}

bool Mamizou::_useSkillCard(int id)
{
	if (this->gameData.sequenceData->actionLock > 50)
		return false;
	if (
		this->frameState.actionId >= SokuLib::ACTION_5A &&
		(this->collisionType == COLLISION_TYPE_NONE || this->collisionType == COLLISION_TYPE_INVUL)
	)
		return false;

	int index = id - 100;
	int slot = index % 4;
	int action = SokuLib::ACTION_DEFAULT_SKILL1_B + slot * 20 + (index / 4) * 5;

	if (this->inputData.keyInput.verticalAxis || this->inputData.keyInput.horizontalAxis) {
		if (!this->isGrounded())
			return false;
		this->_onSkillUpgrade(index, this->skilledSkillLevel[index]);
		this->setAction(SokuLib::ACTION_SKILL_CARD);
		return true;
	}
	if (!this->_canUseCard(id))
		return false;

	if (!this->isGrounded())
		action += 2;
	if (this->gameData.sequenceData->actionLock > this->getMoveLock(action))
		return false;
	if (!this->skillCancelsUsed[slot]) {
		this->skillCancelCount++;
		this->skillCancelsUsed[slot] = true;
	}
	this->_onSkillUpgrade(index, this->skilledSkillLevel[index]);
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
	if (!this->_canUseCard(id))
		return false;

	id += 400;
	if (!this->isGrounded())
		id += 50;
	this->renderInfos.shaderType = 0;
	this->useSpellCard(id, this->gameData.sequenceData->moveLock);
	return true;
}

bool Mamizou::_useTransformedCard(int id)
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

	auto old = this->_transformPlayer->frameState.actionId;

	this->_preTransformCall();
	memset(&this->_transformPlayer->inputData.keyInput, 0, sizeof(this->_transformPlayer->inputData.keyInput));
	memset(&this->_transformPlayer->inputData.bufferedKeyInput, 0, sizeof(this->_transformPlayer->inputData.bufferedKeyInput));
	this->_transformPlayer->inputData.keyInput.spellcard = 1;
	this->_transformPlayer->inputData.keyUpA = 0;
	this->_transformPlayer->inputData.keyUpB = 0;
	this->_transformPlayer->inputData.keyUpC = 0;
	this->_transformPlayer->inputData.keyUpD = 0;
	this->_transformPlayer->inputData.keyUpE = 0;
	this->_transformPlayer->inputData.keyUpF = 0;
	this->_transformPlayer->inputData.bufferTimer = 0;
	this->_transformPlayer->inputData.unknown7AD[0] = 0;
	this->_transformPlayer->inputData.unknown7AD[1] = 0;
	this->_transformPlayer->inputData.unknown7AD[2] = 0;
	this->_transformPlayer->inputData.commandInputBuffer.clear();
	this->_transformPlayer->inputData.movementCombination.value = 0;
	this->_transformPlayer->inputData.commandCombination.value = 0;
	this->_transformPlayer->handInfo.hand[0].id = id;
	this->_transformPlayer->handInfo.hand[0].cost = this->handInfo.hand[0].cost;

	this->_transformPlayer->handleInputs();
	if (this->_transformPlayer->frameState.actionId == old)
		return false;
	if (!this->_transformed)
		this->_transform(true, false);
	this->_transformKind = KIND_FULL_MOVE;
	this->frameState.actionId = this->_transformPlayer->frameState.actionId;
	this->_postTransformCall();
	this->_transformTimer = TIMER_MAX;
	this->_transformTimerDelay = 0;
	return true;
}

void Mamizou::_processInputsGrounded()
{
	if (
		this->_transformKind != KIND_DEBUFF_TIMER &&
		!this->handInfo.hand.empty() &&
		((this->inputData.keyInput.spellcard != 0 && this->inputData.keyInput.spellcard < 3) || this->inputData.bufferedKeyInput.spellcard != 0) &&
		this->confusionDebuffTimer == 0 &&
		this->unknown836 == 0 &&
		this->canActivateCard(0)
	) {
		auto &card = this->handInfo.hand[0];

		if (0   <= card.id && card.id < 100 && this->useSystemCard(this->gameData.sequenceData->actionLock))
			return;
		if (100 <= card.id && card.id < 200 && this->_useSkillCard(card.id))
			return;
		if (200 <= card.id && card.id < 300 && this->_useSpellCard(card.id))
			return;
		if (300 <= card.id && card.id < 400 && this->_useTransformedCard(card.id - 100))
			return;
	}
	if (!this->confusionDebuffTimer && this->_processSkillsGrounded())
		return;

	if (
		this->_transformKind != KIND_DEBUFF_TIMER &&
		((this->inputData.keyUpC != 0 && this->inputData.keyUpC <= 2) || this->inputData.keyInput.c == 2 || this->inputData.bufferedKeyInput.c != 0) &&
		this->_processCGrounded()
	)
		return;
	if (
		this->_transformKind != KIND_DEBUFF_TIMER &&
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

bool Mamizou::_tryDoUntransformedMove(bool cardsOnly, bool spellsOnly)
{
	auto frameFlags = this->_transformPlayer->gameData.frameData->frameFlags;

	this->_stayTransformed = false;
	if (this->_transformPlayer->stopwatchTimeLeft)
		goto failed;
	if (!frameFlags.cancellable)
		goto failed;
	if (this->_transformPlayer->confusionDebuffTimer)
		goto failed;
	if (
		this->_transformPlayer->isGrounded() &&
		!this->handInfo.hand.empty() &&
		(
			(this->inputData.keyInput.spellcard != 0 && this->inputData.keyInput.spellcard < 3) ||
			this->inputData.bufferedKeyInput.spellcard != 0
		) &&
		this->unknown836 == 0 && this->_transformPlayer->unknown836 == 0 &&
		this->canActivateCard(0)
	) {
		auto &card = this->handInfo.hand[0];

		if (!spellsOnly && 0   <= card.id && card.id < 100 && this->useSystemCard(this->gameData.sequenceData->actionLock))
			goto succeeded;
		if (!spellsOnly && 100 <= card.id && card.id < 200 && this->_useSkillCard(card.id))
			goto succeeded;
		if (200 <= card.id && card.id < 300 && this->_useSpellCard(card.id))
			goto succeeded;
		if (300 <= card.id && card.id < 400 && this->isGrounded() && this->_useTransformedCard(card.id - 100))
			goto succeeded;
	}
	if (spellsOnly || cardsOnly)
		goto failed;

	bool result;

	if (this->isOnGround())
		result = this->_processSkillsGrounded();
	else
		result = this->_processSkillsAirborne();

	this->_stayTransformed = true;
	return result;
succeeded:
	this->_stayTransformed = true;
	return true;
failed:
	this->_stayTransformed = true;
	return false;
}

void Mamizou::handleInputs()
{
	auto frameFlags = this->gameData.frameData->frameFlags;

	if (this->_transformKind == KIND_DEBUFF_TIMER && this->_transformTimer == 0)
		return;
	if (this->_transformKind == KIND_DEBUFF_TIMER)
		this->inputData.commandCombination.value = 0;
	if (this->_transformed) {
		if (this->_transformKind == KIND_SPELL_TIMER) {
			if (this->inputData.keyInput.changeCard == 1 && this->frameState.actionId < 600) {
				this->inputData.keyInput.changeCard = 4;
				this->_currentCard = (this->_currentCard + 1) % this->_allOpponentSpellCards.size();
				this->_fillHand();
			}
		} else if (this->handleCardSwitch())
			return;
	} else if (this->handleCardSwitch())
		return;

	if (this->_transformed) {
		if (this->_transformKind == KIND_SPELL_TIMER) {
			if (this->_tryDoUntransformedMove(false, true))
				return;
		}
		if (this->_transformKind == KIND_TIMER) {
			if (this->_tryDoUntransformedMove(false, false))
				return;
			this->inputData.commandCombination.value = 0;
		}
		if (
			this->_transformKind == KIND_STACK &&
			frameFlags.cancellable && (
				this->frameState.actionId < SokuLib::ACTION_5A ||
				(this->collisionType != COLLISION_TYPE_INVUL && this->collisionType != COLLISION_TYPE_NONE)
			) &&
			this->inputData.commandCombination._22d
		) {
			this->_unTransform();
			this->setAction(this->isOnGround() ? SokuLib::ACTION_IDLE : SokuLib::ACTION_FALLING);
			return;
		}
		if (this->_transformKind == KIND_STACK && this->_tryDoUntransformedMove(true, false))
			return;
		if (this->_transformKind == KIND_FULL_MOVE) {
			if (this->_tryDoUntransformedMove(false, false))
				return;
			this->inputData.commandCombination.value = 0;
		}
		if (
			this->_transformKind == KIND_STACK &&
			frameFlags.cancellable && (
				this->frameState.actionId < SokuLib::ACTION_5A ||
				(this->collisionType != COLLISION_TYPE_INVUL && this->collisionType != COLLISION_TYPE_NONE)
			) &&
			this->inputData.commandCombination._22d
		) {
			this->_unTransform();
			this->setAction(this->isOnGround() ? SokuLib::ACTION_IDLE : SokuLib::ACTION_FALLING);
			return;
		}
		if (
			this->_transformKind == KIND_STACK &&
			this->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
			this->frameState.actionId < SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN &&
			this->inputData.commandCombination._22d
		) {
			this->_unTransform();
			this->setSequence(this->_transformPlayer->frameState.sequenceId);
			for (unsigned i = 0; i < this->_transformPlayer->frameState.currentFrame; i++)
				this->advanceFrame();
			this->renderInfos = this->_transformPlayer->renderInfos;
			this->center = this->spinRotationCenter.to<float>();
			return;
		}
	}
	if (this->_transformed) {
		unsigned oldAction = this->_transformPlayer->frameState.actionId;

		this->_preTransformCall();
		this->_transformPlayer->handleInputs();
		this->_postTransformCall();
		if (this->_transformKind == KIND_SINGLE_HIT && oldAction > this->frameState.actionId && oldAction >= SokuLib::ACTION_5A) {
			this->_unTransform();
			this->setAction(this->isOnGround() ? ACTION_a1_22b_UNTRANSFORM : ACTION_ja1_22b_UNTRANSFORM);
		}
		return;
	}
	if (frameFlags.cancellable || frameFlags.highJumpCancellable) {
		auto uVar10 = this->inputData.commandCombination.value;

		// For story mode
		if (699 < uVar10 && uVar10 < 800) {
			this->setAction(uVar10);
			return this->FUN_0046d950();
		}

		if (this->unknown4C4 == 0 && this->isOnGround()) {
			if (this->collisionType != COLLISION_TYPE_NONE && frameFlags.highJumpCancellable && this->gameData.sequenceData->actionLock > 9 && this->handleHJ())
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
//	SokuLib::DxVertex vertices[1280];

//	for (int i = 0; i < 1280; i++) {
//		vertices[i].color = 0xFFFFFFFF;
//		vertices[i].x = (SokuLib::camera.translate.x + i) * SokuLib::camera.scale;
//		vertices[i].y = (SokuLib::camera.translate.y - SokuLib::v2::groundHeight[i]) * SokuLib::camera.scale;
//		vertices[i].z = 0;
//		vertices[i].rhw = 1;
//		vertices[i].u = 0;
//		vertices[i].v = 0;
//	}
//	SokuLib::textureMgr.setTexture(0, 0);

//	HRESULT r = SokuLib::pd3dDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 1279, vertices, sizeof(*vertices));

//	if (r != D3D_OK)
//		printf("Error: %d\n", r);

	if (this->_transformed)
		this->_transformPlayer->render();
	else
		SokuLib::v2::Player::render();
	if (this->teamId == 0 && __practiceEffect != -1) {
		SokuLib::DrawUtils::Sprite sprite;

		sprite.texture.setHandle(this->_opponentSpellCards[__practiceEffect].second, {41, 65});
		sprite.rect.width = 41;
		sprite.rect.height = 65;
		sprite.setSize({27, 43});
		sprite.setPosition({10, 125});
		sprite.setRotation(-M_PI_2 / 3);
		sprite.draw();
		sprite.texture.releaseHandle();
	}
}

void Mamizou::updatePhysics()
{
	if (this->_transformed) {
		this->_preTransformCall();
		this->_transformPlayer->updatePhysics();
		this->_postTransformCall();
	}
	SokuLib::v2::Player::updatePhysics();
	if (this->_transformed && this->_transformKind == KIND_SPELL_TIMER) {
		this->speedPower *= SPD_5SC_BOOST;
		if (this->_transformPlayer->frameState.actionId >= SokuLib::ACTION_USING_SC_ID_200 && this->_transformPlayer->frameState.actionId < SokuLib::ACTION_SKILL_CARD) {
			switch (this->_spellCost) {
			case 1:
				this->attackPower *= ATK_1COST_BOOST;
				break;
			case 2:
				this->attackPower *= ATK_2COST_BOOST;
				break;
			case 3:
				this->attackPower *= ATK_3COST_BOOST;
				break;
			case 4:
				this->attackPower *= ATK_4COST_BOOST;
				break;
			case 5:
				this->attackPower *= ATK_5COST_BOOST;
				break;
			}
		} else
			this->attackPower *= ATK_5SC_BOOST;
	}
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
	if (this->gpShort[0] == 200 && this->_transformed)
		this->_unTransform();
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

void Mamizou::_setupOpponentDebuff()
{
	this->_currentAnimal = SokuLib::rand(NB_ANIMALS);
	if (this->_currentAnimal == NB_ANIMALS) this->_currentAnimal = NB_ANIMALS - 1;
	this->gameData.opponent->setAction(2000 + this->_currentAnimal * 2 + 1);
	for (auto &[id, seq] : this->_spellMelees) {
		auto current = seq;

		while (true) {
			for (auto &frame: current->frames) {
				if (frame.damage) {
					frame.onAirHitSet = 2000 + this->_currentAnimal * 2 + 1;
					frame.onGroundHitSet = 2000 + this->_currentAnimal * 2 + 1;
				}
			}
			if (current->next == seq)
				break;
			current = current->next;
		}
		(*this->gameData.patternMap)[id] = seq;
	}
	for (int i = 0; i < 50; i++) {
		float params[9];
		float x = this->gameData.opponent->position.x + SokuLib::rand(150) - 75;
		float y = this->gameData.opponent->position.y + SokuLib::rand(200);

		params[0] = std::atan2(this->gameData.opponent->position.y + 100 - y, this->gameData.opponent->position.x - x) + M_PI;
		params[1] = SokuLib::rand(200) / 100.f + 0.5;
		params[2] = SokuLib::rand(4);
		params[3] = SokuLib::rand(10) + 10;
		params[4] = SokuLib::rand(40) - 20;
		params[5] = 1;
		params[6] = 1;
		params[7] = 0;
		params[8] = 0;
		this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
	}
	this->_transformKind = KIND_DEBUFF_TIMER;
	this->_transformTimer = DEBUFF_TIMER_MAX;
}

void Mamizou::_cleanupOpponentDebuff()
{
	if (this->skilledSkillLevel[1] >= 0)
		this->_transformKind = KIND_TIMER;
	if (this->skilledSkillLevel[5] >= 0)
		this->_transformKind = KIND_SINGLE_HIT;
	if (this->skilledSkillLevel[9] >= 0)
		this->_transformKind = KIND_STACK;
	for (auto &[id, seq] : this->_originalMelees)
		(*this->gameData.patternMap)[id] = seq;
	this->_transformTimer = TIMER_MAX;
	if (this->gameData.opponent->isOnGround())
		this->gameData.opponent->setAction(SokuLib::ACTION_IDLE);
	else
		this->gameData.opponent->setAction(SokuLib::ACTION_FALLING);
	for (int i = 0; i < 50; i++) {
		float params[9];
		float x = this->gameData.opponent->position.x + SokuLib::rand(150) - 75;
		float y = this->gameData.opponent->position.y + SokuLib::rand(200);

		params[0] = std::atan2(this->gameData.opponent->position.y + 100 - y, this->gameData.opponent->position.x - x) + M_PI;
		params[1] = SokuLib::rand(200) / 100.f + 0.5;
		params[2] = SokuLib::rand(4);
		params[3] = SokuLib::rand(10) + 10;
		params[4] = SokuLib::rand(40) - 20;
		params[5] = 1;
		params[6] = 1;
		params[7] = 0;
		params[8] = 0;
		this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
	}
	this->playSFX(0);
}

static bool pasteOnTop(LPDIRECT3DTEXTURE9 *output, LPDIRECT3DTEXTURE9 *input)
{
	D3DLOCKED_RECT resultRect;
	D3DLOCKED_RECT originalRect;
	SokuLib::Color *resultPixels;
	SokuLib::Color *originalPixels;
	HRESULT hret;

	hret = (*input)->LockRect(0, &originalRect, nullptr, 0);
	if (FAILED(hret)) {
		fprintf(stderr, "(*input)->LockRect(0, &r, nullptr, D3DLOCK_DISCARD) failed with code %li\n", hret);
		return false;
	}
	hret = (*output)->LockRect(0, &resultRect, nullptr, 0);
	if (FAILED(hret)) {
		(*input)->UnlockRect(0);
		fprintf(stderr, "(*output)->LockRect(0, &r, nullptr, D3DLOCK_DISCARD) failed with code %li\n", hret);
		return false;
	}

	resultPixels   = (SokuLib::Color *)resultRect.pBits;
	originalPixels = (SokuLib::Color *)originalRect.pBits;

	for (int y = 0; y < 65; y++)
		for (int x = 0; x < 41; x++)
			if (originalPixels[y * (originalRect.Pitch / sizeof(*originalPixels)) + x].a == 255)
				resultPixels[y * (resultRect.Pitch / sizeof(*resultPixels)) + x] = originalPixels[y * (originalRect.Pitch / sizeof(*originalPixels)) + x];

	hret = (*output)->UnlockRect(0);
	if (FAILED(hret))
		fprintf(stderr, "(*final)->UnlockRect(0) failed with code %li\n", hret);
	hret = (*input)->UnlockRect(0);
	if (FAILED(hret))
		fprintf(stderr, "(*texture)->UnlockRect(0) failed with code %li\n", hret);
	return true;
}

void Mamizou::initialize()
{
	Player::initialize();
	if (!this->_transformPlayer)
		return;
	this->_transformPlayer->initialize();
	this->_transformPlayer->setAction(SokuLib::ACTION_IDLE);
	this->_addRainbowGui();
	this->_addDebuffGui();
	this->_addTimerGui();
	this->_addStackGui();
	// TODO: Check if the opponent has extra GUI (like Sanae)
	//       In that case, we should move ours; or theirs out of the way and also probably display it all the time?

	char buffer[1024];
	sprintf(buffer, "data/csv/%s/spellcard.csv", SokuLib::getCharName(this->_transformPlayer->characterIndex));
	SokuLib::CSVParser parser{buffer};
	SokuLib::DrawUtils::Texture card2CostTexture;
	SokuLib::DrawUtils::Texture card5CostTexture;

	card2CostTexture.loadFromGame("data/card/mamizou/card_2cost.bmp");
	card5CostTexture.loadFromGame("data/card/mamizou/card_5cost.bmp");

	auto texture = card2CostTexture.getDXTexture();
	std::map<unsigned, int> map;

	do {
		auto id = parser.getNextValue();
		SokuLib::String name;

		parser.getNextCell(name);

		auto type = parser.getNextValue();
		auto cost = parser.getNextValue();
		int ret = 0;

		sprintf(buffer, "data/card/%s/card%li.bmp", SokuLib::getCharName(this->_transformPlayer->characterIndex), id);
		if (type == 2) {
			if (cost <= 3) {
				SokuLib::CardInfo &cinfo = this->deckInfo.cardById[id + 100];

				// TODO: Spawn an effect on top of the card
				if (!SokuLib::textureMgr.loadTexture(&ret, buffer, nullptr, nullptr))
					ret = 0;
				else if (!pasteOnTop(SokuLib::textureMgr.Get(ret), card2CostTexture.getDXTexture())) {
					SokuLib::textureMgr.deallocate(ret);
					ret = 0;
				}
				cinfo.name = name;
				cinfo.type = 2;
				cinfo.costOrSlot = 2;
				this->_opponentSpellCards.emplace_back(static_cast<unsigned char>(id), ret);
			}

			SokuLib::CardInfo &cinfo = this->deckInfo.cardById[id + 200];

			if (!SokuLib::textureMgr.loadTexture(&ret, buffer, nullptr, nullptr))
				ret = 0;
			else if (!pasteOnTop(SokuLib::textureMgr.Get(ret), card5CostTexture.getDXTexture())) {
				SokuLib::textureMgr.deallocate(ret);
				ret = 0;
			}
			cinfo.name = name;
			cinfo.type = 2;
			cinfo.costOrSlot = 5;
			this->_allOpponentSpellCards.emplace_back(id, ret, cost);
		} else if (type == 1) {
			parser.getNextCell(); // Desc
			parser.getNextCell();
			parser.getNextCell();
			parser.getNextCell();
			parser.getNextCell();
			parser.getNextCell();
			parser.getNextCell();
			parser.getNextCell();
			parser.getNextCell();
			parser.getNextCell();

			auto it = map.find(cost);
			auto sort = parser.getNextValue();

			if (sort % 10 > 2)
				continue;
			if (it == map.end()) {
				map.emplace(cost, this->_opponentSkillCards.size());
				this->_opponentSkillCards.emplace_back(cost, std::array<int, 3>{0, 0, 0});
				it = map.find(cost);
			}
			if (SokuLib::textureMgr.loadTexture(&ret, buffer, nullptr, nullptr))
				this->_opponentSkillCards[it->second].second[sort % 10] = ret;
		}
	} while (parser.goToNextLine());

	DWORD old;
	const char *old_pat;
	const char *old_pal;
	const char *old_tex;
	char name[] = "animal10";

	VirtualProtect((void *)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	old_pat = SokuLib::TamperDword(0x467A5F, "data/character/mamizou/%s/%s.pat");
	old_pal = SokuLib::TamperDword(0x467AEC, "data/character/mamizou/%s/palette%03d.bmp");
	old_tex = SokuLib::TamperDword(0x467BC9, "data/character/mamizou/%s/%s");

	for (size_t i = 0; i < NB_ANIMALS; i++) {
		SokuLib::Map<int, SokuLib::v2::CharacterSequenceData*> data;

		sprintf(name, "animal%i", i + 1);
		SokuLib::readPattern(name, 0, &data, &this->gameData.opponent->patternData, this->gameData.opponent->textures);
		(*this->gameData.opponent->gameData.patternMap)[2000 + i * 2] = data[0];
		(*this->gameData.opponent->gameData.patternMap)[2001 + i * 2] = data[70];
	}

	SokuLib::TamperDword(0x467A5F, old_pat);
	SokuLib::TamperDword(0x467AEC, old_pal);
	SokuLib::TamperDword(0x467BC9, old_tex);
	VirtualProtect((void *)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);

	for (auto action : meleeActions) {
		SokuLib::v2::CharacterSequenceData *sequence = (*this->gameData.patternMap)[action];

		this->_originalMelees[action] = sequence;
		this->_spellMeleesStorage.push_back(*sequence);

		SokuLib::v2::CharacterSequenceData *first = &this->_spellMeleesStorage.back();
		SokuLib::v2::CharacterSequenceData *current = first;
		SokuLib::v2::CharacterSequenceData *last = nullptr;

		while (true) {
			for (auto &frame : current->frames) {
				if (frame.damage != 0)
					frame.attackFlags.unk1000 = true;
			}
			current->previous = last;
			if (current->next == sequence) {
				current->next = first;
				break;
			}
			this->_spellMeleesStorage.push_back(*current->next);
			current->next = &this->_spellMeleesStorage.back();
			last = current;
			current = current->next;
		}
		this->_spellMelees[action] = first;
	}
}

void Mamizou::_onSkillUpgrade(int id, int oldLevel)
{
	if (id == 1)
		this->_transformKind = KIND_TIMER;
	if (id == 5)
		this->_transformKind = KIND_SINGLE_HIT;
	if (id == 9) {
		if (oldLevel < 0)
			this->_transformStacks = 0;
		this->_transformKind = KIND_STACK;
		this->_transformStacks += STACK_PER_LEVEL;
		if (this->_transformStacks >= MAX_STACKS) {
			this->_transformStacks = MAX_STACKS;
			this->_transformStackCharge = 0;
		}
	}
}

void Mamizou::_addStackGui()
{
	if (this->teamId == 0)
		this->createObject(997, 56,  380, SokuLib::RIGHT, 3);
	else
		this->createObject(997, 584, 380, SokuLib::LEFT, 3);
}

void Mamizou::_addTimerGui()
{
	float params[3] = {0, 0, 0};

	if (this->teamId == 0) {
		this->createObject(996, 56,  376, SokuLib::RIGHT, 3, params);
		params[2] = 1;
		this->createObject(996, 161, 422, SokuLib::RIGHT, 3, params);
		params[2] = 3;
		this->createObject(996, 161, 422, SokuLib::RIGHT, 3, params);
	} else {
		this->createObject(996, 584, 376, SokuLib::LEFT, 3, params);
		params[2] = 1;
		this->createObject(996, 479, 422, SokuLib::LEFT, 3, params);
		params[2] = 3;
		this->createObject(996, 479, 422, SokuLib::LEFT, 3, params);
	}
}

void Mamizou::_addRainbowGui()
{
	float params[3] = {0, 0, 0};

	if (this->teamId == 0) {
		this->createObject(995, 56,  376, SokuLib::RIGHT, 3, params);
		params[2] = 1;
		this->createObject(995, 161, 422, SokuLib::RIGHT, 3, params);
		params[2] = 3;
		this->createObject(995, 161, 422, SokuLib::RIGHT, 3, params);
	} else {
		this->createObject(995, 584, 376, SokuLib::LEFT, 3, params);
		params[2] = 1;
		this->createObject(995, 479, 422, SokuLib::LEFT, 3, params);
		params[2] = 3;
		this->createObject(995, 479, 422, SokuLib::LEFT, 3, params);
	}
}

void Mamizou::_addDebuffGui()
{
	float params[3] = {0, 0, 0};

	if (this->teamId == 0) {
		this->createObject(993, 56,  376, SokuLib::RIGHT, 3, params);
		params[2] = 1;
		this->createObject(993, 161, 422, SokuLib::RIGHT, 3, params);
	} else {
		this->createObject(993, 584, 376, SokuLib::LEFT, 3, params);
		params[2] = 1;
		this->createObject(993, 479, 422, SokuLib::LEFT, 3, params);
	}
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

void Mamizou::_fillHand()
{
	if (this->_allOpponentSpellCards.empty())
		return;
	if (this->handInfo.hand.empty())
		return;
	this->handInfo.hand.resize(5);
	this->handInfo.cardCount = 5;
	for (size_t i = 0; i < 5; i++) {
		auto &hand = this->handInfo.hand[i];
		auto &card = this->_allOpponentSpellCards[(this->_currentCard + i) % this->_allOpponentSpellCards.size()];

		if (i == 0)
			this->_spellCost = card.cost;
		hand.id = card.id + 200;
		hand.cost = 5;
		hand.sprite.setTexture2(card.tex, 0, 0, 41, 65);
	}
}

void Mamizou::_preTransformCall()
{
	memcpy(&this->_transformPlayer->inputData.keyInput, &this->inputData.keyInput, sizeof(this->inputData.keyInput));
	memcpy(&this->_transformPlayer->inputData.bufferedKeyInput, &this->inputData.bufferedKeyInput, sizeof(this->inputData.bufferedKeyInput));
	memset(&this->_transformPlayer->skilledSkillLevel, 0, sizeof(this->_transformPlayer->skilledSkillLevel));
	memset(&this->_transformPlayer->effectiveSkillLevel, 0, sizeof(this->_transformPlayer->effectiveSkillLevel));
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

	if (this->_transformKind == KIND_SPELL_TIMER) {
		auto &card = this->_allOpponentSpellCards[this->_currentCard];

		if (!this->handInfo.hand.empty()) {
			this->_transformPlayer->handInfo.hand.resize(5);
			this->_transformPlayer->handInfo.cardCount = 5;
			if (this->_transformPlayer->frameState.actionId >= SokuLib::ACTION_USING_SC_ID_200 && this->_transformPlayer->frameState.actionId < SokuLib::ACTION_SKILL_CARD) {
				this->_transformTimer = 0;
				for (auto &hand : this->_transformPlayer->handInfo.hand) {
					hand.cost = 5;
					hand.id = 204;
				}
			} else {
				for (auto &hand : this->_transformPlayer->handInfo.hand) {
					hand.cost = 5;
					hand.id = card.id;
				}
			}
		}
		for (size_t i = 0; i < this->_selectedCards.size(); i++) {
			this->_transformPlayer->skilledSkillLevel[i + this->_selectedCards.size() * this->_selectedCards[i]] = 4;
			this->_transformPlayer->effectiveSkillLevel[i + this->_selectedCards.size() * this->_selectedCards[i]] = 4;
		}
	} else {
		this->_transformPlayer->handInfo.hand.resize(this->handInfo.hand.size());
		this->_transformPlayer->handInfo.cardCount = this->handInfo.cardCount;
		if (this->_transformPlayer->frameState.actionId >= SokuLib::ACTION_USING_SC_ID_200 && this->_transformPlayer->frameState.actionId < SokuLib::ACTION_SKILL_CARD) {
			for (size_t i = 0; i < this->handInfo.hand.size(); i++) {
				this->_transformPlayer->handInfo.hand[i].cost = this->handInfo.hand[i].cost;
				this->_transformPlayer->handInfo.hand[i].id = 201;
			}
		} else {
			for (size_t i = 0; i < this->handInfo.hand.size(); i++) {
				this->_transformPlayer->handInfo.hand[i].cost = 100;
				this->_transformPlayer->handInfo.hand[i].id = 201;
			}
		}
	}
	this->_transformPlayer->handInfo.cardGauge = 0;
	this->_transformPlayer->handInfo.cardSlots = 5;
	this->_transformPlayer->spellBgId = this->spellBgId;
	this->_transformPlayer->spellBgTimer = this->spellBgTimer;
	this->_transformPlayer->attackPower = this->attackPower;
	this->_transformPlayer->defensePower = this->defensePower;
	this->_transformPlayer->unknown538 = this->unknown538;
	this->_transformPlayer->unknown53C = this->unknown53C;
	this->_transformPlayer->bonusProration = this->bonusProration;
	this->_transformPlayer->spellDmgMultiplier = this->spellDmgMultiplier;
	this->_transformPlayer->specialDmgMultiplier = this->specialDmgMultiplier;
	this->_transformPlayer->meterGainMultiplier = this->meterGainMultiplier;
	this->_transformPlayer->lifeStealMultiplier = this->lifeStealMultiplier;
	this->_transformPlayer->discardMultiplier = this->discardMultiplier;
	this->_transformPlayer->reflectDamageMultiplier = this->reflectDamageMultiplier;
	this->_transformPlayer->unknown55C = this->unknown55C;

	this->_transformPlayer->grazeTimer = this->grazeTimer;
	this->_transformPlayer->meleeInvulTimer = this->meleeInvulTimer;
	this->_transformPlayer->projectileInvulTimer = this->projectileInvulTimer;
	this->_transformPlayer->grabInvulTimer = this->grabInvulTimer;
	this->_transformPlayer->attackPower = this->attackPower;
	this->_transformPlayer->defensePower = this->defensePower;
	this->_transformPlayer->speedPower = this->speedPower;
	this->_transformPlayer->spellDmgMultiplier = this->spellDmgMultiplier;
	this->_transformPlayer->specialDmgMultiplier = this->specialDmgMultiplier;
	this->_transformPlayer->meterGainMultiplier = this->meterGainMultiplier;
	this->_transformPlayer->lifeStealMultiplier = this->lifeStealMultiplier;
	this->_transformPlayer->discardMultiplier = this->discardMultiplier;
	this->_transformPlayer->reflectDamageMultiplier = this->reflectDamageMultiplier;
	this->_transformPlayer->grimoireCount = this->grimoireCount;
	this->_transformPlayer->tenguFans = this->tenguFans;
	this->_transformPlayer->sacrificialDolls = this->sacrificialDolls;
	this->_transformPlayer->controlRod = this->controlRod;
	this->_transformPlayer->magicPotionTimeLeft = this->magicPotionTimeLeft;
	this->_transformPlayer->stopwatchTimeLeft = this->stopwatchTimeLeft;
	this->_transformPlayer->dragonStarTimeLeft = this->dragonStarTimeLeft;
	this->_transformPlayer->drops = this->drops;
	this->_transformPlayer->dropInvulTimeLeft = this->dropInvulTimeLeft;
	this->_transformPlayer->confusionDebuffTimer = this->confusionDebuffTimer;
	this->_transformPlayer->SORDebuffTimer = this->SORDebuffTimer;
	this->_transformPlayer->healCharmTimer = this->healCharmTimer;

	this->_transformPlayer->hp = this->hp;
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
	if (this->_transformKind == KIND_SPELL_TIMER) {
		if (this->_transformTimer != 0)
			this->frameState.actionId = this->_transformPlayer->frameState.actionId;
	} else if (this->_transformKind != KIND_FULL_MOVE)
		this->frameState.actionId = this->_transformPlayer->frameState.actionId;
	this->gameData.sequenceData = this->_transformPlayer->gameData.sequenceData;
	this->gameData.frameData = this->_transformPlayer->gameData.frameData;
	this->skillIndex = this->_transformPlayer->skillIndex;

	while (this->_transformPlayer->handInfo.hand.size() < this->handInfo.hand.size())
		this->handInfo.hand.pop_front();
	this->handInfo.cardCount = this->_transformPlayer->handInfo.cardCount;
	if (this->_transformKind == KIND_SPELL_TIMER) {
		this->spellBgId = 0;
		this->spellBgTimer = 10;
	} else {
		this->spellBgId = this->_transformPlayer->spellBgId;
		this->spellBgTimer = this->_transformPlayer->spellBgTimer;
	}
	this->spellStopCounter = this->_transformPlayer->spellStopCounter;
	this->hp = this->_transformPlayer->hp;
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
	this->riverMistAdditionalSpeed = this->_transformPlayer->riverMistAdditionalSpeed;

	this->grazeTimer = this->_transformPlayer->grazeTimer;
	this->meleeInvulTimer = this->_transformPlayer->meleeInvulTimer;
	this->projectileInvulTimer = this->_transformPlayer->projectileInvulTimer;
	this->grabInvulTimer = this->_transformPlayer->grabInvulTimer;
	this->attackPower = this->_transformPlayer->attackPower;
	this->defensePower = this->_transformPlayer->defensePower;
	this->speedPower = this->_transformPlayer->speedPower;
	this->spellDmgMultiplier = this->_transformPlayer->spellDmgMultiplier;
	this->specialDmgMultiplier = this->_transformPlayer->specialDmgMultiplier;
	this->meterGainMultiplier = this->_transformPlayer->meterGainMultiplier;
	this->lifeStealMultiplier = this->_transformPlayer->lifeStealMultiplier;
	this->discardMultiplier = this->_transformPlayer->discardMultiplier;
	this->reflectDamageMultiplier = this->_transformPlayer->reflectDamageMultiplier;
	this->grimoireCount = this->_transformPlayer->grimoireCount;
	this->tenguFans = this->_transformPlayer->tenguFans;
	this->sacrificialDolls = this->_transformPlayer->sacrificialDolls;
	this->controlRod = this->_transformPlayer->controlRod;
	this->magicPotionTimeLeft = this->_transformPlayer->magicPotionTimeLeft;
	this->stopwatchTimeLeft = this->_transformPlayer->stopwatchTimeLeft;
	this->dragonStarTimeLeft = this->_transformPlayer->dragonStarTimeLeft;
	this->drops = this->_transformPlayer->drops;
	this->dropInvulTimeLeft = this->_transformPlayer->dropInvulTimeLeft;
	this->confusionDebuffTimer = this->_transformPlayer->confusionDebuffTimer;
	this->SORDebuffTimer = this->_transformPlayer->SORDebuffTimer;
	this->healCharmTimer = this->_transformPlayer->healCharmTimer;

	if (this->_transformKind == KIND_SPELL_TIMER) {
		if (this->_transformTimer == 0 &&(
			this->_transformPlayer->frameState.actionId < SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN ||
			this->_transformPlayer->frameState.actionId >= SokuLib::ACTION_FORWARD_DASH
		) && (
			this->_transformPlayer->frameState.actionId != this->frameState.actionId ||
			this->_transformPlayer->frameState.actionId < SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN
		)) {
			this->_unTransform();
			if (
				this->_transformPlayer->frameState.actionId < SokuLib::ACTION_5A &&
				this->gameData.patternMap->find(this->_transformPlayer->frameState.actionId) != this->gameData.patternMap->end()
			)
				this->setAction(this->_transformPlayer->frameState.actionId);
			else
				this->setAction(this->isOnGround() ? SokuLib::ACTION_IDLE : SokuLib::ACTION_FALLING);
			this->_transformTimerDelay = 0;
			this->_transformTimer = TIMER_MAX;
			this->_transformedCooldown = false;
			this->handInfo.hand.clear();
			this->handInfo.cardCount = 0;
		}
	} else if (this->_transformPlayer->frameState.actionId != this->frameState.actionId) {
		this->_unTransform();
		if (
			this->_transformPlayer->frameState.actionId < SokuLib::ACTION_5A &&
			this->gameData.patternMap->find(this->_transformPlayer->frameState.actionId) != this->gameData.patternMap->end()
		)
			this->setAction(this->_transformPlayer->frameState.actionId);
	}

	this->attackPower = this->_transformPlayer->attackPower;
	this->defensePower = this->_transformPlayer->defensePower;
	this->unknown538 = this->_transformPlayer->unknown538;
	this->unknown53C = this->_transformPlayer->unknown53C;
	this->bonusProration = this->_transformPlayer->bonusProration;
	this->spellDmgMultiplier = this->_transformPlayer->spellDmgMultiplier;
	this->specialDmgMultiplier = this->_transformPlayer->specialDmgMultiplier;
	this->meterGainMultiplier = this->_transformPlayer->meterGainMultiplier;
	this->lifeStealMultiplier = this->_transformPlayer->lifeStealMultiplier;
	this->discardMultiplier = this->_transformPlayer->discardMultiplier;
	this->reflectDamageMultiplier = this->_transformPlayer->reflectDamageMultiplier;
	this->unknown55C = this->_transformPlayer->unknown55C;
}

void Mamizou::_preUntransformCall()
{
	this->_transformPlayer->riverMistAdditionalSpeed = 0;
}

void Mamizou::_postUntransformCall()
{
	this->riverMistAdditionalSpeed += this->_transformPlayer->riverMistAdditionalSpeed;
}

void Mamizou::_transform(bool spawnEffects, bool resetTransformee)
{
	this->_transformed = true;
	this->_neverTransformed = false;
	this->_savedFrameData = this->gameData.frameData;
	this->_savedSequenceData = this->gameData.sequenceData;
	this->_transformPlayer->collisionType = COLLISION_TYPE_NONE;
	if (resetTransformee) {
		if (this->isGrounded())
			this->_transformPlayer->setAction(SokuLib::ACTION_IDLE);
		else
			this->_transformPlayer->setAction(SokuLib::ACTION_FALLING);
	}
	this->playSFX(0);
	if (!spawnEffects)
		return;
	for (int i = 0; i < 50; i++) {
		float params[9];
		float x = this->position.x + SokuLib::rand(150) - 75;
		float y = this->position.y + SokuLib::rand(200);

		params[0] = std::atan2(this->position.y + 100 - y, this->position.x - x) + M_PI;
		params[1] = SokuLib::rand(200) / 100.f + 0.5;
		params[2] = SokuLib::rand(4);
		params[3] = SokuLib::rand(10) + 10;
		params[4] = SokuLib::rand(40) - 20;
		params[5] = 1;
		params[6] = 1;
		params[7] = 0;
		params[8] = 0;
		this->createObject(802, x, y, SokuLib::rand(100) < 50 ? -1 : 1, 1, params);
	}
}

void Mamizou::_unTransform()
{
	if (this->_transformKind == KIND_TIMER)
		this->_transformTimerDelay = TIMER_DELAY;

	if (this->skilledSkillLevel[1] >= 0)
		this->_transformKind = KIND_TIMER;
	if (this->skilledSkillLevel[5] >= 0)
		this->_transformKind = KIND_SINGLE_HIT;
	if (this->skilledSkillLevel[9] >= 0)
		this->_transformKind = KIND_STACK;

	this->_transformed = false;
	this->gameData.frameData = this->_savedFrameData;
	this->gameData.sequenceData = this->_savedSequenceData;
	this->confusionDebuffTimer = this->_transformPlayer->confusionDebuffTimer;
	this->SORDebuffTimer = this->_transformPlayer->SORDebuffTimer;
	this->_transformPlayer->setAction(SokuLib::ACTION_IDLE);
	for (int i = 0; i < 50; i++) {
		float params[9];
		float x = this->position.x + SokuLib::rand(150) - 75;
		float y = this->position.y + SokuLib::rand(200);

		params[0] = std::atan2(this->position.y + 100 - y, this->position.x - x) + M_PI;
		params[1] = SokuLib::rand(200) / 100.f + 0.5;
		params[2] = SokuLib::rand(4);
		params[3] = SokuLib::rand(10) + 10;
		params[4] = SokuLib::rand(40) - 20;
		params[5] = 1;
		params[6] = 1;
		params[7] = 0;
		params[8] = 0;
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

void __declspec(naked) checkIdleAnims()
{
	__asm {
		CMP word ptr [EAX + 0x13C], 2000
		JGE backToGame

		CMP word ptr [EAX + 0x13C], DX
	backToGame:
		RET
	}
}

static unsigned char hook1_og[7];
static unsigned char hook2_og[7];

void Mamizou::hook()
{
	DWORD old;

	// TODO: Replace with some smarter hook method in case another character wants to hook the same places
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	*(char *)0x48894C = 0xEB;
	og_FUN_0046a240 = SokuLib::TamperNearJmpOpr(0x46E07E, FUN_0046a240);

	memcpy(hook1_og, (void *)0x46E143, 7);
	SokuLib::TamperNearCall(0x46E143, checkIdleAnims);
	*(unsigned short *)0x46E148 = 0x9090;

	memcpy(hook1_og, (void *)0x46E16D, 7);
	SokuLib::TamperNearCall(0x46E16D, checkIdleAnims);
	*(unsigned short *)0x46E172 = 0x9090;
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);
}

void Mamizou::unhook()
{
	DWORD old;

	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	*(char *)0x48894C = 0x74;
	SokuLib::TamperNearJmpOpr(0x46E07E, og_FUN_0046a240);

	memcpy((void *)0x46E143, hook1_og, 7);
	memcpy((void *)0x46E16D, hook2_og, 7);
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);
}
