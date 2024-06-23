//
// Created by PinkySmile on 08/05/24.
//

#include "RotatingBlockingObject.hpp"

void RotatingBlockingObject::update()
{
	this->renderInfos.zRotation += 4;
	BlockingObject::update();
}
