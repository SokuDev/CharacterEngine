//
// Created by PinkySmile on 02/08/24.
//

#include <cmath>
#include "SpellTrapHole.hpp"

#define HOLE_RADIUS 180
#define HOLE_DEPTH 150
#define HOLE_FADE_IN_OFFSET 25
#define HOLE_FADE_IN 90
#define HOLE_FADE_OUT 30
#define STAY_TIME 8
#define NB_RABBIT 40
#define rabbitShot gpShort[1]
#define rabbitShootTimer gpShort[0]
#define RABBIT_GO_TIME 20

#define convColor(c) ((((c).a & 1) << 15) | (((c).r >> 3) << 10) | (((c).g >> 3) << 5) | (((c).b >> 3) << 0))
#define frontHole(t) ((SokuLib::v2::GameObject **)&t->data[0])
#define fadeSprite(t) ((SokuLib::DrawUtils::Sprite **)&t->data[4])
#define stageSprite(t) ((SokuLib::DrawUtils::Sprite **)&t->data[4])

SpellTrapHole::~SpellTrapHole()
{
	if (this->frameState.sequenceId == 1)
		delete *stageSprite(this);
	if (this->frameState.sequenceId == 0)
		delete *fadeSprite(this);
}

void SpellTrapHole::render()
{
	AnimationObject::render();
	/*if (this->frameState.sequenceId == 1) {
		(*stageSprite(this))->setPosition((*stageSprite(this))->getPosition());
		(*stageSprite(this))->draw();
	}*/
	if (this->frameState.sequenceId == 0) {
		auto size = this->frameData->texSize.to<float>();
		auto offset = this->frameData->offset.to<float>();

		offset.x *= this->renderInfos.scale.x * this->frameData->blendOptionsPtr->scale.x;
		offset.y *= this->renderInfos.scale.y * this->frameData->blendOptionsPtr->scale.y;

		auto pos = this->position - offset;

		size.x *= this->renderInfos.scale.x * this->frameData->blendOptionsPtr->scale.x * 3;
		size.y *= this->renderInfos.scale.y * this->frameData->blendOptionsPtr->scale.y * 3;
		(*fadeSprite(this))->setSize(size.to<unsigned>());
		(*fadeSprite(this))->setPosition(pos.to<int>());
		(*fadeSprite(this))->draw();
	}
}

