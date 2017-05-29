/*
 * IndexedPrimitiveSet.h
 *
 *  Created on: 05.12.2013
 *      Author: baudenri_local
 */

#pragma once

#include "Geometry.h"

namespace X3D {
class Coordinate;
class Normal;
class TextureCoordinate;
class Color;

/**
 * Class which contains 3D Positions
 *   possibly with Normals and Texture-Positions
 * @see IndexedLineSet
 * @see IndexedTriangleSet
 * @see IndexedFaceSet
 */
class IndexedPrimitiveSet: public Geometry {
protected:
    std::vector<int> _textureIndex;

    std::shared_ptr<Coordinate> _coordinate;
    std::shared_ptr<Normal> _normal;
	std::shared_ptr<TextureCoordinate> _textureCoordinate;
    std::shared_ptr<Color> _color;
public:


    /**
     * Specifies the TextureIndexes
     * @param textureIndex
     */
	void texCoordIndex(const std::vector<int>& textureIndex) {
		_textureIndex = textureIndex;
	}

	/**
	 * Specifies the TextureCoordinates
	 * @param textureCoordinates
	 */
	void textureCoordinates(const std::shared_ptr<TextureCoordinate>& textureCoordinates) {
		_textureCoordinate = textureCoordinates;
	}

	/**
	 * Specifies the coordinates
	 * @param coordinates
	 */
    void coordinate(const std::shared_ptr<Coordinate>& coordinates) {
        _coordinate = coordinates;
    }

    /**
     * Specifies the normals
     * @param normal
     */
    void normal(const std::shared_ptr<Normal>& normal) {
        _normal = normal;
    }

    /**
     * Specifies the ColorVector.
     * colorIndex takes this Colors to color the Mesh
     */
    void color(const std::shared_ptr<Color>& color) {
        _color = color;
    }

    bool hasVertexColors() const override {
        return bool(_color);
    }

    void configureAnimation(Ogre::AnimationState* anim) const override;

    const Ogre::String& animation() const override;
};

}
