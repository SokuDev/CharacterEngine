//
// Created by PinkySmile on 31/10/2020
//

#include <vector>
#include <fstream>
#include <filesystem>
#include <shlwapi.h>
#include "CharacterModule.hpp"
#include "Memory.hpp"
#include "log.hpp"
#include "cardSave.hpp"
#include "checkSoku2.hpp"
#include "scenes.hpp"
#include "giuroll.hpp"

static unsigned tmp;
static unsigned nbIndices;
static std::vector<unsigned> characterSelectIndices;
static SokuLib::Dequeue<unsigned short> cards;
static std::vector<SokuLib::Sprite> extraSprites;
static wchar_t profilePath[MAX_PATH];
static std::map<void *, std::unique_ptr<CharacterModule>> loadedModules;
static void (*og_loadDat)(const char *path);

static std::vector<int> chrSelectNameTextures;
static std::vector<SokuLib::Sprite> chrSelectNameSprites;
static std::vector<int> chrSelectPortraitTextures;
static std::vector<SokuLib::Sprite> chrSelectPortraitSprites;


std::vector<CharacterModule> modules;


void loadExtraDatFiles(const char *path)
{
	og_loadDat(path);
	for (auto &module : modules)
		SokuLib::v2::loadDatFile((module.getFolder() / module.getData()).string().c_str());
}

static SokuLib::v2::Player *createCustomCharacter(int id, SokuLib::PlayerInfo &info)
{
	CharacterModule *module = nullptr;

	if (id < 0)
		module = &modules[-id - 1];
	else for (auto &i : modules)
		if (i.getIndex() == id) {
			 module = &i;
			 break;
		}
	if (!module)
		return nullptr;

	module = new CharacterModule(*module);
	while (true) {
		try {
			module->load();
		} catch (std::exception &e) {
			if (MessageBoxA(nullptr, ("Failed to create character: " + std::string(e.what())).c_str(), "CharacterEngine error", MB_RETRYCANCEL) == IDCANCEL)
				return nullptr;
			continue;
		}
		try {
			auto chr = module->build(info);

			loadedModules[chr].reset(module);
			module->registerGR();
			return chr;
		} catch (std::exception &e) {
			delete module;
			if (MessageBoxA(nullptr, ("Failed to create character: " + std::string(e.what())).c_str(), "CharacterEngine error", MB_RETRYCANCEL) == IDCANCEL)
				return nullptr;
		}
	}
}

static const unsigned createCustomCharacter_hook_ret = 0x46DE25;
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

static const char *__fastcall getCharacterName(int index)
{
	for (const auto &module : modules)
		if (module.getIndex() == index)
			return module.getInternalName().c_str();
	MessageBoxA(SokuLib::window, ("Fatal error in getCharacterName: Cannot find character index " + std::to_string(index)).c_str(), "Fatal Error", MB_ICONERROR);
	return nullptr;
}

static SokuLib::Sprite *__fastcall getCharacterSprite(int index)
{
	for (size_t i = 0; i < modules.size(); i++)
		if (modules[i].getIndex() == index)
			return &extraSprites[i];
	MessageBoxA(SokuLib::window, ("Fatal error in getCharacterSprite: Cannot find character index " + std::to_string(index)).c_str(), "Fatal Error", MB_ICONERROR);
	return nullptr;
}

static bool initSpritesDone = false;
static void initSprites()
{
	if (initSpritesDone)
		return;

	initSpritesDone = true;
	extraSprites.resize(modules.size());
	for (size_t i = 0; i < modules.size(); i++) {
		SokuLib::DrawUtils::Texture t;

		if (t.loadFromGame(("data/scene/select/character/01b_name/name_" + std::to_string(modules[i].getIndex()) + ".bmp").c_str()))
			extraSprites[i].init(t.releaseHandle(), 0, 0, t.getSize().x, t.getSize().y);
	}
}

static const unsigned chrSelectPortrait_hook_ret1 = 0x4212B3;
static const unsigned chrSelectPortrait_hook_ret2 = 0x4212A3;
static const unsigned chrSelectPortrait_hook_ret3 = 0x4212CC;
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
		MOV ECX, EDX
		PUSH EAX
		CALL getCharacterSprite
		MOV ECX, EAX
		POP EAX
		JMP [chrSelectPortrait_hook_ret3]
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
		JMP getCharacterName
	}
}

