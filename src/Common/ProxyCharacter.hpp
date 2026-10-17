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

	void initializeAction() override
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

	bool handleGroundMovement() override
	{
		log("handleGroundMovement()\n");
		return T::handleGroundMovement();
	}

	bool setScenarioAction(int a) override
	{
		log("setScenarioAction(%i)\n", a);
		return T::setScenarioAction(a);
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

	void computerInputs() override
	{
		log("computerInputs()\n");
		T::computerInputs();
	}

	void updateStory() override
	{
		log("updateStory()\n");
		T::updateStory();
	}

	bool setCustomScenarioAction(int a) override
	{
		log("setCustomScenarioAction(%i)\n", a);
		return T::setCustomScenarioAction();
	}
};


#endif //CHARACTERENGINE_PROXYCHARACTER_HPP
