//
// Created by PinkySmile on 31/10/2020
//

#include <SokuLib.hpp>
#include "Memory.hpp"
#include "tewi/Tewi.hpp"
#include "log.hpp"
#include "BasicObject.hpp"

char *(*getCharName)(unsigned) = (char *(*)(unsigned))0x43F3F0;
static int (__fastcall *og_ProfileChrSelect)(void *This);
static int (SokuLib::Select::*og_SelectOnProcess)();
static int (SokuLib::SelectClient::*og_SelectCLOnProcess)();
static int (SokuLib::SelectServer::*og_SelectSVOnProcess)();

static SokuLib::Dequeue<unsigned short> cards;
static const char *names[] = {
	"momiji",
	"clownpiece",
	"flandre",
	"orin",
	"yuuka",
	"kaguya",
	"mokou",
	"mima",
	"shou",
	"murasa",
	"sekibanki",
	"satori",
	"ran",
	"tewi"
};
static SokuLib::Sprite extraSprites[sizeof(names) / sizeof(*names)];
bool hooked = false;
bool init = false;
FILE *file;
std::vector<SokuLib::Map<int, int>> characterCards;

static const unsigned createCustomCharacter_hook_ret = 0x46DE25;

static void initCardArray()
{
	unsigned list[] = {
		SokuLib::CHARACTER_REIMU,
		SokuLib::CHARACTER_MARISA,
		SokuLib::CHARACTER_SAKUYA,
		SokuLib::CHARACTER_ALICE,
		SokuLib::CHARACTER_PATCHOULI,
		SokuLib::CHARACTER_YOUMU,
		SokuLib::CHARACTER_REMILIA,
		SokuLib::CHARACTER_YUYUKO,
		SokuLib::CHARACTER_YUKARI,
		SokuLib::CHARACTER_SUIKA,
		SokuLib::CHARACTER_REISEN,
		SokuLib::CHARACTER_AYA,
		SokuLib::CHARACTER_KOMACHI,
		SokuLib::CHARACTER_IKU,
		SokuLib::CHARACTER_TENSHI,
		SokuLib::CHARACTER_SANAE,
		SokuLib::CHARACTER_CIRNO,
		SokuLib::CHARACTER_MEILING,
		SokuLib::CHARACTER_UTSUHO,
		SokuLib::CHARACTER_SUWAKO,
		SokuLib::CHARACTER_TEWI
	};
	char buffer[] = "data/csv/000000000000000000/spellcard.csv";

	characterCards.clear();
	characterCards.resize(SokuLib::CHARACTER_TEWI + 1);
	for (auto c : list) {
		if (c <= SokuLib::CHARACTER_SUWAKO && !SokuLib::v2::SaveDataManager::instance.enabledCharacters[c])
			continue;

		sprintf(buffer, "data/csv/%s/spellcard.csv", getCharName(c));
		printf("Loading cards from %s\n", buffer);

		SokuLib::CSVParser parser{buffer};
		std::vector<unsigned short> valid;
		int i = 0;

		do {
			auto str = parser.getNextCell();

			i++;
			try {
				characterCards[c][std::stoul(str)] = 100;
			} catch (std::exception &e) {
				MessageBoxA(
					SokuLib::window,
					(
						"Fatal error: Cannot load cards list for " + std::string(getCharName(c)) + ":\n" +
						"In file " + buffer + ": Cannot parse cell #1 at line #" + std::to_string(i) +
						" \"" + str + "\": " + e.what()
					).c_str(),
					"Loading default deck failed",
					MB_ICONERROR
				);
				abort();
			}
		} while (parser.goToNextLine());
	}
}

static void *getCharacterCardObject()
{
	if (!init) {
		initCardArray();
		init = true;
	}
	return (void *)((unsigned)characterCards.data() - offsetof(SokuLib::v2::SaveDataManager, characterCards));
}

static SokuLib::v2::Player *createCustomCharacter(int id, SokuLib::PlayerInfo &info)
{
	if (id == SokuLib::CHARACTER_TEWI) {
		if (!hooked)
			Tewi::hook();
		hooked = true;
		return new Tewi(info);
	}
	return nullptr;
}

static void __declspec(naked) createCustomCharacter_hook()
{
	__asm {
		PUSH ECX
		PUSH EDX
		PUSH EDI
		PUSH EAX
		CALL createCustomCharacter
		ADD ESP, 8
		POP EDX
		POP ECX
		JMP [createCustomCharacter_hook_ret]
	}
}

