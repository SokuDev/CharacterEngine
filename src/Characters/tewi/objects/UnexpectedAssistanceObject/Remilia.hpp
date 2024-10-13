//
// Created by PinkySmile on 13/10/24.
//

#ifndef CHARACTERENGINE_REMILIA_HPP
#define CHARACTERENGINE_REMILIA_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Remilia : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_REMILIA_HPP
