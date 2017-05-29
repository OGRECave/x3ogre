/*
 * TimeSensor.cpp
 *
 *  Created on: 31.03.2014
 *      Author: parojtbe
 */

#include "TimeSensor.h"

#include <OgreAnimationState.h>

namespace {
class AccumulateControllerFunction: public Ogre::ControllerFunction<Ogre::Real> {
    Ogre::Real _timeInterval;
    Ogre::Real _time = 0;

public:
    AccumulateControllerFunction(Ogre::Real timeInterval) :
            ControllerFunction<Ogre::Real>(false), _timeInterval(timeInterval) {
    }

    void setTime(Ogre::Real time) {
    	_time = time;
    }

    Ogre::Real getTime() {
    	return _time;
    }

    void setTimeInterval(Ogre::Real timeInterval) {
    	_timeInterval = timeInterval;
    }

    Ogre::Real getTimeInterval() {
    	return _timeInterval;
    }

    Ogre::Real calculate(Ogre::Real source) {
        _time += source;
        return _time / _timeInterval;
    }
};
}

namespace X3D {
const float& TimeSensor::cycleInterval() const {
    return _cycleInterval;
}

void TimeSensor::cycleInterval(const float& cycleInterval) {
    _cycleInterval = cycleInterval;
}

void TimeSensor::loop(const bool& loop) {
	if (_loop==loop) {
		return;
	}

    _loop = loop;

    for (auto& anim: _animations) {
        // Propagate this value to AnimationStates
    	anim->setLoop(_loop);
    }
    // Smooth transition between Loop and no Loop
    for (auto func: _functions) {
    	AccumulateControllerFunction* acf = dynamic_cast<AccumulateControllerFunction*>(func.get());
    	if (_loop) {
    		// Loop is set from false to true
    		// 		--> Restart Animation if it has already finished
    		if (acf->getTime() > acf->getTimeInterval()) {
    			acf->setTime(0);
    		}
    	} else {
    		// Loop is set from true to false
    		// 		--> Wrap Time
    		acf->setTime(std::fmod(acf->getTime(), acf->getTimeInterval()));
    	}
    }
}

const bool& TimeSensor::loop() const {
    return _loop;
}

const float& TimeSensor::startTime() const {
    return _startTime;
}

void TimeSensor::startTime(const float& startTime) {
    _startTime = startTime;
}

const float& TimeSensor::stopTime() const {
    return _stopTime;
}

void TimeSensor::stopTime(const float& stopTime) {
    _stopTime = stopTime;
}

void TimeSensor::controlAnimation(Ogre::AnimationState* anim) {
    using namespace Ogre;
    _animations.push_back(anim);

    anim->setEnabled(true);
    anim->setLoop(_loop);

    ControllerFunctionRealPtr func(new AccumulateControllerFunction(_cycleInterval));
    _functions.push_back(func);
    ControllerValueRealPtr dst(new AnimationStateControllerValue(anim));

    auto& mgr = ControllerManager::getSingleton();
    mgr.createController(mgr.getFrameTimeSource(), dst, func);
}

} /* namespace X3D */

