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
#include <OgreCodec.h>
#include <OgreResourceGroupManager.h>

namespace X3D
{

class SceneLoader : public Ogre::Codec
{
public:
    SceneLoader();
    ~SceneLoader();

    Ogre::String magicNumberToFileExt(const char* magicNumberPtr, size_t maxbytes) const override { return ""; }
    Ogre::String getType() const override { return "x3d"; }

    void decode(const Ogre::DataStreamPtr& input, const Ogre::Any& output) const override
    {
        auto groupName = Ogre::ResourceGroupManager::getSingleton().getWorldResourceGroupName();
        auto rootNode = Ogre::any_cast<Ogre::SceneNode*>(output);
        load(input, groupName, rootNode, "");
    }

    void load(const Ogre::DataStreamPtr& stream, const Ogre::String& groupName,
            Ogre::SceneNode* rootNode, const Ogre::String& nameSpace) const;
};

}
