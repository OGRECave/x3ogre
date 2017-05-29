/*
 * Group.h
 *
 *  Created on: 06.03.2014
 *      Author: parojtbe
 */

#pragma once

#include <core/Node.h>
#include <OgreSceneNode.h>

namespace X3D {

/**
 * Vector of X3DObjects
 */
class Group : public Node {
protected:
    std::vector<std::shared_ptr<Node> > _children;
    Ogre::SceneNode* _groupParent = nullptr;

    /**
     * attaches obj to _groupParent if possible
     * @param obj
     */
    void attachChild(Node* obj, Ogre::SceneNode* groupParent);

public:
    /**
     * Initializes all the Children
     * @param sceneManager
     */
    void initialise(World& world) override;

    /**
     * @param node SceneNode
     */
    virtual void detachFrom(Ogre::SceneNode* node);

    /**
     * Specifies the parent node
     * to which all sub-nodes will be attached to
     * @param node Parent-Node of the Group
     */
    virtual void attachTo(Ogre::SceneNode* node);

    /**
     * Adds a child node
     * All Child nodes will be transformed if this is a Transformation
     * @param obj Transform or Shape
     */
    virtual void addChild(const std::shared_ptr<Node>& obj);

    /**
     * Sets visibility for all underlying objects
     * @oaram visible objects should be visible
     */
    void visible(const bool& visible);

	/**
	 * Creates a deep copy of this object
	 */
	//virtual std::shared_ptr<X3DObject> clone();
};

} /* namespace X3D */