void loadCharacterModules()
{
	std::filesystem::path folderPath = profilePath;
	unsigned baseOrder[] = {
		SokuLib::CHARACTER_SUIKA,
		SokuLib::CHARACTER_YUKARI,
		SokuLib::CHARACTER_YUYUKO,
		SokuLib::CHARACTER_REMILIA,
		SokuLib::CHARACTER_YOUMU,
		SokuLib::CHARACTER_PATCHOULI,
		SokuLib::CHARACTER_ALICE,
		SokuLib::CHARACTER_SAKUYA,
		SokuLib::CHARACTER_MARISA,
		SokuLib::CHARACTER_REIMU,
		SokuLib::CHARACTER_SANAE,
		SokuLib::CHARACTER_CIRNO,
		SokuLib::CHARACTER_MEILING,
		SokuLib::CHARACTER_UTSUHO,
		SokuLib::CHARACTER_SUWAKO,
		SokuLib::CHARACTER_REISEN,
		SokuLib::CHARACTER_AYA,
		SokuLib::CHARACTER_KOMACHI,
		SokuLib::CHARACTER_IKU,
		SokuLib::CHARACTER_TENSHI,
	};

	folderPath /= L"characters";
	for (auto &dir : std::filesystem::directory_iterator(folderPath)) {
		auto path = dir.path() / "character.json";
		std::ifstream stream{path};

		if (!stream)
			continue;
		try {
			auto json = nlohmann::json::parse(stream);

			if (!json.contains("enabled") || json["enabled"])
				modules.emplace_back(json, dir.path());
		} catch (std::exception &e) {
			throw std::runtime_error("Failed loading " + path.string() + ": " + e.what());
		}
	}
	std::sort(modules.begin(), modules.end(), [](const CharacterModule &module1, const CharacterModule &module2) -> bool{
		return module1.getIndex() < module2.getIndex();
	});
	GiuRoll::load();
	if (!modules.empty())
		GiuRoll::setCharDataSize(modules.back().getIndex() + 1);

	characterSelectIndices.reserve(modules.size() + 20);
	for (size_t i = 0; i < std::ceil(modules.size() / 2.f); i++)
		characterSelectIndices.push_back(modules[i * 2].getIndex());
	characterSelectIndices.insert(characterSelectIndices.end(), std::begin(baseOrder), std::end(baseOrder));
	for (size_t i = 0; i < modules.size() / 2; i++)
		characterSelectIndices.push_back(modules[i * 2 + 1].getIndex());
	nbIndices = characterSelectIndices.size();
}

void __fastcall onCharacterDeleted(void *This)
{
	auto it = loadedModules.find(This);

	if (it != loadedModules.end())
		loadedModules.erase(it);
}

void generateSoku2Files()
{
	std::filesystem::path folder = profilePath;
	auto luaPathTemplate = folder / "config" / "SOKU2_base.lua";
	auto luaPath = folder / "config" / "SOKU2.lua";
	auto chrInfoPathTemplate = folder / "config" / "info" / "characters_base.csv";
	auto chrInfoPath = folder / "config" / "info" / "characters.csv";
	std::string line;
	std::ifstream chrInfoPathTemplateStream{chrInfoPathTemplate};

	if (!chrInfoPathTemplateStream) {
		MessageBoxA(nullptr, ("Failed to open " + chrInfoPathTemplate.string() + " for reading: " + strerror(errno)).c_str(), "Init failed", MB_ICONERROR);
		return;
	}

	std::ifstream luaPathTemplateStream{luaPathTemplate};

	if (!luaPathTemplateStream) {
		MessageBoxA(nullptr, ("Failed to open " + luaPathTemplate.string() + " for reading: " + strerror(errno)).c_str(), "Init failed", MB_ICONERROR);
		return;
	}

	std::ofstream chrInfoPathStream{chrInfoPath};

	while (std::getline(chrInfoPathTemplateStream, line))
		if (!line.empty())
			chrInfoPathStream << line << std::endl;
	for (auto &chr : modules) {
		auto inputs = chr.getSkillInputs();

		chrInfoPathStream << chr.getIndex() << ';';
		chrInfoPathStream << chr.getInternalName() << ';';
		chrInfoPathStream << chr.getShortName() << ';';
		chrInfoPathStream << chr.getFullName() << ';';
		for (size_t i = 0; i < inputs.size(); i++) {
			if (i != 0)
				chrInfoPathStream << ',';
			chrInfoPathStream << inputs[i];
		}
		chrInfoPathStream << std::endl;
	}

	std::ofstream luaPathStream{luaPath};

	while (std::getline(luaPathTemplateStream, line)) {
		if (line.ends_with("{{EXTRA_CHARACTERS}}")) {
			for (size_t i = 0; i < modules.size(); i++) {
				auto &chr = modules[i];

				luaPathStream << chr.getShortName();
				for (size_t j = chr.getShortName().size(); j < 10; j++)
					luaPathStream << ' ';
				luaPathStream << "= add_character(";
				luaPathStream << chr.getIndex();
				luaPathStream << ", ";

				luaPathStream << '"';
				luaPathStream << chr.getInternalName();
				luaPathStream << "\",";
				for (size_t j = chr.getInternalName().size(); j < 11; j++)
					luaPathStream << ' ';

				luaPathStream << '-';
				luaPathStream << i + 1;
				luaPathStream << ',';
				for (size_t j = log10(i + 1); j < 8; j++)
					luaPathStream << ' ';

				luaPathStream << "\"";
				luaPathStream << chr.getData();
				luaPathStream << "\",";
				for (size_t j = chr.getData().size(); j < 15; j++)
					luaPathStream << ' ';
				luaPathStream << R"("dummy.lua",      "dummy.asm",      "deck.cfg"))" << std::endl;
			}
		} else if (line.ends_with("{{EXTRA_CHARACTERS_DEFINITION}}")) {
			for (auto &module : modules)
				luaPathStream << "  " << module.getShortName() << ',' << std::endl;
		} else if (line.ends_with("{{EXTRA_CHARACTERS_AFTER}}")) {
			for (size_t i = 1; i < modules.size(); i += 2)
				luaPathStream << "  " << modules[i].getShortName() << ',' << std::endl;
		} else if (line.ends_with("{{EXTRA_CHARACTERS_BEFORE}}")) {
			for (size_t i = modules.size() + modules.size() % 2; i > 0; i -= 2)
				luaPathStream << "  " << modules[i - 2].getShortName() << ',' << std::endl;
		} else
			luaPathStream << line << std::endl;
	}
}

