//
// Created by PinkySmile on 09/07/24.
//

#ifndef CHARACTERENGINE_TACRABBIT_HPP
#define CHARACTERENGINE_TACRABBIT_HPP


#include "BasicObject.hpp"

class TaCRabbit : public BasicObject {
private:
	bool _canGrabHammer(const SokuLib::v2::GameObject *hammer) const;

public:
	TaCRabbit() = default;
	~TaCRabbit() override = default;

	void update() override;
	void initializeAction() override;
};


#endif //CHARACTERENGINE_TACRABBIT_HPP
