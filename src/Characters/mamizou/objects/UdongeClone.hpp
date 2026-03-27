//
// Created by PinkySmile on 27/03/2026.
//

#ifndef CHARACTERENGINE_UDONGECLONE_HPP
#define CHARACTERENGINE_UDONGECLONE_HPP


#include "BasicObject.hpp"

class UdongeClone : public BasicObject {
public:
	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_UDONGECLONE_HPP
