//
// Created by PinkySmile on 21/12/24.
//

#ifndef CHARACTERENGINE_SMOKE_HPP
#define CHARACTERENGINE_SMOKE_HPP


#include "BasicObject.hpp"

class Smoke : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_SMOKE_HPP
