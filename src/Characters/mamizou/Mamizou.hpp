//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_TEWI_HPP
#define CHARACTERENGINE_TEWI_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

class Mamizou : public SokuLib::v2::Player {
private:
	static constexpr unsigned MAX_STACKS = 8;
	static constexpr unsigned STACK_PER_LEVEL = 2;
	static constexpr unsigned METER_PER_STACK = 150;
	static constexpr unsigned TIMER_COOLDOWN_MAX = 900;
	static constexpr unsigned TIMER_MAX = 450;
	static constexpr unsigned TIMER_DELAY = 150;

	friend class TimerGui;
	friend class StackGui;

	enum TransformKind : char {
		KIND_TIMER,
		KIND_SINGLE_HIT,
		KIND_STACK,
		KIND_FULL_MOVE
	};

	Player *_transformPlayer = nullptr;
	unsigned _transformTimerDelay = TIMER_DELAY;
	unsigned _transformTimer = TIMER_MAX;
	unsigned _transformStacks = 0;
	unsigned _transformStackCharge = 0;
	bool _transformedCooldown = false;
	bool _transformed = false;
	bool _init = false;
	bool _forward = true;
	TransformKind _transformKind = KIND_TIMER;
	SokuLib::v2::CharacterFrameData *_savedFrameData;
	SokuLib::v2::CharacterSequenceData *_savedSequenceData;

	static constexpr float BACKDASH_DECEL = 2;
	static constexpr float BACKDASH_IMPULSE = -10.0;
	static constexpr float BACKDASH_IMPULSE_UP = 4.5;
	static constexpr float BACKDASH_GRAVITY = 0.75;
	static constexpr float BE4_DECEL = 3;
	static constexpr float BE4_IMPULSE = -12.5;
	static constexpr float BE4_IMPULSE_UP = 4;
	static constexpr float BE4_GRAVITY = 0.5;

	static constexpr float FAR_RANGE = 110;

	static constexpr float MAX_DASH_HOLD = 45;

	static constexpr float FRONT_WALK_SPEED = 4;
	static constexpr float BACK_WALK_SPEED = -3;

	static constexpr float JUMP_SPEED_X = 5;
	static constexpr float JUMP_SPEED_Y = 15;
	static constexpr float JUMP_GRAVITY = 0.6;
	static constexpr float HIGH_JUMP_SPEED_X = 8;
	static constexpr float N_HIGH_JUMP_FD_SPEED_X = 3;
	static constexpr float F_HIGH_JUMP_FD_SPEED_X = 10;
	static constexpr float HIGH_JUMP_SPEED_Y = 13;
	static constexpr float HIGH_JUMP_SPEED_NEUTRAL_Y = 16.5;
	static constexpr float HIGH_JUMP_GRAVITY = 0.5;

	static constexpr float FALLING_GRAVITY = 0.6;

	static constexpr float INITIAL_DASH_SPEED = 10;
	static constexpr float TOP_DASH_SPEED = 8;
	static constexpr float INITIAL_BE6_SPEED = 5;
	static constexpr float TOP_BE6_SPEED = 3;
	static constexpr float DASH_RECOVERY_DECEL = 0.75;

	static constexpr float FLIGHT_TURN = 3.5;
	static constexpr float FLIGHT_TURN_SUNNY = 5;
	static constexpr float FLIGHT_SPEED = 9;

	static constexpr float FAD_SPEED_X = 8;
	static constexpr float FAD_SPEED_Y = 3.5;
	static constexpr float FAD_GRAVITY = 0.35;

	static constexpr float BAD_SPEED_X = (-8);
	static constexpr float BAD_SPEED_Y = 3.5;
	static constexpr float BAD_GRAVITY = 0.35;

	bool _tryDoUntransformedMove(bool cardsOnly, bool spellsOnly);

	bool _processAGrounded();
	bool _processBGrounded();
	bool _processCGrounded();
	bool _processSkillsGrounded();
	void _processInputsGrounded();

