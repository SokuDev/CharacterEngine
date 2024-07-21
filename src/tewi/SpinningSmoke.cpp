//
// Created by PinkySmile on 04/07/24.
//

#include "SpinningSmoke.hpp"

void SpinningSmoke::update()
{
	if (this->frameState.sequenceId != 0)
		return;

	auto fVar18 = sin(this->customData[0] * M_PI / 180);

	if (fVar18 <= 0.0)
		this->layer = 1;
	else
		this->layer = -1;
	this->customData[0] += this->unknown378;
	this->customData[1] *= 0.95;
	this->unknown378 += this->unknown37C;
	this->unknown37C += 0.1;
	this->position.x = this->direction * (this->customData[1] + 50.0) * cos(this->customData[0] * M_PI / 180) + this->parentPlayerB->position.x;
	this->position.y = (this->customData[1] + 50.0) * sin(this->customData[0] * M_PI / 180) * 0.25 + this->parentPlayerB->position.y + this->unknown37C;
	if (this->unknown36C == 0) {
		if (this->renderInfos.color.a >= 0xF6){
			this->renderInfos.color.a = 0xFF;
			this->unknown36C = 1;
		} else
			this->renderInfos.color.a += 10;
	} else {
		if (this->renderInfos.color.a < 7)
			this->lifetime = 0;
		else
			this->renderInfos.color.a -= 7;
	}
}

bool SpinningSmoke::initializeAction()
{
	this->unknown36C = 0;
	this->renderInfos.color.a = 0;
	return true;
}
