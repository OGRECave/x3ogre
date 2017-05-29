/*
 * World.h
 *
 *  Created on: 19.11.2013
 *      Author: baudenri_local
 */

#pragma once

#include <OgreSceneManager.h>

namespace X3D {

class Scene;

/**
 * World is the starting class for building a Scene.
 * It contains the Root-Node and the SceneManager
 */
struct World {
    Ogre::SceneManager* const _sceneManager;
    Scene* const _scene;

    /**
     * Getter for the sceneManager
     * @return Ogre::SceneManager
     */
    Ogre::SceneManager* sceneManager() {
        return _sceneManager;
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
