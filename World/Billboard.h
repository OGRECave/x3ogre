/*
 * Billboard.h
 *
 *  Created on: 17.09.2015
 *      Author: baudenri
 */

#pragma once

#include <World/Group.h>
#include <World/Scene.h>
#include <core/ClonePropagationListener.h>

#include <OgreNode.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>

namespace X3D {

/**
 * The Billboard node is a grouping node that transforms the
 * coordinate system of its children so that the local Z-axis
 * of the children turns to point at the viewer within the
 * limitations of its rotational axis.
 */
class Billboard : public Group , public Ogre::SceneManager::Listener {

private:
	/**
	 * NodeList is partitioned as:
	 * 0    = SceneNode representing this billboard
	 * 1..N = clones of this SceneNode created by def/ use mechanism
	 * when this node gets updated, all clones are updated by clonepropagationlistner
	 */
	std::vector<Ogre::SceneNode*> _groupParentList;
	Ogre::SceneManager* _sceneManager = nullptr;
public:
	~Billboard();

    /**
     * Initializes all the Children
     * @param world
     */
    void initialise(World& world) override;

	/**
	 * @param node SceneNode
	 */
	void detachFrom(Ogre::SceneNode* node) override;

	/**
	 * @param node SceneNode
	 */
	void attachTo(Ogre::SceneNode* node) override;


	void preUpdateSceneGraph(Ogre::SceneManager* source, Ogre::Camera* camera) override;

};

} /* namespace X3D */

