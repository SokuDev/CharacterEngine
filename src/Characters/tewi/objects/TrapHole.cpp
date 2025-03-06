//
// Created by PinkySmile on 12/07/24.
//

#include "TrapHole.hpp"

#define HOLE_RADIUS 65
#define HOLE_DEPTH 100
#define HOLE_FADE_IN 15
#define HOLE_FADE_OUT 30
#define STAY_TIME(lvl) (lvl) * 2 + 10

#define convColor(c) ((((c).a & 1) << 15) | (((c).r >> 3) << 10) | (((c).g >> 3) << 5) | (((c).b >> 3) << 0))
#define fadeSprite(t) ((SokuLib::DrawUtils::Sprite **)&t->data[0])

TrapHole::~TrapHole()
{
	if (this->frameState.sequenceId == 0)
		delete *fadeSprite(this);
}

void TrapHole::render()
{
	AnimationObject::render();
	if (this->frameState.sequenceId == 0) {
		auto size = this->frameData->texSize.to<float>() * 2;
		auto offset = this->frameData->offset.to<float>();

		offset.x *= this->renderInfos.scale.x;
		offset.y *= this->renderInfos.scale.y;

		auto pos = this->position - offset;

		size.x *= this->renderInfos.scale.x;
		size.y *= this->renderInfos.scale.y;
		(*fadeSprite(this))->setSize(size.to<unsigned>());
		(*fadeSprite(this))->setPosition(pos.to<int>());
		(*fadeSprite(this))->draw();
	}
}

void TrapHole::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->lifetime == 0)
		return;
	if (this->frameState.sequenceId == 0) {
		if (this->collisionLimit && this->renderInfos.scale.x != 1) {
			this->renderInfos.scale.x = this->frameState.currentFrame / (float)HOLE_FADE_IN;
			this->renderInfos.scale.y = this->frameState.currentFrame / (float)HOLE_FADE_IN;
			if (this->frameState.currentFrame == HOLE_FADE_IN) {
				this->collisionLimit = STAY_TIME(this->parentPlayerB->effectiveSkillLevel[2]);
				this->collisionType = COLLISION_TYPE_NONE;
			}
			for (int x = this->position.x - HOLE_RADIUS; x < this->position.x + HOLE_RADIUS; x++) {
				if (x < 0 || x >= 1280)
					continue;
				SokuLib::v2::groundHeight[x] = HOLE_DEPTH * this->frameState.currentFrame / -(float)HOLE_FADE_IN;
			}
		} else if (this->collisionType)
			this->collisionLimit = 0;
		this->advanceFrame();
		if (this->frameState.poseFrame == 0 && this->collisionLimit)
			this->collisionLimit--;
		if (!this->collisionLimit) {
			if (this->frameState.currentFrame > HOLE_FADE_OUT)
				this->frameState.currentFrame = 0;
			if (this->frameState.currentFrame == HOLE_FADE_OUT) {
				for (int x = this->position.x - HOLE_RADIUS; x < this->position.x + HOLE_RADIUS; x++) {
					if (x < 0 || x >= 1280)
						continue;
					SokuLib::v2::groundHeight[x] = 0;
				}
				this->lifetime = 0;
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
				if (SokuLib::v2::groundHeight[x] <= 0 && SokuLib::v2::groundHeight[x] >= -HOLE_DEPTH)
					SokuLib::v2::groundHeight[x] = -HOLE_DEPTH;
			}
		}
	} else if (this->frameState.sequenceId == 1) {
		if (this->parentB != nullptr) {
			this->renderInfos.scale.x = this->parentB->renderInfos.scale.x;
			this->renderInfos.scale.y = this->parentB->renderInfos.scale.y;
			this->lifetime = this->parentB->lifetime;
		} else
			this->lifetime = 0;
	} else {
		if (this->renderInfos.color.a <= 15) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.zRotation += this->direction * 2;
		this->renderInfos.color.a -= 15;
		this->position.x += this->customData[0];
		this->position.y += this->customData[1];
		this->customData[1] -= 0.5;
	}
}

