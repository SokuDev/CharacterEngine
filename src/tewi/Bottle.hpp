//
// Created by PinkySmile on 28/07/24.
//

#ifndef CHARACTERENGINE_BOTTLE_HPP
#define CHARACTERENGINE_BOTTLE_HPP


#include "BasicObject.hpp"

class Bottle : public BasicObject {
public:
	void update() override;
	bool initializeAction() override;
};


#endif //CHARACTERENGINE_BOTTLE_HPP
