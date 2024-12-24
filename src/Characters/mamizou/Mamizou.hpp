//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_TEWI_HPP
#define CHARACTERENGINE_TEWI_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

class Mamizou : public SokuLib::v2::Player {
private:
	Player *_transformPlayer = nullptr;
	bool _transformed = false;
	bool _init = false;
	bool _forward = true;
	SokuLib::v2::CharacterFrameData *_savedFrameData;
	SokuLib::v2::CharacterSequenceData *_savedSequenceData;

	static constexpr float BACKDASH_DECEL = 2;
	static constexpr float BACKDASH_IMPULSE = -17.0;
	static constexpr float BACKDASH_IMPULSE_UP = 3.5;
	static constexpr float BACKDASH_GRAVITY = 0.75;
	static constexpr float BE4_DECEL = 3;
	static constexpr float BE4_IMPULSE = -12.5;
	static constexpr float BE4_IMPULSE_UP = 4;
	static constexpr float BE4_GRAVITY = 0.5;

	static constexpr float FAR_RANGE = 80;

	static constexpr float MAX_DASH_HOLD = 45;

	static constexpr float FRONT_WALK_SPEED = 4;
	static constexpr float BACK_WALK_SPEED = -3;

	static constexpr float JUMP_SPEED_X = 7;
	static constexpr float JUMP_SPEED_Y = 20;
	static constexpr float JUMP_GRAVITY = 1;
	static constexpr float HIGH_JUMP_SPEED_X = 10;
	static constexpr float N_HIGH_JUMP_FD_SPEED_X = 3;
	static constexpr float F_HIGH_JUMP_FD_SPEED_X = 12;
	static constexpr float HIGH_JUMP_SPEED_Y = 19;
	static constexpr float HIGH_JUMP_SPEED_NEUTRAL_Y = 22;
	static constexpr float HIGH_JUMP_GRAVITY = 0.9;

	static constexpr float FALLING_GRAVITY = 1;

	static constexpr float INITIAL_DASH_SPEED = 15;
	static constexpr float TOP_DASH_SPEED = 10;
	static constexpr float INITIAL_BE6_SPEED = 7;
	static constexpr float TOP_BE6_SPEED = 5;
	static constexpr float DASH_RECOVERY_DECEL = 1;

	static constexpr float FLIGHT_TURN = 1.0;
	static constexpr float FLIGHT_TURN_SUNNY = 2.0;
	static constexpr float FLIGHT_SPEED = 12;

	static constexpr float FAD_SPEED_X = 12;
	static constexpr float FAD_SPEED_Y = 4;
	static constexpr float FAD_GRAVITY = 0.45;

	static constexpr float BAD_SPEED_X = (-12);
	static constexpr float BAD_SPEED_Y = 4;
	static constexpr float BAD_GRAVITY = 0.45;


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

	bool _useSkill(bool input, unsigned char id, unsigned short action);
	bool _checkDashSlide();
	void _jumpUpdate(float xSpeed);
	void _highJumpUpdate(float xSpeed, float ySpeed, float gravity);

	void _preTransformCall();
	void _postTransformCall();
	void _transform();
	void _unTransform();

public:
	enum Moves {
		ACTION_d623B = 500,
	};

	Mamizou(SokuLib::PlayerInfo &info);
	~Mamizou() override = default;

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
};


#endif //CHARACTERENGINE_TEWI_HPP
