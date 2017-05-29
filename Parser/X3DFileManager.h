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

#include "X3DFile.h"

namespace X3D
{

class SceneAccessInterface;
class Scene;

class X3DFileManager : public Ogre::ResourceManager, public Ogre::Singleton<X3DFileManager>
{
public:
    typedef std::shared_ptr<Scene> ScenePtr;
protected:
    /**
     * Called by ResourceManager before loading a file. The X3DFileManager creates a
     * X3DFile and adds it to the Resources list.
     * @param name
     * @param handle
     * @param group
     * @param isManual
     * @param loader
     * @param createParams
     */
    Ogre::Resource *createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
             const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
             const Ogre::NameValuePairList *createParams);

    ResourceCreateOrRetrieveResult
    createOrRetrieve(const Ogre::String& filename, const Ogre::String& groupName,
            ScenePtr& scene, const Ogre::String& nameSpace);
public:
    X3DFileManager();
    ~X3DFileManager();

    X3DFilePtr load(const Ogre::String& filename, const Ogre::String& groupName,
            ScenePtr& scene, const Ogre::String& nameSpace = "");

    /// @copydoc Singleton::getSingleton()
    static X3DFileManager& getSingleton(void);
    /// @copydoc Singleton::getSingleton()
    static X3DFileManager* getSingletonPtr(void);
};

}
