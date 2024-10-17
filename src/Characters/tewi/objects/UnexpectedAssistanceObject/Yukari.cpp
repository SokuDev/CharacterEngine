//
// Created by PinkySmile on 15/10/24.
//

#include "Yukari.hpp"

void UnexpectedAssistance::Yukari::update()
{
	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	if (this->frameState.sequenceId == 0) {
		float f[1] = {1.f + (this->speed.x != 0)};

		if (this->position.y < -300) {
			this->lifetime = 0;
			return;
		}
		if (this->frameState.currentFrame < 41)
			this->position.y = 1300;
		else if (this->frameState.currentFrame == 41) {
			if (this->position.y < this->gameData.opponent->position.y + 100)
				this->position.y = this->gameData.opponent->position.y + 100;

			unsigned char options = 1;
			unsigned time = (this->position.y - this->gameData.opponent->position.y - 100) / -(float)this->speed.y;
			unsigned travel = time * -this->speed.y;

			this->parentPlayerB->playSFX(29);
			options += this->gameData.opponent->position.x - travel > -700;
			options += this->gameData.opponent->position.x + travel < 1980;

			unsigned char option = SokuLib::rand(options);

			if (option >= options)
				option = options - 1;
			if (option == 0) {
				this->position.x = this->gameData.opponent->position.x;
				this->speed.x = 0;
			} else if (option == 1 && this->gameData.opponent->position.x - travel > -700) {
				this->position.x = this->gameData.opponent->position.x - travel;
				this->speed.x = -this->speed.y;
			} else {
				this->position.x = this->gameData.opponent->position.x + travel;
				this->speed.x = this->speed.y;
			}
			this->collisionLimit = 1;
		} else {
			unsigned time = this->position.y / -(float)this->speed.y;

			if (time == 26)
				this->createObject(this->frameState.actionId, this->position.x + this->speed.x * 28 - this->direction * 90, 0, this->direction, -1, f);
		}
		this->renderInfos.zRotation -= 35;
		this->position += this->speed;
	}
	if (this->advanceFrame())
		this->lifetime = 0;
}

void UnexpectedAssistance::Yukari::initializeAction()
{
	if (this->customData) {
		this->renderInfos.scale.x = 2;
		this->renderInfos.scale.y = 2;
		this->setSequence(1);
	} else {
		this->center.x = 0;
		this->center.y = 100;
		this->parentPlayerB->playSFX(28);
		this->speed.y = -25;
	}
}


