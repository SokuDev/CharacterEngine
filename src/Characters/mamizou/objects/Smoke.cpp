//
// Created by PinkySmile on 21/12/24.
//

#include <cmath>
#include "Smoke.hpp"

void Smoke::update()
{
	this->position.x += std::cos(this->customData[0]) * this->customData[1];
	this->position.y += std::sin(this->customData[0]) * this->customData[1];
	this->customData[1] -= 0.01;
	this->renderInfos.zRotation += this->customData[4];
	if (this->renderInfos.color.a <= this->customData[3]) {
		this->renderInfos.color.a = 0;
		this->lifetime = 0;
	} else
		this->renderInfos.color.a -= this->customData[3];
}

void Smoke::initializeAction()
{
	this->renderInfos.zRotation = SokuLib::rand(360);
	this->setSequence(this->customData[2]);
}