static SokuLib::Dequeue<unsigned short> *selectDeckSlot(SokuLib::Profile *profile, int chr, char subdeck)
{
	if (chr < 20)
		return &profile->cards[chr][subdeck];
	if (!cards.data) {
		//for (int i = 0; i < 20; i++)
		//	cards.push_back(i);
		short card[] = {0,0,1,100,100,101,101,102,102,103,103,200,200,201,201,208,208,210,210,210};

		for (short c : card)
			cards.push_back(c);
	}
	return &cards;
}

static void __declspec(naked) selectDeckSlot_hook()
{
	__asm {
		PUSH ECX
		PUSH EBX
		PUSH EDI
		PUSH ESI
		CALL selectDeckSlot
		ADD ESP, 12
		POP ECX
		RET
	}
}

static const unsigned getCreateCustomCharacterName_hook_ret = 0x43F3F8;

static void __declspec(naked) getCreateCustomCharacterName_hook()
{
	__asm {
		MOV ECX, [ESP + 0x4]
		CMP ECX, 22
		JGE custom

		SUB ESP, 8
		PUSH ESI
		LEA EAX, [ESP + 0x10]
		JMP [getCreateCustomCharacterName_hook_ret]

	custom:
		SUB ECX, 22
		MOV EAX, [names + ECX * 4]
		RET
	}
}

static const unsigned chrSelectPortrait_hook_ret1 = 0x4212B3;
static const unsigned chrSelectPortrait_hook_ret2 = 0x4212A3;
static const unsigned chrSelectPortrait_hook_ret3 = 0x4212CC;
static bool initSpritesDone = false;

static void initSprites()
{
	if (initSpritesDone)
		return;

	int i = 0;

	initSpritesDone = true;
	for (auto &sprite : extraSprites) {
		SokuLib::DrawUtils::Texture t;

		if (t.loadFromGame(("data/scene/select/character/01b_name/name_" + std::to_string(i + 22) + ".bmp").c_str()))
			sprite.init(t.releaseHandle(), 0, 0, t.getSize().x, t.getSize().y);
		i++;
	}
}

static void __declspec(naked) chrSelectPortrait_hook()
{
	__asm {
		FADD dword ptr [EAX + 0xc]
		PUSH EAX
		PUSH ECX
		CALL initSprites
		POP ECX
		POP EAX
		MOV EDX, [ECX + EBP + 0x8]
		CMP EDX, 22
		JGE ret3

		CMP byte ptr [ESI + ECX + 0x4c], 0x0
		JNZ ret2
		JMP [chrSelectPortrait_hook_ret1]
	ret2:
		JMP [chrSelectPortrait_hook_ret2]
	ret3:
		SUB EDX, 22
		IMUL EDX, EDX, 0x94
		LEA ECX, [extraSprites + EDX]
		JMP [chrSelectPortrait_hook_ret3]
	}
}

void selectCommon(SokuLib::Select *This)
{
	if (hooked) {
		hooked = false;
		Tewi::unhook();
	}
	if (This->leftKeys && This->leftKeys->input.spellcard == 1)
		SokuLib::leftChar = SokuLib::CHARACTER_TEWI;
	if (This->rightKeys && This->rightKeys->input.spellcard == 1)
		SokuLib::rightChar = SokuLib::CHARACTER_TEWI;
}

static int __fastcall SelectOnProcess(SokuLib::Select *This)
{
	int ret = (This->*og_SelectOnProcess)();

	selectCommon(This);
	return ret;
}

static int __fastcall SelectCLOnProcess(SokuLib::SelectClient *This)
{
	int ret = (This->*og_SelectCLOnProcess)();

	selectCommon(&This->base);
	return ret;
}

static int __fastcall SelectSVOnProcess(SokuLib::SelectServer *This)
{
	int ret = (This->*og_SelectSVOnProcess)();

	selectCommon(&This->base);
	return ret;
}

static int __fastcall ProfileCharacterSelect_Update(void *This)
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

void __declspec(naked) gameEndGrantCards()
{
	__asm {
		POP ECX
		MOV EAX, [ESP + 8]
		CMP EAX, 20
		JL cont

		MOV AL, 1
		RET 0x8

	cont:
		PUSH 0x84E0B8
		JMP ECX
	}
}


// We check if the game version is what we target (in our case, Soku 1.10a).
extern "C" __declspec(dllexport) bool CheckVersion(const BYTE hash[16])
{
	return memcmp(hash, SokuLib::targetHash, sizeof(SokuLib::targetHash)) == 0;
}

