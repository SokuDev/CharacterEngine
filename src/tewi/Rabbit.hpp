//
// Created by PinkySmile on 09/07/24.
//

#ifndef CHARACTERENGINE_RABBIT_HPP
#define CHARACTERENGINE_RABBIT_HPP


#include "BasicObject.hpp"

class Rabbit : public BasicObject {
public:
	Rabbit() = default;
	~Rabbit() override = default;

	void update() override;
	bool initializeAction() override;
};


#endif //CHARACTERENGINE_RABBIT_HPP
