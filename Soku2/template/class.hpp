#ifndef CHARACTERENGINE_{{CLASS_NAME}}_HPP
#define CHARACTERENGINE_{{CLASS_NAME}}_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"


class GameObjectList_{{ClassName}} : public SokuLib::v2::GameObjectList_{{BaseName}} {
public:
	GameObjectList_{{ClassName}}(SokuLib::v2::Player *player);

	void update() override;
	SokuLib::v2::GameObject *createObject(SokuLib::v2::GameObject *a0, SokuLib::v2::Player *a1, short a2, float a3, float a4, char a5, char a6, void* a7, unsigned int a8) override;
};

class {{ClassName}} : public SokuLib::v2::Player{{BaseName}} {
public:
	{{ClassName}}(SokuLib::PlayerInfo &info);
	~{{ClassName}}() override = default;

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


#endif //CHARACTERENGINE_{{CLASS_NAME}}_HPP