// Called when the mod loader is ready to initialize this module.
// All hooks should be placed here. It's also a good moment to load settings from the ini.
extern "C" __declspec(dllexport) bool Initialize(HMODULE hMyModule, HMODULE hParentModule)
{
	DWORD old;

#ifdef _DEBUG
	FILE *_;

	AllocConsole();
	freopen_s(&_, "CONOUT$", "w", stdout);
	freopen_s(&_, "CONOUT$", "w", stderr);
#endif
	file = fopen("character_log.log", "a+");

	auto gr = LoadLibraryA("giuroll");

	if (gr) {
		auto fct1 = GetProcAddress(gr, "set_char_data_size");
		auto fct2 = GetProcAddress(gr, "set_char_data_pos");

		if (!fct1 || !fct2) {
			MessageBoxA(
				nullptr,
				"Netplay rollback not supported. This mod supports giuroll 0.6.12+, which wasn't found.\n"
				"A different (and not supported) giuroll version is in use. Please use version 0.6.12 or greater, or otherwise one that is compatible.\n"
				"Playing online now will cause desyncs",
				"CustomWeathers",
				MB_ICONWARNING | MB_OK
			);
		} else {
			reinterpret_cast<void (*)(size_t)>(fct1)(SokuLib::CHARACTER_TEWI + 1);
			reinterpret_cast<void (*)(size_t, size_t, size_t)>(fct2)(SokuLib::CHARACTER_TEWI, sizeof(Tewi), sizeof(BasicObject));
		}
	}

	puts("Hello, world!");
	VirtualProtect((PVOID)RDATA_SECTION_OFFSET, RDATA_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	og_SelectOnProcess = SokuLib::TamperDword(&SokuLib::VTable_Select.onProcess, SelectOnProcess);
	og_SelectCLOnProcess = SokuLib::TamperDword(&SokuLib::VTable_SelectClient.onProcess, SelectCLOnProcess);
	og_SelectSVOnProcess = SokuLib::TamperDword(&SokuLib::VTable_SelectServer.onProcess, SelectSVOnProcess);
	og_ProfileChrSelect = SokuLib::TamperDword(0x859878, ProfileCharacterSelect_Update);
	VirtualProtect((PVOID)RDATA_SECTION_OFFSET, RDATA_SECTION_SIZE, old, &old);

	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	SokuLib::TamperNearJmp(0x43F3F0, getCreateCustomCharacterName_hook);
	SokuLib::TamperNearJmpOpr(0x46DA71, createCustomCharacter_hook);
	SokuLib::TamperNearJmp(0x42129E, chrSelectPortrait_hook);
	memset((void *)0x4356B5, 0x90, 16);
	memset((void *)0x4356CB, 0x90, 16);
	*(char *)0x4356DB = 0x5F;
	SokuLib::TamperNearCall(0x4356B5, selectDeckSlot_hook);
	SokuLib::TamperNearCall(0x4356CB, selectDeckSlot_hook);
	//*(char *)0x46DA6F = 0x12;

	// FIXME: Result screen hack
	*(char *)0x4497CB = 0xC6;
	*(char *)0x4497CC = 0x46;
	*(char *)0x4497CD = 0x04;
	*(char *)0x4497CE = 0x00;
	*(char *)0x4497CF = 0xEB;
	*(char *)0x4497D0 = 0x05;
	*(char *)0x482039 = 0xE9;
	*(char *)0x48203A = 0xA7;
	*(char *)0x48203B = 0x00;
	*(char *)0x48203C = 0x00;
	*(char *)0x48203D = 0x00;
	*(char *)0x48203E = 0x90;

	// Add extra characters to save data manager
	SokuLib::TamperNearJmpOpr(0x44E102, getCharacterCardObject);
	SokuLib::TamperNearJmpOpr(0x44E11D, getCharacterCardObject);

	// Filesystem first patch
	*(char *)0x40D1FB = 0xEB;
	*(char *)0x40D27A = 0x74;
	*(char *)0x40D27B = 0x91;
	*(char *)0x40D245 = 0x1C;
	memset((char *)0x40D27C, 0x90, 7);
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);

	FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
	return true;
}

extern "C" int APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	return TRUE;
}

// New mod loader functions
// Loading priority. Mods are loaded in order by ascending level of priority (the highest first).
// When 2 mods define the same loading priority the loading order is undefined.
extern "C" __declspec(dllexport) int getPriority()
{
	return 0;
}

// Not yet implemented in the mod loader, subject to change
// SokuModLoader::IValue **getConfig();
// void freeConfig(SokuModLoader::IValue **v);
// bool commitConfig(SokuModLoader::IValue *);
// const char *getFailureReason();
// bool hasChainedHooks();
// void unHook();