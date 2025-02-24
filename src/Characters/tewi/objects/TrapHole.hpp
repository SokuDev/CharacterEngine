//
// Created by PinkySmile on 12/07/24.
//

#ifndef CHARACTERENGINE_TRAPHOLE_HPP
#define CHARACTERENGINE_TRAPHOLE_HPP


#include "BasicObject.hpp"

class TrapHole : public BasicObject {
public:
	~TrapHole();
	void render() override;
	void update() override;
	void initializeAction() override;

	void prepareFade();
};


#endif //CHARACTERENGINE_TRAPHOLE_HPP
