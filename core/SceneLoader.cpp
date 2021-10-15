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

using namespace X3D;

void SceneLoader::load(const Ogre::DataStreamPtr& stream,
                          const Ogre::String& groupName,
                          Ogre::SceneNode* rootNode, const Ogre::String& nameSpace) const {
    auto scene = std::make_shared<Scene>();
    scene->attachTo(rootNode);
    rootNode->getUserObjectBindings().setUserAny("x3d_scene", Ogre::Any(scene));

    std::string xml = stream->getAsString();
    stream->close();

    try {
        Ogre::LogManager::getSingleton().logMessage("X3DFileManager::load: '"+stream->getName()+"'", Ogre::LML_TRIVIAL);
        X3DParser parser(xml, *scene, *scene, nameSpace);
    } catch (std::exception &e) {
        Ogre::LogManager::getSingleton().logError("parsing '"+stream->getName()+": "+e.what());
    }

    World world = {scene.get()};
    scene->initialise(world);
}

SceneLoader::SceneLoader() {
    if(auto other = Ogre::Codec::getCodec("x3d"))
    {
        Ogre::Codec::unregisterCodec(other);
        Ogre::LogManager::getSingleton().logWarning("unregistering previous codec for loading X3D");
    }
    Ogre::Codec::registerCodec(this);
}

SceneLoader::~SceneLoader() {
    Ogre::Codec::unregisterCodec(this);
}
