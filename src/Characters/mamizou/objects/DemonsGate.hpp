//
// Created by PinkySmile on 18/10/2025.
//

#ifndef CHARACTERENGINE_DEMONSGATE_HPP
#define CHARACTERENGINE_DEMONSGATE_HPP


#include "BasicObject.hpp"


class DemonsGate : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_DEMONSGATE_HPP
