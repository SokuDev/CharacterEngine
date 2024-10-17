//
// Created by PinkySmile on 18/09/24.
//

#ifndef CHARACTERENGINE_SASORB_HPP
#define CHARACTERENGINE_SASORB_HPP


#include "BasicObject.hpp"

class SaSOrb : public BasicObject {
private:
	void _checkCollisionWithOwner();

public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_SASORB_HPP
