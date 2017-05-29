/*
 * Billboard.cpp
 *
 *  Created on: 17.09.2015
 *      Author: baudenri
 */

#include <World/Billboard.h>
#include <core/World.h>

using namespace X3D;

void Billboard::initialise(World& world) {
	if (_init) {
		return;
	}

	_sceneManager = world.sceneManager();
	_sceneManager->addListener(this);

	// Initialize Group
	Group::initialise(world);

	_init = true;
}

Billboard::~Billboard() {
    _sceneManager->removeListener(this);
}

void Billboard::attachTo(Ogre::SceneNode* node) {
    // create Ogre Nodes
	Ogre::SceneNode* groupParent = node->createChildSceneNode();
    _groupParentList.push_back(groupParent);

	Group::attachTo(groupParent);
}

void Billboard::detachFrom(Ogre::SceneNode* node) {
	for (Ogre::SceneNode* groupParent: _groupParentList) {
		node->removeChild(groupParent);
	}
}

void Billboard::preUpdateSceneGraph(Ogre::SceneManager* source, Ogre::Camera* camera){
	if (!camera) return;

	Ogre::Quaternion camOrient(0,0,0,1);
	Ogre::Vector3 camPos(0,0,0), camScale(0,0,0);

	// Get Camera orientation and the inverse
	camera->getViewMatrix().decomposition(camPos, camScale, camOrient);
	auto camOrientInv = camOrient.Inverse();

	for (auto groupParent: _groupParentList) {
		// Get the inverse rotation for every node
		auto nodeOrientInv = groupParent->getParentSceneNode()->_getDerivedOrientation().Inverse();

		// Apply the combination to make every object face the camera
		groupParent->setOrientation(nodeOrientInv*camOrientInv);
	}
}
