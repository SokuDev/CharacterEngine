//
// Created by PinkySmile on 27/10/24.
//

#ifndef CHARACTERENGINE_AYA_HPP
#define CHARACTERENGINE_AYA_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Aya : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_AYA_HPP
