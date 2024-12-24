//
// Created by PinkySmile on 28/09/24.
//

#ifndef CHARACTERENGINE_BAMBOO_HPP
#define CHARACTERENGINE_BAMBOO_HPP


#include "BasicObject.hpp"

class Bamboo : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_BAMBOO_HPP
