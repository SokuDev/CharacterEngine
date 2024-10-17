//
// Created by PinkySmile on 15/10/24.
//

#ifndef CHARACTERENGINE_YUKARI_HPP
#define CHARACTERENGINE_YUKARI_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Yukari : public BasicObject {
	public:
		void onRenderEnd() override;
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_YUKARI_HPP
