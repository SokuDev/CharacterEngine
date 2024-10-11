//
// Created by PinkySmile on 09/10/24.
//

#include "scenes.hpp"

int (SokuLib::Select::*og_SelectOnProcess)();
int (SokuLib::SelectClient::*og_SelectCLOnProcess)();
int (SokuLib::SelectServer::*og_SelectSVOnProcess)();
int (__fastcall *og_ProfileChrSelect)(void *This);

void selectCommon(SokuLib::Select *This)
{
	if (This->leftKeys) {
		if (This->leftKeys->input.spellcard == 1 || (This->leftKeys->input.a == 1 && This->leftKeys->input.verticalAxis > 0))
			SokuLib::leftChar = SokuLib::CHARACTER_TEWI;
	}
	if (This->rightKeys) {
		if (This->rightKeys->input.spellcard == 1 || (This->rightKeys->input.a == 1 && This->rightKeys->input.verticalAxis > 0))
			SokuLib::rightChar = SokuLib::CHARACTER_TEWI;
	}
}

int __fastcall SelectOnProcess(SokuLib::Select *This)
{
	int ret = (This->*og_SelectOnProcess)();

	selectCommon(This);
	return ret;
}

int __fastcall SelectCLOnProcess(SokuLib::SelectClient *This)
{
	int ret = (This->*og_SelectCLOnProcess)();

	selectCommon(&This->base);
	return ret;
}

int __fastcall SelectSVOnProcess(SokuLib::SelectServer *This)
{
	int ret = (This->*og_SelectSVOnProcess)();

	selectCommon(&This->base);
	return ret;
}

int __fastcall ProfileCharacterSelect_Update(void *This)
{
	if (og_ProfileChrSelect(This) == 0)
		return 0;
	if (SokuLib::inputMgrs.input.spellcard != 1)
		return 1;

	int ret, w, h;
	auto sprite = (SokuLib::Sprite *)((unsigned)This + 8);
	auto profile = *(SokuLib::Profile **)((unsigned)This + 0x9C);

	if (SokuLib::textureMgr.loadTexture(&ret, "data/stand/tewi.bmp", &w, &h)) {
		sprite->init(ret, 0, 0, w, h);
		for (auto &vertice: sprite->vertices)
			vertice.color = SokuLib::Color{0x80, 0x80, 0x80, 0xFF};
	}

	SokuLib::playSEWaveBuffer(0x28);
	SokuLib::activateMenu(((SokuLib::ProfileDeckEdit *(__thiscall *)(void *, SokuLib::Profile *, SokuLib::Character, SokuLib::Sprite *))(*(unsigned *)0x44D52A + 0x44D52E))(
		SokuLib::NewFct(0x840),
			profile,
			SokuLib::CHARACTER_TEWI,
			sprite
	));
	return 1;
}