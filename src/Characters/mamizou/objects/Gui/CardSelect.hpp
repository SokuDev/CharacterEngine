//
// Created by PinkySmile on 12/10/2025.
//

#ifndef CHARACTERENGINE_CARDSELECT_HPP
#define CHARACTERENGINE_CARDSELECT_HPP


#include "BasicObject.hpp"

class CardSelect : public BasicObject {
public:
	void update() override;
	void initializeAction() override;

	void onRenderEnd() override;
};


#endif //CHARACTERENGINE_CARDSELECT_HPP
