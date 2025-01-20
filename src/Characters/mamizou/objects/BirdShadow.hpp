//
// Created by PinkySmile on 19/01/25.
//

#ifndef CHARACTERENGINE_BIRDSHADOW_HPP
#define CHARACTERENGINE_BIRDSHADOW_HPP


#include "BasicObject.hpp"

class BirdShadow : public BasicObject {
public:
	void update() override;

	void initializeAction() override;
};


#endif //CHARACTERENGINE_BIRDSHADOW_HPP
