//
// Created by PinkySmile on 21/07/24.
//

#ifndef CHARACTERENGINE_PATCHOULI_HPP
#define CHARACTERENGINE_PATCHOULI_HPP


#include "BasicObject.hpp"

namespace UnexpectedAssistance {

	class Patchouli : public BasicObject {
	public:
		void update() override;
		void initializeAction() override;
	};

}


#endif //CHARACTERENGINE_PATCHOULI_HPP
