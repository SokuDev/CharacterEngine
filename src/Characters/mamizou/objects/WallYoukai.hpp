//
// Created by PinkySmile on 13/06/2026.
//

#ifndef CHARACTERENGINE_WALLYOUKAI_HPP
#define CHARACTERENGINE_WALLYOUKAI_HPP


#include "BasicObject.hpp"

class WallYoukai : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_WALLYOUKAI_HPP
