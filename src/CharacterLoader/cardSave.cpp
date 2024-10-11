//
// Created by PinkySmile on 08/10/24.
//

#include <SokuLib.hpp>
#include "Memory.hpp"
#include "cardSave.hpp"

std::vector<SokuLib::Map<int, int>> characterCards;
static bool init = false;

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

		sprintf(buffer, "data/csv/%s/spellcard.csv", SokuLib::getCharName(c));
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
						"Fatal error: Cannot load cards list for " + std::string(SokuLib::getCharName(c)) + ":\n" +
						"In file " + buffer + ": Cannot parse cell #1 at line #" + std::to_string(i) +
						" \"" + str + "\": " + e.what()
					).c_str(),
					"Loading spellcard.csv failed",
					MB_ICONERROR
				);
				abort();
			}
		} while (parser.goToNextLine());
	}
}

void *getCharacterCardObject()
{
	if (!init) {
		initCardArray();
		init = true;
	}
	return (void *)((unsigned)characterCards.data() - offsetof(SokuLib::v2::SaveDataManager, characterCards));
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