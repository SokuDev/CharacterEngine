//
// Created by PinkySmile on 01/05/24.
//

#ifndef CHARACTERENGINE_PROXYCHARACTER_HPP
#define CHARACTERENGINE_PROXYCHARACTER_HPP


#include <SokuLib.hpp>
#include "log.hpp"

template<typename T, typename T2>
class ProxyCharacter : public T {
public:
	ProxyCharacter(SokuLib::PlayerInfo &info) : T(info)
	{
		log("ProxyCharacter()\n");
		this->objectList = new T2(this);
	}

	~ProxyCharacter() override
	{
		log("~ProxyCharacter()\n");
	}

	void setActionSequence(short i, short i1) override
	{
		log("setActionSequence(%i, %i)\n", i, i1);
		T::setActionSequence(i, i1);
	}

	bool setAction(short i) override
	{
		log("setAction(%i)\n", i);
		return T::setAction(i);
	}

	void setSequence(short i) override
	{
		log("setSequence(%i)\n", i);
		T::setSequence(i);
	}

	void resetSequence() override
	{
		log("resetSequence()\n");
		T::resetSequence();
	}

	bool nextSequence() override
	{
		log("nextSequence()\n");
		return T::nextSequence();
	}

	void prevSequence() override
	{
		log("prevSequence()\n");
		T::prevSequence();
	}

	void setPose(short i) override
	{
		log("setPose(%i)\n", i);
		T::setPose(i);
	}

	bool nextPose() override
	{
		log("nextPose()\n");
		return T::nextPose();
	}

	void prevPose() override
	{
		log("prevPose()\n");
		T::prevPose();
	}

	void update() override
	{
		log("update()\n");
		T::update();
	}

	bool initializeAction() override
	{
		log("initializeAction()\n");
		return T::initializeAction();
	}

	void render() override
	{
		log("render()\n");
		T::render();
	}

	void render2() override
	{
		log("render2()\n");
		T::render2();
	}

	void applyTransform() override
	{
		log("applyTransform()\n");
		T::applyTransform();
	}

	void onRenderEnd() override
	{
		log("onRenderEnd()\n");
		T::onRenderEnd();
	}

	void updatePhysics() override
	{
		log("updatePhysics()\n");
		T::updatePhysics();
	}

	void initialize() override
	{
		log("initialize()\n");
		T::initialize();
	}

	bool VUnknown48() override
	{
		log("VUnknown48()\n");
		return T::VUnknown48();
	}

	bool VUnknown4C(int a) override
	{
		log("VUnknown4C(%i)\n", a);
		return T::VUnknown4C(a);
	}

	void handleInputs() override
	{
		log("handleInputs()\n");
		T::handleInputs();
	}

	void checkAllMotionInputs() override
	{
		log("checkAllMotionInputs()\n");
		T::checkAllMotionInputs();
	}

	void VUnknown58() override
	{
		log("VUnknown58()\n");
		T::VUnknown58();
	}

	void VUnknown5C() override
	{
		log("VUnknown5C()\n");
		T::VUnknown5C();
	}

	bool VUnknown60(int a) override
	{
		log("VUnknown60(%i)\n", a);
		return T::VUnknown60();
	}
};


#endif //CHARACTERENGINE_PROXYCHARACTER_HPP
