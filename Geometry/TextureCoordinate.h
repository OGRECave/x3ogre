/*
 * TextureCoordinate.h
 *
 *  Created on: 24.01.2014
 *      Author: baudenri
 */

#pragma once
#include <Geometry/Geometry.h>
#include <OgreVector2.h>

namespace X3D {

/**
 * Vector of 2D positions between 0 and 1
 * Used as Texture Coordinates
 */
class TextureCoordinate: public Node {
    std::vector<Ogre::Vector2> _point;
public:
    /**
     * Sets the 2D position Vector
     * @param point Texture Position Vector
     */
    void point(const std::vector<Ogre::Vector2>& point);

    /**
     * @return Returns the Texture Position Vector
     */
    const std::vector<Ogre::Vector2>& point() const;
};

}
