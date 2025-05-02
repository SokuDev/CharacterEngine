#ifndef CHARACTERENGINE_YUUKA_HPP
#define CHARACTERENGINE_YUUKA_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"


class GameObjectList_Yuuka : public SokuLib::v2::GameObjectList_Utsuho {
public:
	GameObjectList_Yuuka(SokuLib::v2::Player *player);

	void update() override;
	SokuLib::v2::GameObject *createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8) override;
};

class Yuuka : public SokuLib::v2::PlayerUtsuho {
public:
	Yuuka(SokuLib::PlayerInfo &info);
	~Yuuka() override = default;

	void update() override;
	void initializeAction() override;
	void initialize() override;
	void handleInputs() override;
	void VUnknown58() override;
	void VUnknown5C() override;
	bool VUnknown60(int a) override;

	static void hook();
	static void unhook();
};


#endif //CHARACTERENGINE_YUUKA_HPP
