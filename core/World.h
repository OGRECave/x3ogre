/*
 * World.h
 *
 *  Created on: 19.11.2013
 *      Author: baudenri_local
 */

#pragma once

#include <OgreSceneManager.h>
#include <World/Scene.h>

namespace X3D {

/**
 * World is the starting class for building a Scene.
 * It contains the Root-Node and the SceneManager
 */
struct World {
    Scene* const _scene;

    /**
     * Getter for the sceneManager
     * @return Ogre::SceneManager
     */
    Ogre::SceneManager* sceneManager() {
        return _scene->getSceneNode()->getCreator();
    }

    /**
     * Getter for the Scene
     * @return scene
     */
    Scene* scene() {
        return _scene;
    }
};

}
