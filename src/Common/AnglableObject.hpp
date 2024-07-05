//
// Created by PinkySmile on 22/06/24.
//

#ifndef CHARACTERENGINE_ANGLABLEOBJECT_HPP
#define CHARACTERENGINE_ANGLABLEOBJECT_HPP


#include "BasicObject.hpp"

class AnglableObject : public BasicObject {
public:
	AnglableObject(std::pair<char, char> crystalsSpawn);

	void update() override;
	bool initializeAction() override;
};


#endif //CHARACTERENGINE_ANGLABLEOBJECT_HPP