void UnexpectedAssistance::Yukari::onRenderEnd()
{
	if (this->frameState.sequenceId != 0)
		return AnimationObject::onRenderEnd();

	float floor = SokuLib::camera.translate.y * SokuLib::camera.scale;
	SokuLib::DxVertex vertices[6];
	SokuLib::DxVertex points[4];
	int index = 0;
	float mini = this->sprite.transfCoords[0].y;
	unsigned char indices[4][4] = {
		{0, 1, 2, 3},
		{1, 3, 0, 2},
		{2, 3, 0, 1},
		{3, 1, 2, 0}
	};

	for (int i = 1; i < 4; i++)
		if (this->sprite.transfCoords[i].y < mini) {
			mini = this->sprite.transfCoords[i].y;
			index = i;
		}

	for (int i = 0; i < 4; i++) {
		auto &coord = this->sprite.transfCoords[indices[index][i]];

		points[i] = this->sprite.vertices[indices[index][i]];
		points[i].x = coord.x;
		points[i].y = coord.y;
		points[i].z = coord.z;
		points[i].rhw = 1;
	}

	if (
		this->sprite.transfCoords[0].y <= floor &&
		this->sprite.transfCoords[1].y <= floor &&
		this->sprite.transfCoords[2].y <= floor &&
		this->sprite.transfCoords[3].y <= floor
	) {
		AnimationObject::onRenderEnd();
		return;
	}

	if (points[0].y > floor)
		return;

	float step;

	SokuLib::textureMgr.setTexture(this->sprite.dxHandle, 0);
	SokuLib::pd3dDev->SetFVF(0x144);
	if (points[1].y > floor && points[2].y > floor) {
		vertices[0] = points[0];
		vertices[1] = points[1];
		vertices[2] = points[2];
		for (int i = 0; i < 2; i++) {
			float percent = (floor - points[0].y) / (points[i + 1].y - points[0].y);

			vertices[i + 1].x = points[0].x + (points[i + 1].x - points[0].x) * percent;
			vertices[i + 1].y = points[0].y + (points[i + 1].y - points[0].y) * percent;
			vertices[i + 1].z = points[0].z + (points[i + 1].z - points[0].z) * percent;
			vertices[i + 1].u = points[0].u + (points[i + 1].u - points[0].u) * percent;
			vertices[i + 1].v = points[0].v + (points[i + 1].v - points[0].v) * percent;
		}
		SokuLib::pd3dDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, vertices, sizeof(*vertices));
		return;
	}
	if (points[1].y > floor && points[2].y < floor) {
		vertices[0] = points[0];
		vertices[1] = points[1];
		vertices[2] = points[2];
		vertices[3] = points[2];

		step = (floor - points[0].y) / (points[1].y - points[0].y);
		vertices[1].x = points[0].x + (points[1].x - points[0].x) * step;
		vertices[1].y = points[0].y + (points[1].y - points[0].y) * step;
		vertices[1].z = points[0].z + (points[1].z - points[0].z) * step;
		vertices[1].u = points[0].u + (points[1].u - points[0].u) * step;
		vertices[1].v = points[0].v + (points[1].v - points[0].v) * step;

		step = (floor - points[3].y) / (points[2].y - points[3].y);
		vertices[2].x = points[3].x + (points[2].x - points[3].x) * step;
		vertices[2].y = points[3].y + (points[2].y - points[3].y) * step;
		vertices[2].z = points[3].z + (points[2].z - points[3].z) * step;
		vertices[2].u = points[3].u + (points[2].u - points[3].u) * step;
		vertices[2].v = points[3].v + (points[2].v - points[3].v) * step;
		SokuLib::pd3dDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(*vertices));
		return;
	}
	if (points[2].y > floor && points[1].y < floor) {
		vertices[0] = points[0];
		vertices[1] = points[1];
		vertices[2] = points[3];
		vertices[3] = points[2];

		step = (floor - points[1].y) / (points[3].y - points[1].y);
		vertices[2].x = points[1].x + (points[3].x - points[1].x) * step;
		vertices[2].y = points[1].y + (points[3].y - points[1].y) * step;
		vertices[2].z = points[1].z + (points[3].z - points[1].z) * step;
		vertices[2].u = points[1].u + (points[3].u - points[1].u) * step;
		vertices[2].v = points[1].v + (points[3].v - points[1].v) * step;

		step = (floor - points[0].y) / (points[2].y - points[0].y);
		vertices[3].x = points[0].x + (points[2].x - points[0].x) * step;
		vertices[3].y = points[0].y + (points[2].y - points[0].y) * step;
		vertices[3].z = points[0].z + (points[2].z - points[0].z) * step;
		vertices[3].u = points[0].u + (points[2].u - points[0].u) * step;
		vertices[3].v = points[0].v + (points[2].v - points[0].v) * step;
		SokuLib::pd3dDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(*vertices));
		return;
	}
	if (points[3].y <= floor) {
#ifdef _DEBUG
		_wassert(L"points[3].y > floor", _CRT_WIDE(__FILE__), __LINE__);
#else
		return AnimationObject::onRenderEnd();
#endif
	}
	vertices[0] = points[0];
	vertices[1] = points[1];
	vertices[2] = points[3];
	vertices[3] = points[3];
	vertices[4] = points[2];

	step = (floor - points[1].y) / (points[3].y - points[1].y);
	vertices[2].x = points[1].x + (points[3].x - points[1].x) * step;
	vertices[2].y = points[1].y + (points[3].y - points[1].y) * step;
	vertices[2].z = points[1].z + (points[3].z - points[1].z) * step;
	vertices[2].u = points[1].u + (points[3].u - points[1].u) * step;
	vertices[2].v = points[1].v + (points[3].v - points[1].v) * step;

	step = (floor - points[2].y) / (points[3].y - points[2].y);
	vertices[3].x = points[2].x + (points[3].x - points[2].x) * step;
	vertices[3].y = points[2].y + (points[3].y - points[2].y) * step;
	vertices[3].z = points[2].z + (points[3].z - points[2].z) * step;
	vertices[3].u = points[2].u + (points[3].u - points[2].u) * step;
	vertices[3].v = points[2].v + (points[3].v - points[2].v) * step;
	SokuLib::pd3dDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 3, vertices, sizeof(*vertices));
}
