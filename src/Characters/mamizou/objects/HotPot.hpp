//
// Created by PinkySmile on 09/02/25.
//

#ifndef CHARACTERENGINE_HOTPOT_HPP
#define CHARACTERENGINE_HOTPOT_HPP


#include "BasicObject.hpp"

class HotPot : public BasicObject {
private:
	void _handleCollisions(SokuLib::v2::Player *player, bool canHit);

public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_HOTPOT_HPP
