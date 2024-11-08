//
// Created by PinkySmile on 03/11/24.
//

#ifndef CHARACTERENGINE_UTSUHO_HPP
#define CHARACTERENGINE_UTSUHO_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance
{
	class Utsuho : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};
}


#endif //CHARACTERENGINE_UTSUHO_HPP
