//
// Created by PinkySmile on 18/10/2025.
//

#ifndef CHARACTERENGINE_DEMONGATE_HPP
#define CHARACTERENGINE_DEMONGATE_HPP


#include "BasicObject.hpp"


class DemonGate : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_DEMONGATE_HPP
