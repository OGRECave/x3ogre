/*
 * BindableNode.h
 *
 *  Created on: 20.11.2014
 *      Author: baudenri
 */

#pragma once

#include <core/Node.h>
#include <core/World.h>
#include <World/Scene.h>
#include <OgreLogManager.h>

namespace X3D {

/**
 *	Only one BindableNode can be active at one time
 *	http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/core.html#BindableChildrenNodes
 */
template <typename T>
class BindableNode : public Node {
protected:
	Scene* _scene = nullptr;

public:
	void set_bind(const bool& bound) {
	    if (bound) {
	        // set_bound
	        _scene->activateBindable<T>(this);
	    } else {
			Ogre::LogManager::getSingleton().logMessage("set_bind(false) is not implemented\n", Ogre::LogMessageLevel::LML_CRITICAL);
	    }
	}

	virtual void onBound(Scene& scene) {}
	virtual void onUnbound(Scene& scene) {}

	void initialise(World& world) {
	    // in scene
	    _scene = world.scene();

	    // initialize
	    if(not _scene->bound<T>()) {
	        _scene->activateBindable<T>(this);
	    }
	}
};

}

