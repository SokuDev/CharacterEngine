//
// Created by PinkySmile on 13/01/25.
//

#ifndef CHARACTERENGINE_TIMERGUI_HPP
#define CHARACTERENGINE_TIMERGUI_HPP


#include "BasicObject.hpp"

class TimerGui : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_TIMERGUI_HPP
