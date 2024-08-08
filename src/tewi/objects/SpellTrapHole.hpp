//
// Created by PinkySmile on 02/08/24.
//

#ifndef CHARACTERENGINE_SPELLTRAPHOLE_HPP
#define CHARACTERENGINE_SPELLTRAPHOLE_HPP


#include "BasicObject.hpp"

class SpellTrapHole : public BasicObject {
public:
	~SpellTrapHole();
	void render() override;
	void update() override;
	void initializeAction() override;

	void prepareFade();
	void prepareTexture();
};


#endif //CHARACTERENGINE_SPELLTRAPHOLE_HPP
