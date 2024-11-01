//
// Created by PinkySmile on 31/10/24.
//

#ifndef CHARACTERENGINE_SANAE_HPP
#define CHARACTERENGINE_SANAE_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Sanae : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_SANAE_HPP