void SpellTrapHole::update()
{
	if (this->frameState.sequenceId == 1)
		return;
	if (this->lifetime == 0)
		return;
	if (this->frameState.sequenceId == 2) {
		if (this->checkTurnIntoCrystals(false, 0, 0)) {
			this->lifetime = 0;
			return;
		}
		this->advanceFrame();
		this->position.x = this->parentObject->position.x - (62 * 2 * this->parentObject->renderInfos.scale.x) * this->direction;
		this->position.y = this->getGroundHeight();
		if (this->parentObject->renderInfos.scale.x == 1)
			this->lifetime = 0;
		return;
	}
	if (this->frameState.sequenceId == 3) {
		if (this->parentPlayerB->timeStop)
			return;
		this->advanceFrame();
		this->position += this->speed;
		if (this->position.x < -140 || this->position.x > 1420 || this->position.y > 1000)
			this->lifetime = 0;
		return;
	}
	if (this->frameState.sequenceId == 4) {
		this->position += this->speed;
		this->speed.y -= this->gravity.y;
		if (this->speed.y < 0 && this->position.y <= 0)
			this->setSequence(2);
		return;
	}
	if (this->collisionLimit && this->renderInfos.scale.x != 1) {
		if (this->checkTurnIntoCrystals(false, 10, 50, 0, -50)) {
			this->collisionLimit = 0;
			this->collisionType = COLLISION_TYPE_NONE;
			return;
		}
		if (this->frameState.currentFrame < RABBIT_GO_TIME) {
			this->advanceFrame();
			return;
		}
		SokuLib::camera.shake = 5;

		auto f = this->frameState.currentFrame - RABBIT_GO_TIME;

		if (f % 20 == 0)
			this->parentPlayerB->playSFX(14);
		this->renderInfos.scale.x = f / ((float) HOLE_FADE_IN + 1);
		this->renderInfos.scale.y = f / ((float) HOLE_FADE_IN + 1);
		if (f > HOLE_FADE_IN) {
			this->collisionLimit = STAY_TIME;
			this->collisionType = COLLISION_TYPE_NONE;
		}
		for (int x = this->position.x - HOLE_RADIUS; x < this->position.x + HOLE_RADIUS; x++) {
			if (x < 0 || x >= 1280)
				continue;
			if (f > HOLE_FADE_IN_OFFSET)
				SokuLib::v2::groundHeight[x] = HOLE_DEPTH * (f - HOLE_FADE_IN_OFFSET) / -(float) (HOLE_FADE_IN - HOLE_FADE_IN_OFFSET);
		}
	} else if (this->collisionType == COLLISION_TYPE_HIT || this->collisionType == COLLISION_TYPE_BLOCKED) {
		if (this->parentPlayerB->timeStop)
			return;
		this->rabbitShootTimer++;
		if (this->rabbitShot < NB_RABBIT) {
			this->collisionLimit = 2;
			if ((this->rabbitShootTimer > 30 && this->gameData.opponent->position.y + this->gameData.opponent->speed.y * 2 <= 100) || this->rabbitShot) {
				if (this->rabbitShootTimer % 4 == 0) {
					auto x = this->position.x + SokuLib::rand(350) - 175.f;
					float params[] = {0, 25, 3};
					auto dir = x < this->gameData.opponent->position.x ? SokuLib::RIGHT : SokuLib::LEFT;

					params[0] = fmod(atan2(this->gameData.opponent->position.y + 225, this->gameData.opponent->position.x - x) * 180 / M_PI + 360, 360);
					this->createObject(this->frameState.actionId, x, -150, dir, 1, params);
					this->parentPlayerB->playSFX(3);
					this->rabbitShot++;
				}
				SokuLib::camera.shake = 5;
			} else
				SokuLib::camera.shake = min(this->rabbitShootTimer / 6, 5);
		} else
			this->collisionLimit = 0;
	} else if (this->collisionType || (this->collisionLimit && this->checkTurnIntoCrystals(false, 10, 50, 0, -50)))
		this->collisionLimit = 0;
	this->advanceFrame();
	if (this->frameState.poseFrame == 0 && this->collisionLimit)
		this->collisionLimit--;
	if (!this->collisionLimit) {
		if (this->parentPlayerB->timeStop)
			return;
		if (this->frameState.currentFrame > HOLE_FADE_OUT)
			this->frameState.currentFrame = 0;
		if (this->frameState.currentFrame == HOLE_FADE_OUT) {
			for (int x = this->position.x - HOLE_RADIUS; x < this->position.x + HOLE_RADIUS; x++) {
				if (x < 0 || x >= 1280)
					continue;
				SokuLib::v2::groundHeight[x] = 0;
			}
			this->lifetime = 0;
			(*frontHole(this))->lifetime = 0;
			return;
		}
		this->renderInfos.scale.x = 1 - this->frameState.currentFrame / (float)HOLE_FADE_OUT;
		this->renderInfos.scale.y = 1 - this->frameState.currentFrame / (float)HOLE_FADE_OUT;
		for (int x = this->position.x - HOLE_RADIUS; x < this->position.x + HOLE_RADIUS; x++) {
			if (x < 0 || x >= 1280)
				continue;
			SokuLib::v2::groundHeight[x] = -HOLE_DEPTH + HOLE_DEPTH * this->frameState.currentFrame / (float)HOLE_FADE_OUT;
		}
	} else if (this->renderInfos.scale.x == 1) {
		for (int x = this->position.x - HOLE_RADIUS; x < this->position.x + HOLE_RADIUS; x++) {
			if (x < 0 || x >= 1280)
				continue;
			SokuLib::v2::groundHeight[x] = -HOLE_DEPTH;
		}
	}
	if (*frontHole(this) != nullptr) {
		(*frontHole(this))->renderInfos.scale.x = this->renderInfos.scale.x;
		(*frontHole(this))->renderInfos.scale.y = this->renderInfos.scale.y;
		(*frontHole(this))->lifetime = this->lifetime;
	}
}

void SpellTrapHole::initializeAction()
{
	if (this->customData) {
		this->setSequence(this->customData[2]);
		if (this->frameState.sequenceId == 1)
			this->renderInfos.color.a = 0;
		if (this->frameState.sequenceId == 3) {
			this->speed.x = std::cos(this->customData[0] * M_PI / 180) * this->customData[1];
			this->speed.y = std::sin(this->customData[0] * M_PI / 180) * this->customData[1];
			this->collisionLimit = 1;
		}
		if (this->frameState.sequenceId == 4) {
			const auto t = RABBIT_GO_TIME;

			this->speed.x = (this->parentObject->position.x - this->position.x) / (t * 1.f);
			this->speed.y = (-this->position.y + 0.5f * t * t / 2) / (t * 1.f);
			this->direction = std::copysign(1, this->speed.x);
			this->gravity.y = 0.5;
		}
		//this->prepareTexture();
		return;
	}

	float params[] = {0, 0, 1};
	float params2[] = {0, 0, 4};

	*frontHole(this) = this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
	this->createChild(this->frameState.actionId, SokuLib::camera.leftEdge - 50, 50, this->direction, -1, params2);
	this->createChild(this->frameState.actionId, SokuLib::camera.rightEdge + 50, 50, this->direction, -1, params2);
	this->prepareFade();
	this->renderInfos.scale.x = 0;
	this->renderInfos.scale.y = 0;
	this->collisionLimit = 1;
	this->collisionType = COLLISION_TYPE_HIT;
	for (int x = this->position.x - HOLE_RADIUS; x < this->position.x + HOLE_RADIUS; x++) {
		if (x < 0 || x >= 1280)
			continue;
		SokuLib::v2::groundHeight[x] = -1;
	}
}