void resetChrSelect()
{
	for (auto t : chrSelectNameTextures)
		SokuLib::textureMgr.remove(t);
	for (auto t : chrSelectPortraitTextures)
		SokuLib::textureMgr.remove(t);
	chrSelectNameTextures.clear();
	chrSelectNameSprites.clear();
	chrSelectPortraitTextures.clear();
	chrSelectPortraitSprites.clear();

	chrSelectNameTextures.reserve(20 + modules.size());
	chrSelectNameSprites.reserve(20 + modules.size());
	chrSelectPortraitTextures.reserve(20 + modules.size());
	chrSelectPortraitSprites.reserve(20 + modules.size());
}

void __declspec(naked) setIndicesArraySize()
{
	__asm {
		CALL resetChrSelect
		MOV EAX, [nbIndices]
		MOV ECX, 21
		RET
	}
}

void __fastcall addNameTexture(int tex)
{
	chrSelectNameTextures.push_back(tex);
}

void __fastcall addNameSprite(int w, int h)
{
	chrSelectNameSprites.emplace_back().setTexture2(chrSelectNameTextures.back(), 0, 0, w, h);
}

const unsigned __name_retValue = 0x4250A0;
void __declspec(naked) loadNameSprites_hook()
{
	__asm {
		CALL addNameTexture
		MOV ECX, [ESP + 0x34]
		MOV EDX, [ESP + 0x30]
		CALL addNameSprite
		JMP [__name_retValue]
	}
}

void __fastcall addPortraitTexture(int tex)
{
	chrSelectPortraitTextures.push_back(tex);
}

void __fastcall addPortraitSprite(int w, int h)
{
	chrSelectPortraitSprites.emplace_back().setTexture2(chrSelectPortraitTextures.back(), 0, 0, w, h);
}

const unsigned __portrait_retValue = 0x425183;
void __declspec(naked) loadPortraitSprites_hook()
{
	__asm {
		MOV ECX, EAX
		CALL addPortraitTexture
		MOV ECX, [ESP + 0x34]
		MOV EDX, [ESP + 0x30]
		CALL addPortraitSprite
		JMP [__portrait_retValue]
	}
}

int getPortraitSize()
{
	int w;
	int h;

	SokuLib::textureMgr.getSize(chrSelectPortraitTextures.front(), &w, &h);
	if (w * modules.size() < 640)
		return w;
	return 640 / modules.size();
}

