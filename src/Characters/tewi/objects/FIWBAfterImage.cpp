//
// Created by PinkySmile on 23/02/25.
//

#include "FIWBAfterImage.hpp"

void FIWBAfterImage::update()
{
	if (this->parentPlayerB->hitStop)
		return;
	if (this->renderInfos.color.a < 30)
		this->lifetime = 0;
	else
		this->renderInfos.color.a -= 30;
}

void FIWBAfterImage::initializeAction()
{
	this->center = {0, 78};
	this->setPose(this->customData[1]);
	this->renderInfos.zRotation = this->customData[0];
}
