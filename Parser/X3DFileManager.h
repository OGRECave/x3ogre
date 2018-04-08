/*
 * X3DFileManager.h
 *
 *  Created on: 08.04.2014
 *      Author: baudenri
 */

#pragma once

#include <OgreScriptLoader.h>
#include <OgreDataStream.h>
#include <OgreStringVector.h>
#include <OgreString.h>
#include <OgreSingleton.h>
#include <OgreResourceManager.h>

#include <map>

#include <core/Node.h>

namespace X3D
{

class SceneAccessInterface;
class Scene;

class X3DFileManager : public Ogre::Singleton<X3DFileManager>
{
public:
    typedef std::shared_ptr<Scene> ScenePtr;
public:
    X3DFileManager();
    ~X3DFileManager();

    void load(const Ogre::String& filename, const Ogre::String& groupName,
            ScenePtr& scene, const Ogre::String& nameSpace = "");

    /// @copydoc Singleton::getSingleton()
    static X3DFileManager& getSingleton(void);
    /// @copydoc Singleton::getSingleton()
    static X3DFileManager* getSingletonPtr(void);
};

}
