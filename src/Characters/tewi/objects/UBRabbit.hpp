//
// Created by PinkySmile on 21/09/24.
//

#ifndef CHARACTERENGINE_UBRABBIT_HPP
#define CHARACTERENGINE_UBRABBIT_HPP


#include "BasicObject.hpp"

class UBRabbit : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_UBRABBIT_HPP
