//
// Created by PinkySmile on 06/08/24.
//

#ifndef CHARACTERENGINE_RABBITORB_HPP
#define CHARACTERENGINE_RABBITORB_HPP


#include "BasicObject.hpp"

class RabbitOrb : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_RABBITORB_HPP
