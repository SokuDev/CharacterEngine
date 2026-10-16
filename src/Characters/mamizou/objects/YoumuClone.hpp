//
// Created by PinkySmile on 25/03/2026.
//

#ifndef CHARACTERENGINE_YOUMUCLONE_HPP
#define CHARACTERENGINE_YOUMUCLONE_HPP


#include "BasicObject.hpp"

class YoumuClone : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_YOUMUCLONE_HPP