void TrapHole::initializeAction()
{
	if (
		this->gameData.opponent->damageLimited &&
		this->gameData.opponent->position.x > this->position.x - HOLE_RADIUS &&
		this->gameData.opponent->position.x < this->position.x + HOLE_RADIUS
	) {
		this->lifetime = 0;
		return;
	}
	if (this->customData) {
		this->setSequence(this->customData[2]);
		if (this->frameState.sequenceId == 1)
			this->renderInfos.color.a = 0;
		return;
	}

	for (int x = this->position.x - HOLE_RADIUS; x < this->position.x + HOLE_RADIUS; x++) {
		if (x < 0 || x >= 1280)
			continue;
		if (SokuLib::v2::groundHeight[x] == 0)
			continue;
		this->lifetime = 0;
		return;
	}

	float arr[] = {0, 0, 1};

	this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, arr);
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


void TrapHole::prepareFade()
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

	EnterCriticalSection((LPCRITICAL_SECTION)0x8A0E14);
	ret = D3DXCreateTexture(SokuLib::pd3dDev, this->frameData->texSize.x * 2, this->frameData->texSize.y * 2, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, pphandleResult);
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
	memset(resultPixels, 0, this->frameData->texSize.y * 2 * resultRect.Pitch);

	for (int y = 0; y < this->frameData->texSize.y; y++) {
		for (int x = 0; x < this->frameData->texSize.x; x++) {
			if (originalPixels[y * (originalRect.Pitch / sizeof(*originalPixels)) + x] & 0x8000) {
				resultPixels[(y * 2 + 0) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 0)] = SokuLib::Color::Black;
				resultPixels[(y * 2 + 0) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 1)] = SokuLib::Color::Black;
				resultPixels[(y * 2 + 1) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 0)] = SokuLib::Color::Black;
				resultPixels[(y * 2 + 1) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 1)] = SokuLib::Color::Black;
				resultPixels[(y * 2 + 0) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 0)].a = alpha;
				resultPixels[(y * 2 + 0) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 1)].a = alpha;
				resultPixels[(y * 2 + 1) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 0)].a = alpha + 4;
				resultPixels[(y * 2 + 1) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 1)].a = alpha + 4;
				b = true;
			} else {
				resultPixels[(y * 2 + 0) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 0)] = SokuLib::Color::Transparent;
				resultPixels[(y * 2 + 0) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 1)] = SokuLib::Color::Transparent;
				resultPixels[(y * 2 + 1) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 0)] = SokuLib::Color::Transparent;
				resultPixels[(y * 2 + 1) * (resultRect.Pitch / sizeof(*resultPixels)) + (x * 2 + 1)] = SokuLib::Color::Transparent;
			}
		}
		if (b)
			alpha += 8;
	}


	ret = (*pphandleResult)->UnlockRect(0);
	if (FAILED(ret))
		fprintf(stderr, "(*pphandleResult)->UnlockRect(0) failed with code %li\n", ret);
	ret = (*pphandleOriginal)->UnlockRect(0);
	if (FAILED(ret))
		fprintf(stderr, "(*pphandleOriginal)->UnlockRect(0) failed with code %li\n", ret);


	(*fadeSprite(this))->texture.setHandle(resultId, (this->frameData->texSize * 2).to<unsigned>());
	(*fadeSprite(this))->rect = {
		0, 0,
		static_cast<int>((*fadeSprite(this))->texture.getSize().x),
		static_cast<int>((*fadeSprite(this))->texture.getSize().y),
	};
	(*fadeSprite(this))->setSize((this->frameData->texSize * 2).to<unsigned>());
	(*fadeSprite(this))->setPosition((this->position - this->frameData->offset).to<int>());
	return;

error:
	SokuLib::textureMgr.deallocate(resultId);
}
