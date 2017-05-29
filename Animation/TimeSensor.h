/*
 * TimeSensor.h
 *
 *  Created on: 31.03.2014
 *      Author: parojtbe
 */

#pragma once

#include <Animation/AnimationController.h>
#include <OgreController.h>
#include <OgreControllerManager.h>
#include <OgrePredefinedControllers.h>
#include <vector>

namespace X3D {

class TimeSensor: public AnimationController {
    bool _loop = false;
    float _cycleInterval = 1;
    float _startTime = 0;
    float _stopTime = 0;

    std::vector<Ogre::AnimationState*> _animations = std::vector<Ogre::AnimationState*>();
    std::vector<Ogre::ControllerFunctionRealPtr> _functions =std::vector<Ogre::ControllerFunctionRealPtr>();

public:
    const float& cycleInterval() const;
    const bool& loop() const;
    const float& startTime() const;
    const float& stopTime() const;

    void cycleInterval(const float& cycleInterval);
    void loop(const bool& loop);
    void startTime(const float& startTime);
    void stopTime(const float& stopTime);

    void controlAnimation(Ogre::AnimationState* anim) override;
};

} /* namespace X3D */
