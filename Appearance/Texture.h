/*
 * Texture.h
 *
 *  Created on: 10.10.2014
 *      Author: parojtbe
 */

#pragma once

#include <core/Node.h>
#include <OgrePass.h>

namespace X3D {

/**
 * Base class for all Textures
 */
class Texture: public Node {

public:
    /**
     * Add Texture to Ogre::Pass
     * @param pass Pass the texture is linked to
     */
    virtual void apply(Ogre::Pass* pass) = 0;
};

}
