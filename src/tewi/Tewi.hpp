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

	static constexpr float MAX_DASH_HOLD = 0x2D;
	static constexpr float WALK_SPEED = 4;
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
	static constexpr float FALLING_GRAVITY = 1;
	static constexpr float INITIAL_DASH_SPEED = 15;
	static constexpr float TOP_DASH_SPEED = 10;
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
	void _processInputsGrounded();

	bool _processCAirborne();
	void _processInputsAirborne();

	bool _tryPickUpHammer();
	bool _canPickUpHammer();
	bool _checkDashSlide();
	void _jumpUpdate(float xSpeed);
	void _highJumpUpdate(float xSpeed, float ySpeed, float gravity);

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
