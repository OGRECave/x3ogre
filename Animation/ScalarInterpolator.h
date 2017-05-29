/*
 * ScalarInterpolator.h
 *
 *  Created on: 28.03.2014
 *      Author: parojtbe
 */

#pragma once

#include <Animation/AnimationController.h>
#include <Animation/TimeSensor.h>
#include <memory>

namespace X3D {

class ScalarInterpolator: public AnimationController {
    std::vector<float> _key;
    std::vector<float> _keyValue;
    std::shared_ptr<TimeSensor> _time;

public:
    void key(const std::vector<float>& key);
    void keyValue(const std::vector<float>& keyValue);

    const std::vector<float>& key() const;
    const std::vector<float>& keyValue() const;

    void controlAnimation(Ogre::AnimationState* anim) override;

    void set_fraction(const std::shared_ptr<TimeSensor>& time);
};

} /* namespace X3D */
