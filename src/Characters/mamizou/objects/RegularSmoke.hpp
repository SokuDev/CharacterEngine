//
// Created by PinkySmile on 08/02/25.
//

#ifndef CHARACTERENGINE_REGULARSMOKE_HPP
#define CHARACTERENGINE_REGULARSMOKE_HPP


#include "BasicObject.hpp"

class RegularSmoke : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_REGULARSMOKE_HPP
