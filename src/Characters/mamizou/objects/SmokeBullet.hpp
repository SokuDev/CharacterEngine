//
// Created by PinkySmile on 15/01/25.
//

#ifndef CHARACTERENGINE_SMOKEBULLET_HPP
#define CHARACTERENGINE_SMOKEBULLET_HPP


#include "BasicObject.hpp"

class SmokeBullet : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_SMOKEBULLET_HPP