void __declspec(naked) getPortraitSize_hook()
{
	__asm {
		CALL getPortraitSize
		MOV [tmp], EAX
		FILD dword ptr [tmp]
		FST dword ptr [ESP + 0x1C]
		RET
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
	wchar_t check[1024];

#ifdef _DEBUG
	FILE *_;

	AllocConsole();
	freopen_s(&_, "CONOUT$", "w", stdout);
	freopen_s(&_, "CONOUT$", "w", stderr);
#endif

	GetModuleFileNameW(hMyModule, profilePath, 1024);
	wcscpy(check, profilePath);
	PathRemoveFileSpecW(profilePath);
	for (int i = 0; check[i]; i++)
		check[i] = tolower(check[i]);
	checkSoku2();

	auto ptr = wcsrchr(check, '\\');

	if (!ptr)
		ptr = check;
	else
		ptr++;
	// If we are called Soku2, then it means we don't actually have Soku2
	if (wcscmp(ptr, L"soku2.dll") == 0) {
		hasSoku2 = false;
		puts("But we are Soku2...");
	}
	while (true)
		try {
			loadCharacterModules();
			break;
		} catch (std::exception &e) {
			auto button = MessageBoxA(nullptr, ("Cannot load character modules: " + std::string(e.what())).c_str(), "CharacterEngine error", MB_ABORTRETRYIGNORE);

			if (button == IDABORT)
				return false;
			else if (button == IDIGNORE)
				break;
		}
	generateSoku2Files();

	puts("Hello, world!");
	VirtualProtect((PVOID)RDATA_SECTION_OFFSET, RDATA_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	if (!hasSoku2) {
		og_SelectOnProcess = SokuLib::TamperDword(&SokuLib::VTable_Select.onProcess, SelectOnProcess);
		og_SelectCLOnProcess = SokuLib::TamperDword(&SokuLib::VTable_SelectClient.onProcess, SelectCLOnProcess);
		og_SelectSVOnProcess = SokuLib::TamperDword(&SokuLib::VTable_SelectServer.onProcess, SelectSVOnProcess);
		og_ProfileChrSelect = SokuLib::TamperDword(0x859878, ProfileCharacterSelect_Update);
	}
	VirtualProtect((PVOID)RDATA_SECTION_OFFSET, RDATA_SECTION_SIZE, old, &old);

	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	SokuLib::TamperNearJmpOpr(0x46DA71, createCustomCharacter_hook);
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

	// Detect character deletion
	std::vector<unsigned char> extra = { 0x59 };
	new SokuLib::Trampoline(0x46E687, (void (*)())onCharacterDeleted, 8, &extra, true, true);
	*(char *)0x46E687 = 0x51;

	if (!hasSoku2) {
		SokuLib::TamperNearJmp(0x43F3F0, getCreateCustomCharacterName_hook);
		SokuLib::TamperNearJmp(0x42129E, chrSelectPortrait_hook);

		memset((void *)0x4356B5, 0x90, 16);
		memset((void *)0x4356CB, 0x90, 16);
		*(char *)0x4356DB = 0x5F;
		SokuLib::TamperNearCall(0x4356B5, selectDeckSlot_hook);
		SokuLib::TamperNearCall(0x4356CB, selectDeckSlot_hook);
		//*(char *)0x46DA6F = 0x12;

		// Add extra characters to save data manager
		SokuLib::TamperNearJmpOpr(0x44E102, getCharacterCardObject);
		SokuLib::TamperNearJmpOpr(0x44E11D, getCharacterCardObject);

		/*
		SokuLib::TamperNearCall(0x4248D1, setIndicesArraySize);
		*(char *)0x4248DB = 0x4C;
		*(unsigned **)0x4248E2 = characterSelectIndices.data();
		// Skip check for character enabled
		*(char *)0x425002 = 0xEB;

		memset((void *)0x425068, 0x90, 0x38);
		SokuLib::TamperNearJmp(0x425068, loadNameSprites_hook);

		memset((void *)0x425161, 0x90, 0x22);
		SokuLib::TamperNearJmp(0x425161, loadPortraitSprites_hook);

		memset((void *)0x42526E, 0x90, 0x2D);
		SokuLib::TamperNearCall(0x42526E, getPortraitSize_hook);
		*/
		// Filesystem first patch
		/*
		*(char *)0x40D1FB = 0xEB;
		*(char *)0x40D27A = 0x74;
		*(char *)0x40D27B = 0x91;
		*(char *)0x40D245 = 0x1C;
		memset((char *)0x40D27C, 0x90, 7);
		 */

		og_loadDat = SokuLib::TamperNearJmpOpr(0x7fb85f, loadExtraDatFiles);
	}
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
	return 500;
}

// Not yet implemented in the mod loader, subject to change
// SokuModLoader::IValue **getConfig();
// void freeConfig(SokuModLoader::IValue **v);
// bool commitConfig(SokuModLoader::IValue *);
// const char *getFailureReason();
// bool hasChainedHooks();
// void unHook();