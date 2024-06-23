//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_RotatingBlockingObject22_HPP
#define CHARACTERENGINE_RotatingBlockingObject22_HPP


#include "BasicObject.hpp"
#include "Memory.hpp"

class BlockingObject : public BasicObject {
public:
	BlockingObject() = default;
	~BlockingObject() override = default;

	void update() override;
	bool initializeAction() override;
};


#endif //CHARACTERENGINE_RotatingBlockingObject22_HPP
