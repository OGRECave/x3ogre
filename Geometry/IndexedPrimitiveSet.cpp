/*
 * IndexedPrimitiveSet.cpp
 *
 *  Created on: 11.04.2014
 *      Author: parojtbe
 */

#include <Geometry/IndexedPrimitiveSet.h>
#include <Geometry/Coordinate.h>

namespace X3D {

const Ogre::String& IndexedPrimitiveSet::animation() const {
    const auto& ci = _coordinate->interpolator();
    return ci ? ci->name() : _noAnimation;
}

void IndexedPrimitiveSet::configureAnimation(Ogre::AnimationState* anim) const {
    if(_coordinate->interpolator()) {
        _coordinate->interpolator()->configure(anim);
    }
}

}
