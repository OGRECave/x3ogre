/*
 * ImageBackground.h
 *
 *  Created on: 08.09.2014
 *      Author: baudenri
 */

#pragma once

#include <vector>

#include <Appearance/Appearance.h>
#include <core/BindableNode.h>

#include <OgreColourValue.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreManualObject.h>
#include <OgreRectangle2D.h>
#include <OgreSceneNode.h>

namespace X3D {

/**
 * Class which represents polygon background
 * Equivalent to a Ogre::Rectangle2D which is infinitly far away
 *
 * TODO Currently it is only a Rectangle which has the possibility to not only have a texture but an Appearance
 * All other properties are not implemented
 *
 */

class PolygonBackground: public BindableNode<PolygonBackground> {
    std::vector<Ogre::Vector3> _texCoords;

    std::shared_ptr<Appearance> _appearance;

    std::unique_ptr<Ogre::Rectangle2D> _rect;

    Ogre::SceneNode* _root = nullptr;

public:
    static const std::string _sceneNodeName;

    void initialise(World& world) override;

    /**
	 * Specifices the Texture Coordinates
	 * It has to be 4 Vec3 elements
	 * @param index Vector of Line Indexes.
	 */
	void texCoords(const std::vector<Ogre::Vector3>& textureCoords);

    /**
	 * Setter for the ImageTexture
	 * @param texture ImageTexture of the background of the Scene
	 */
    void setAppearance(const std::shared_ptr<Appearance>& appearance);

    /**
	 * Propagates setBind command to BindableNode
	 */
    void setBind(const bool& bind) {
		set_bind(bind);
	}

    void onBound(Scene& scene) override;

    void onUnbound(Scene& scene) override;
};

}
