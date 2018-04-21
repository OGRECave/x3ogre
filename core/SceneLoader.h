/*
 * X3DFileManager.h
 *
 *  Created on: 08.04.2014
 *      Author: baudenri
 */

#pragma once

#include <OgreDataStream.h>
#include <OgreString.h>
#include <OgreSingleton.h>
#include <OgreSceneNode.h>
#include <OgreSceneLoader.h>

namespace X3D
{

class SceneLoader : public Ogre::SceneLoader
{
public:
    SceneLoader();
    ~SceneLoader();

    void load(Ogre::DataStreamPtr& stream, const Ogre::String& groupName,
              Ogre::SceneNode* rootNode) override {
        load(stream, groupName, rootNode, "");
    }

    void load(Ogre::DataStreamPtr& stream, const Ogre::String& groupName,
            Ogre::SceneNode* rootNode, const Ogre::String& nameSpace);
};

}
