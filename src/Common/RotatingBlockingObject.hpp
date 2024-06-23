//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_ROTATINGBLOCKINGOBJECT_HPP
#define CHARACTERENGINE_ROTATINGBLOCKINGOBJECT_HPP


#include "BlockingObject.hpp"
#include "Memory.hpp"

class RotatingBlockingObject : public BlockingObject {
public:
	RotatingBlockingObject() = default;
	~RotatingBlockingObject() override = default;

	void update() override;
};


#endif //CHARACTERENGINE_ROTATINGBLOCKINGOBJECT_HPP