	bool _processAAirborne();
	bool _processBAirborne();
	bool _processCAirborne();
	bool _processSkillsAirborne();
	void _processInputsAirborne();

	virtual bool _canUseCard(int id);
	bool _useSkillCard(int id);
	bool _useSpellCard(int id);

	void _onSkillUpgrade(int id, int oldLevel);
	bool _useSkill(bool input, unsigned char id, unsigned short action);
	bool _checkDashSlide();
	void _jumpUpdate(float xSpeed);
	void _highJumpUpdate(float xSpeed, float ySpeed, float gravity);

	void _preTransformCall();
	void _postTransformCall();
	void _transform(bool spawnEffects = true);
	void _unTransform();

	void _addTimerGui();
	void _addStackGui();

public:
	enum Moves {
		ACTION_d623b = SokuLib::ACTION_DEFAULT_SKILL1_B,

		ACTION_d22b = SokuLib::ACTION_DEFAULT_SKILL2_B,
		ACTION_d22b_UNTRANSFORM = SokuLib::ACTION_DEFAULT_SKILL2_C,
		ACTION_FORCE_TIMER_UNTRANSFORM = SokuLib::ACTION_DEFAULT_SKILL2_AIR_B,
		ACTION_FORCE_TIMER_UNTRANSFORM_AIR = SokuLib::ACTION_DEFAULT_SKILL2_AIR_C,

		ACTION_d214b = SokuLib::ACTION_DEFAULT_SKILL3_B,
		ACTION_d214c = SokuLib::ACTION_DEFAULT_SKILL3_C,
		ACTION_jd214b = SokuLib::ACTION_DEFAULT_SKILL3_AIR_B,
		ACTION_jd214c = SokuLib::ACTION_DEFAULT_SKILL3_AIR_C,

		ACTION_d236b = SokuLib::ACTION_DEFAULT_SKILL4_B,
		ACTION_d236c = SokuLib::ACTION_DEFAULT_SKILL4_C,
		ACTION_jd236b = SokuLib::ACTION_DEFAULT_SKILL4_AIR_B,
		ACTION_jd236c = SokuLib::ACTION_DEFAULT_SKILL4_AIR_C,

		ACTION_a1_22b = SokuLib::ACTION_ALT1_SKILL2_B,
		ACTION_a1_22b_UNTRANSFORM = SokuLib::ACTION_ALT1_SKILL2_C,
		ACTION_ja1_22b = SokuLib::ACTION_ALT1_SKILL2_AIR_B,
		ACTION_ja1_22b_UNTRANSFORM = SokuLib::ACTION_ALT1_SKILL2_AIR_C,

		ACTION_a2_22b = SokuLib::ACTION_ALT2_SKILL2_B,
		ACTION_a2_22c = SokuLib::ACTION_ALT2_SKILL2_C,
		ACTION_ja2_22b = SokuLib::ACTION_ALT2_SKILL2_AIR_B,
	};

	Mamizou(SokuLib::PlayerInfo &info);
	~Mamizou() override;

	void render() override;
	void update() override;
	void initializeAction() override;
	void handleInputs() override;
	void VUnknown58() override;
	void VUnknown5C() override;
	bool VUnknown60(int a) override;
	bool setAction(short action) override;

	void render2() override;
	void onRenderEnd() override;
	void setActionSequence(short action, short sequence) override;
	void setSequence(short sequence) override;

	void resetSequence() override;
	bool nextSequence() override;
	void prevSequence() override;
	void setPose(short pose) override;
	bool nextPose() override;
	void prevPose() override;
	void applyTransform() override;
	void updatePhysics() override;
	void initialize() override;
	bool VUnknown48() override;
	bool VUnknown4C(int a) override;

	static void hook();
	static void unhook();

	bool isTransformed() const;
	SokuLib::v2::Player *getTransformPlayer() const;

	friend class MamizouGameObjectList;
};


#endif //CHARACTERENGINE_TEWI_HPP
