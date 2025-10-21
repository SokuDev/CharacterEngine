//
// Created by PinkySmile on 13/01/25.
//

#ifndef CHARACTERENGINE_TIMERSPELLGUI_HPP
#define CHARACTERENGINE_TIMERSPELLGUI_HPP


#include "BasicObject.hpp"

class TimerSpellGui : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_TIMERSPELLGUI_HPP
