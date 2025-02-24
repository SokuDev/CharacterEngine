//
// Created by PinkySmile on 05/02/25.
//

#ifndef CHARACTERENGINE_LANTERNYOUKAI_HPP
#define CHARACTERENGINE_LANTERNYOUKAI_HPP


#include "BasicObject.hpp"

class LanternYoukai : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_LANTERNYOUKAI_HPP