void SpellTrapHole::prepareFade()
{
	int resultId = 0;
	HRESULT ret;
	char alpha = 0;
	bool b = false;

	*fadeSprite(this) = new SokuLib::DrawUtils::Sprite(SokuLib::camera);

	LPDIRECT3DTEXTURE9 *pphandleOriginal = SokuLib::textureMgr.Get((*this->textures)[this->frameData->texIndex]);
	LPDIRECT3DTEXTURE9 *pphandleResult = SokuLib::textureMgr.allocate(&resultId);
	D3DLOCKED_RECT resultRect;
	D3DLOCKED_RECT originalRect;
	SokuLib::Color *resultPixels;
	unsigned short *originalPixels;
	int _scale = 6;

	EnterCriticalSection((LPCRITICAL_SECTION)0x8A0E14);
	ret = D3DXCreateTexture(SokuLib::pd3dDev, this->frameData->texSize.x * _scale, this->frameData->texSize.y * _scale, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, pphandleResult);
	if (FAILED(ret)) {
		LeaveCriticalSection((LPCRITICAL_SECTION)0x8A0E14);
		fprintf(stderr, "D3DXCreateTexture(SokuLib::pd3dDev, 200, 150, D3DX_DEFAULT, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, %p) failed with code %li\n", pphandleResult, ret);
		goto error;
	}
	LeaveCriticalSection((LPCRITICAL_SECTION)0x8A0E14);

	ret = (*pphandleOriginal)->LockRect(0, &originalRect, nullptr, 0);
	if (FAILED(ret)) {
		fprintf(stderr, "(*pphandle3)->LockRect(0, &r, nullptr, D3DLOCK_DISCARD) failed with code %li\n", ret);
		goto error;
	}
	ret = (*pphandleResult)->LockRect(0, &resultRect, nullptr, 0);
	if (FAILED(ret)) {
		(*pphandleOriginal)->UnlockRect(0);
		fprintf(stderr, "(*pphandle4)->LockRect(0, &r, nullptr, D3DLOCK_DISCARD) failed with code %li\n", ret);
		goto error;
	}


	resultPixels     = (SokuLib::Color *)resultRect.pBits;
	originalPixels   = (unsigned short *)originalRect.pBits;
	memset(resultPixels, 0, this->frameData->texSize.y * _scale * resultRect.Pitch);

	for (int y = 0; y < this->frameData->texSize.y; y++) {
		for (int x = 0; x < this->frameData->texSize.x; x++) {
			if (originalPixels[y * (originalRect.Pitch / sizeof(*originalPixels)) + x] & 0x8000) {
				for (int i = 0; i < _scale; i++) {
					for (int j = 0; j < _scale; j++) {
						resultPixels[(y * 2 + i) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + j)] = SokuLib::Color::Black;
						resultPixels[(y * 2 + i) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + j)].a = alpha + i;
					}
				}
				b = true;
			} else for (int i = 0; i < _scale; i++)
				for (int j = 0; j < _scale; j++)
					resultPixels[(y * 2 + i) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + j)] = SokuLib::Color::Transparent;
		}
		if (b)
			alpha += _scale;
	}


	ret = (*pphandleResult)->UnlockRect(0);
	if (FAILED(ret))
		fprintf(stderr, "(*pphandleResult)->UnlockRect(0) failed with code %li\n", ret);
	ret = (*pphandleOriginal)->UnlockRect(0);
	if (FAILED(ret))
		fprintf(stderr, "(*pphandleOriginal)->UnlockRect(0) failed with code %li\n", ret);


	(*fadeSprite(this))->texture.setHandle(resultId, (this->frameData->texSize * _scale).to<unsigned>());
	(*fadeSprite(this))->rect = {
		0, 0,
		static_cast<int>((*fadeSprite(this))->texture.getSize().x),
		static_cast<int>((*fadeSprite(this))->texture.getSize().y),
	};
	(*fadeSprite(this))->setSize((this->frameData->texSize * _scale).to<unsigned>());
	(*fadeSprite(this))->setPosition((this->position - this->frameData->offset).to<int>());
	return;

error:
	SokuLib::textureMgr.deallocate(resultId);
}

