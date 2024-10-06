//
// Created by PinkySmile on 18/09/24.
//

#ifndef CHARACTERENGINE_SETANDSTRIKEORB_HPP
#define CHARACTERENGINE_SETANDSTRIKEORB_HPP


#include "BasicObject.hpp"

class SetAndStrikeOrb : public BasicObject {
private:
	void _checkCollisionWithOwner();

public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_SETANDSTRIKEORB_HPP
