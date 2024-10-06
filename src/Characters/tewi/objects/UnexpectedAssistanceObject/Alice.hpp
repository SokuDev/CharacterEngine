//
// Created by PinkySmile on 20/07/24.
//

#ifndef CHARACTERENGINE_ALICE_HPP
#define CHARACTERENGINE_ALICE_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Alice : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_ALICE_HPP
