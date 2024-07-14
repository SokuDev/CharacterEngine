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

	static constexpr float HAMMER_FAR_RANGE = 105;
	static constexpr float NO_HAMMER_FAR_RANGE = 80;

	static constexpr float MAX_DASH_HOLD = 45;

	static constexpr float FRONT_WALK_SPEED = 4;
	static constexpr float BACK_WALK_SPEED = -3.5;
	static constexpr float HAMMER_FRONT_WALK_SPEED = 7;
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
	static constexpr float HAMMER_TOP_BE6_SPEED = 5.5;
	static constexpr float INITIAL_DASH_SPEED = 15;
	static constexpr float TOP_DASH_SPEED = 10;
	static constexpr float TOP_BE6_SPEED = 7.5;
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

	bool _tryPickUpHammer();
	bool _canPickUpHammer();
	bool _checkDashSlide();
	void _jumpUpdate(float xSpeed);
	void _highJumpUpdate(float xSpeed, float ySpeed, float gravity);
	void _bSeriesUpdate(float angle, float speed, float slowDown, float yOffset, bool isAirborne);

public:
	enum Moves {
		ACTION_IDLE_NO_HAMMER =                               11,
		ACTION_CROUCHING_NO_HAMMER =                          12,
		ACTION_CROUCHED_NO_HAMMER =                           13,
		ACTION_STANDING_UP_NO_HAMMER =                        14,
		ACTION_WALK_FORWARD_NO_HAMMER =                       15,
		ACTION_WALK_BACKWARD_NO_HAMMER =                      16,
		ACTION_NEUTRAL_JUMP_NO_HAMMER =                       17,
		ACTION_FORWARD_JUMP_NO_HAMMER =                       18,
		ACTION_BACKWARD_JUMP_NO_HAMMER =                      19,
		ACTION_FALLING_NO_HAMMER =                            20,
		ACTION_LANDING_NO_HAMMER =                            21,
		ACTION_NEUTRAL_HIGH_JUMP_NO_HAMMER =                  216,
		ACTION_FORWARD_HIGH_JUMP_NO_HAMMER =                  217,
		ACTION_BACKWARD_HIGH_JUMP_NO_HAMMER =                 218,
		ACTION_NEUTRAL_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER = 227,
		ACTION_FORWARD_HIGH_JUMP_FROM_GROUND_DASH_NO_HAMMER = 228,
		ACTION_BE2_NO_HAMMER =                                229,
		ACTION_BE1_NO_HAMMER =                                230,
		ACTION_BE6_NO_HAMMER =                                231,
		ACTION_BE4_NO_HAMMER =                                232,

		ACTION_5AAA6A =                          SokuLib::ACTION_5AAAAA,
		ACTION_5A_HAMMER =                       310,
		ACTION_5AA_HAMMER =                      319,
		ACTION_5AAA_HAMMER =                     312,
		ACTION_4A_HAMMER =                       313,
		ACTION_66A_HAMMER =                      314,
		ACTION_2A_HAMMER =                       315,
		ACTION_3A_HAMMER =                       316,
		ACTION_6A_HAMMER =                       317,
		ACTION_f5A_HAMMER =                      318,
		ACTION_j5A_HAMMER =                      331,
		ACTION_j6A_HAMMER =                      332,
		ACTION_j2A_HAMMER =                      333,
		ACTION_j8A_HAMMER =                      334,
		ACTION_66B_HAMMER =                      327,
		ACTION_66C_HAMMER =                      328,

		ACTION_5B_HAMMER =                       401,
		ACTION_2B_HAMMER =                       403,
		ACTION_j5B_HAMMER =                      405,
		ACTION_j2B_HAMMER =                      407,
		ACTION_4C =                              413,

		ACTION_d623B =                           500,
		ACTION_d623C =                           501,
		ACTION_jd623B =                          502,
		ACTION_jd623C =                          503,
		ACTION_d623B_HAMMER =                    504,
		ACTION_d623C_HAMMER =                    505,
		ACTION_jd623B_HAMMER =                   506,
		ACTION_jd623C_HAMMER =                   507,
		ACTION_d236B =                           508,
		ACTION_d236C =                           509,
		ACTION_d236B_HAMMER =                    512,
		ACTION_d236C_HAMMER =                    513,
		ACTION_d214B =                           516,
		ACTION_d214C =                           517,
		ACTION_d214B_HAMMER =                    520,
		ACTION_d214C_HAMMER =                    521,
		ACTION_d22B =                            524,
		ACTION_d22C =                            525,
		ACTION_jd22B =                           526,
		ACTION_jd22C =                           527,
		ACTION_d22B_HAMMER =                     528,
		ACTION_d22C_HAMMER =                     529,
		ACTION_jd22B_HAMMER =                    530,
		ACTION_jd22C_HAMMER =                    531,

		ACTION_USING_SC_ID_200_HAMMER =          620,
		ACTION_USING_SC_ID_201_HAMMER =          621,
		ACTION_USING_SC_ID_201_HAMMER_AIR =      671,

		ACTION_STAND_PICKUP_HAMMER_FROM_AIR =    420,
		ACTION_STAND_PICKUP_HAMMER_FROM_GROUND = 421,
		ACTION_AIR_PICKUP_HAMMER_FROM_AIR =      422,
		ACTION_AIR_PICKUP_HAMMER_FROM_GROUND =   423,
	};

	Tewi(SokuLib::PlayerInfo &info);
	~Tewi() override = default;

	void update() override;
	bool initializeAction() override;
	void handleInputs() override;
	void VUnknown58() override;
	void VUnknown5C() override;
	bool VUnknown60(int a) override;
	bool setAction(short action) override;

	SokuLib::v2::GameObject *getHammer() const;

	static char hookBuffer;
	static void hook();
	static void unhook();
};


#endif //CHARACTERENGINE_TEWI_HPP
