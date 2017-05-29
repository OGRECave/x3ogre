/*
 * ClonePropagationListener.h
 *
 *  Created on: 15.05.2014
 *      Author: baudenri
 */

#pragma once

#include <sstream>
#include <string>
#include <OgreNode.h>
#include <World/Transform.h>

namespace X3D {

class ClonePropagationListener : public Ogre::Node::Listener {

private:
	Transform* _transform;

public:
	ClonePropagationListener(Transform* transform) {
		_transform = transform;
	}

	virtual void nodeUpdated(const Ogre::Node*) {
		_transform->updateSceneNode();
	}
	virtual void nodeDestroyed(const Ogre::Node*) {
		// TODO If node which holds Listener could be destroyed
		// The Listener should destroy clones or just destroy itself
	}
};

}
