/*
 * ScalarInterpolator.cpp
 *
 *  Created on: 28.03.2014
 *      Author: parojtbe
 */

#include "ScalarInterpolator.h"

#include <OgreControllerManager.h>
#include <OgrePredefinedControllers.h>
#include <OgreAnimationState.h>

namespace X3D {
const std::vector<float>& ScalarInterpolator::key() const {
    return _key;
}

void ScalarInterpolator::key(const std::vector<float>& key) {
    _key = key;
}

const std::vector<float>& ScalarInterpolator::keyValue() const {
    return _keyValue;
}

void ScalarInterpolator::keyValue(const std::vector<float>& keyValue) {
    _keyValue = keyValue;
}

void ScalarInterpolator::set_fraction(const std::shared_ptr<TimeSensor>& time) {
    _time = time;
}

void ScalarInterpolator::controlAnimation(Ogre::AnimationState* anim) {
    if(not _time) {
        throw std::runtime_error("ScalarInterpolator requires Route from TimeSensor ");
    }

    anim->setEnabled(true);
    anim->setLoop(_time->loop());

    Ogre::ControllerFunctionRealPtr func(new Ogre::LinearControllerFunction(_key, _keyValue, 1.0/_time->cycleInterval()));

    Ogre::ControllerValueRealPtr dst(new Ogre::AnimationStateControllerValue(anim));

    auto& mgr = Ogre::ControllerManager::getSingleton();
    mgr.createController(mgr.getFrameTimeSource(), dst, func);
}

} /* namespace X3D */

