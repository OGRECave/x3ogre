/*
 * X3DFileManager.cpp
 *
 *  Created on: 08.04.2014
 *      Author: baudenri
 */

#include "SceneLoader.h"
#include <core/World.h>
#include <World/Scene.h>
#include <Parser/X3DParser.h>
#include <iostream>
#include <OgreLogManager.h>
#include <OgreSceneLoaderManager.h>

using namespace X3D;

void SceneLoader::load(Ogre::DataStreamPtr& stream,
                          const Ogre::String& groupName,
                          Ogre::SceneNode* rootNode, const Ogre::String& nameSpace) {
    auto scene = std::make_shared<Scene>();
    scene->attachTo(rootNode);
    rootNode->getUserObjectBindings().setUserAny("x3d_scene", Ogre::Any(scene));

    std::string xml = stream->getAsString();
    stream->close();

    try {
        Ogre::LogManager::getSingleton().logMessage("X3DFileManager::load: '"+stream->getName()+"'", Ogre::LML_TRIVIAL);
        X3DParser parser(xml, *scene, *scene, nameSpace);
    } catch (std::exception &e) {
        Ogre::LogManager::getSingleton().logMessage("error parsing '"+stream->getName()+": "+e.what(), Ogre::LML_CRITICAL);
    }

    World world = {scene.get()};
    scene->initialise(world);
}

SceneLoader::SceneLoader() {
    Ogre::SceneLoaderManager::getSingleton().registerSceneLoader("X3D", {".x3d"}, this);
}

SceneLoader::~SceneLoader() {
    Ogre::SceneLoaderManager::getSingleton().unregisterSceneLoader("X3D");
}
