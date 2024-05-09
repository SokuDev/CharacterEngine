//
// Created by PinkySmile on 08/05/24.
//

#ifndef CHARACTERENGINE_TEWIOBJECT_HPP
#define CHARACTERENGINE_TEWIOBJECT_HPP


#include "BasicObject.hpp"
#include "Memory.hpp"

class TewiObject : public BasicObject {
public:
	TewiObject() = default;
	~TewiObject() override = default;

	void update() override;
	bool initializeAction() override;
};


#endif //CHARACTERENGINE_TEWIOBJECT_HPP
