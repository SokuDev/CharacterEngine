//
// Created by PinkySmile on 29/10/24.
//

#ifndef CHARACTERENGINE_IKU_HPP
#define CHARACTERENGINE_IKU_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Iku : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_IKU_HPP
