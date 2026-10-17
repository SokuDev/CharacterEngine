//
// Created by PinkySmile on 09/05/24.
//

#ifndef CHARACTERENGINE_BASICOBJECT_HPP
#define CHARACTERENGINE_BASICOBJECT_HPP


#include <SokuLib.hpp>
#include "Memory.hpp"

#if defined(OBJECT_MEMORY_SIZE) && OBJECT_MEMORY_SIZE > 4
#define FINAL_OBJECT_MEMORY_SIZE OBJECT_MEMORY_SIZE
#else
#define FINAL_OBJECT_MEMORY_SIZE 4
#endif

class BasicObject : public SokuLib::v2::GameObject {
protected:
	char data[FINAL_OBJECT_MEMORY_SIZE];

public:
	BasicObject() = default;
	~BasicObject() override = default;

	void update() override;
	void initializeAction() override;
	GameObject* createObject(short action, float x, float y, char direction, char layer, float *extraData, unsigned int extraDataSize) override;
	GameObject* createChild(short action, float x, float y, char direction, char layer, float *extraData, unsigned int extraDataSize) override;

	template<size_t size>
	SokuLib::v2::GameObject *createObject(short action, float x, float y, char direction, char layer, float (&extraData)[size]) {
		// Mimics 0x46EB30
		return this->createObject(action, x, y, direction, layer, extraData, size);
	}
	SokuLib::v2::GameObject *createObject(short action, float x, float y, char direction, char layer) {
		// Mimics 0x46EB30
		return this->createObject(action, x, y, direction, layer, nullptr, 0);
	}

	template<size_t size>
	SokuLib::v2::GameObject *createChild(short action, float x, float y, char direction, char layer, float (&extraData)[size]) {
		// Mimics 0x46EB30
		return this->createChild(action, x, y, direction, layer, extraData, size);
	}
	SokuLib::v2::GameObject *createChild(short action, float x, float y, char direction, char layer) {
		// Mimics 0x46EB30
		return this->createChild(action, x, y, direction, layer, nullptr, 0);
	}
};


#endif //CHARACTERENGINE_BASICOBJECT_HPP
