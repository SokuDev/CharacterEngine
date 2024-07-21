//
// Created by PinkySmile on 18/07/24.
//

#ifndef CHARACTERENGINE_MARISA_HPP
#define CHARACTERENGINE_MARISA_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Marisa : public BasicObject {
	public:
		void update() override;
		bool initializeAction() override;
	};
}


#endif //CHARACTERENGINE_MARISA_HPP
