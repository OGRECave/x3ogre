/*
 * Shape.h
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once

#include <list>

#include <core/Node.h>
#include <OgreSceneNode.h>

namespace X3D {
class Appearance;
class Geometry;

/**
 * Class for linking a Geometry node to an Appearance
 */
class Shape: public Node, public Ogre::Resource::Listener {
    std::shared_ptr<Geometry> _geometry;
    std::shared_ptr<Appearance> _appearance;

    std::list<Ogre::SceneNode*> _parents;

    std::string _name;
	bool _isMesh = false;

public:
	~Shape();

	void id(const std::string& id) override;

    /** callback for material
     * @param resource
     */
    void loadingComplete(Ogre::Resource* resource) override;

    /**
     * @param node SceneNode
     */
    void attachTo(Ogre::SceneNode* node);

    /**
     * @param node SceneNode
     */
    void detachFrom(Ogre::SceneNode* node);

    /**
     * Specifies the Appearance of the underlying Geometry
     * @param appearance
     */
    void setAppearance(const std::shared_ptr<Appearance>& appearance);

    /**
     * Specifies the Geometry which look is defined by the Appearance-Class
     * @param geo Geometry
     */
    void setGeometry(const std::shared_ptr<Geometry>& geo);

    /**
     * Initializes the Appearance and Creates the Mesh and an instance of the Geometry
     * @param world
     */
    void initialise(World& world) override;
};

}
