//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_HAMMER_HPP
#define CHARACTERENGINE_HAMMER_HPP


#include "BasicObject.hpp"

class Hammer : public BasicObject {
public:
	Hammer() = default;
	~Hammer() override = default;

	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_HAMMER_HPP
