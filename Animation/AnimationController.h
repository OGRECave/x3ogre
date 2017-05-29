/*
 * AnimationController.h
 *
 *  Created on: 11.04.2014
 *      Author: parojtbe
 */

#pragma once

#include <OgrePrerequisites.h>
#include <core/Node.h>

namespace X3D {

class AnimationController : public Node {
public:
    virtual void controlAnimation(Ogre::AnimationState* anim) = 0;
};

}

