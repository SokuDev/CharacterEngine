//
// Created by PinkySmile on 08/05/24.
//

#include "RotatingBlockingObject.hpp"

RotatingBlockingObject::RotatingBlockingObject(unsigned char offsetYAir, unsigned char offsetYHigh, unsigned char offsetYLow) :
	BlockingObject(offsetYAir, offsetYHigh, offsetYLow)
{
}

void RotatingBlockingObject::update()
{
	this->renderInfos.zRotation += 4;
	BlockingObject::update();
}
