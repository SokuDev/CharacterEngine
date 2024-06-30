//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_TEWI_HPP
#define CHARACTERENGINE_TEWI_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

class Tewi : public SokuLib::v2::Player {
private:
	SokuLib::v2::GameObject *_hammer = nullptr;
	unsigned _hammerPickTimer = 0;

	static constexpr float HAMMER_BACKDASH_DECEL = 1.5;
	static constexpr float HAMMER_BACKDASH_IMPULSE = -12.75;
	static constexpr float BACKDASH_DECEL = 2;
	static constexpr float BACKDASH_IMPULSE = -17.0;

	static constexpr float HAMMER_FAR_RANGE = 125;
	static constexpr float NO_HAMMER_FAR_RANGE = 80;

	static constexpr float MAX_DASH_HOLD = 45;

	static constexpr float FRONT_WALK_SPEED = 4;
	static constexpr float BACK_WALK_SPEED = -3.5;
	static constexpr float HAMMER_BACK_WALK_SPEED = -6;
	static constexpr float FORWARD_WALK_SPEED_X = 12;
	static constexpr float FORWARD_WALK_SPEED_Y = 10;
	static constexpr float FORWARD_WALK_GRAVITY = 1;

	static constexpr float JUMP_SPEED_X = 7;
	static constexpr float JUMP_SPEED_Y = 20;
	static constexpr float JUMP_GRAVITY = 1;
	static constexpr float HIGH_JUMP_SPEED_X = 10;
	static constexpr float N_HIGH_JUMP_FD_SPEED_X = 3;
	static constexpr float F_HIGH_JUMP_FD_SPEED_X = 12;
	static constexpr float HIGH_JUMP_SPEED_Y = 19;
	static constexpr float HIGH_JUMP_SPEED_NEUTRAL_Y = 22;
	static constexpr float HIGH_JUMP_GRAVITY = 0.9;
	static constexpr float HAMMER_JUMP_SPEED_X = 6;
	static constexpr float HAMMER_JUMP_SPEED_Y = 15;
	static constexpr float HAMMER_JUMP_GRAVITY = 0.6;
	static constexpr float HAMMER_HIGH_JUMP_SPEED_X = 8;
	static constexpr float HAMMER_N_HIGH_JUMP_FD_SPEED_X = 2;
	static constexpr float HAMMER_F_HIGH_JUMP_FD_SPEED_X = 10;
	static constexpr float HAMMER_HIGH_JUMP_SPEED_Y = 14;
	static constexpr float HAMMER_HIGH_JUMP_SPEED_NEUTRAL_Y = 16;
	static constexpr float HAMMER_HIGH_JUMP_GRAVITY = 0.5;

	static constexpr float FALLING_GRAVITY = 1;
	static constexpr float HAMMER_FALLING_GRAVITY = 0.6;

	static constexpr float HAMMER_INITIAL_DASH_SPEED = 10;
	static constexpr float HAMMER_TOP_DASH_SPEED = 7.5;
	static constexpr float INITIAL_DASH_SPEED = 15;
	static constexpr float TOP_DASH_SPEED = 10;
	static constexpr float DASH_RECOVERY_DECEL = 1;

	static constexpr float HAMMER_FLIGHT_TURN = 0.75;
	static constexpr float HAMMER_FLIGHT_TURN_SUNNY = 1.5;
	static constexpr float HAMMER_FLIGHT_SPEED = 10;
	static constexpr float FLIGHT_TURN = 1.0;
	static constexpr float FLIGHT_TURN_SUNNY = 2.0;
	static constexpr float FLIGHT_SPEED = 12;

	static constexpr float HAMMER_FAD_SPEED_X = 9;
	static constexpr float HAMMER_FAD_SPEED_Y = 4;
	static constexpr float HAMMER_FAD_GRAVITY = 0.45;
	static constexpr float FAD_SPEED_X = 12;
	static constexpr float FAD_SPEED_Y = 4;
	static constexpr float FAD_GRAVITY = 0.45;

	static constexpr float HAMMER_BAD_SPEED_X = (-9);
	static constexpr float HAMMER_BAD_SPEED_Y = 4;
	static constexpr float HAMMER_BAD_GRAVITY = 0.45;
	static constexpr float BAD_SPEED_X = (-12);
	static constexpr float BAD_SPEED_Y = 4;
	static constexpr float BAD_GRAVITY = 0.45;

	static constexpr float HAMMER_B_BULLET_SPEED = 15;
	static constexpr float HAMMER_B_BULLET_SLOW_DOWN = 0.5;
	static constexpr float B_BULLET_SPEED = 20;
	static constexpr float B_BULLET_SLOW_DOWN = 1;


	bool _processAGrounded();
	bool _processBGrounded();
	bool _processCGrounded();
	void _processInputsGrounded();

	bool _processAAirborne();
	bool _processBAirborne();
	bool _processCAirborne();
	void _processInputsAirborne();

	virtual bool _canUseCard(int id);
	bool _useSkillCard(int id);
	bool _useSpellCard(int id);

	bool _tryPickUpHammer();
	bool _canPickUpHammer();
	bool _checkDashSlide();
	void _jumpUpdate(float xSpeed);
	void _highJumpUpdate(float xSpeed, float ySpeed, float gravity);
	void _bSeriesUpdate(float angle, float speed, float slowDown, float yOffset, bool isAirborne);

public:
	Tewi(SokuLib::PlayerInfo &info);
	~Tewi() override = default;

	void update() override;
	bool initializeAction() override;
	void handleInputs() override;
	void VUnknown58() override;
	void VUnknown5C() override;
	bool VUnknown60(int a) override;

	bool setAction(short action) override;
};


#endif //CHARACTERENGINE_TEWI_HPP
