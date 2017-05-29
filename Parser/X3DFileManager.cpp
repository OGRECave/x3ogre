/*
 * X3DFileManager.cpp
 *
 *  Created on: 08.04.2014
 *      Author: baudenri
 */

#include <Parser/X3DFileManager.h>
#include <core/SceneAccessInterface.h>
#include <World/Inline.h>
#include <World/Scene.h>
#include <Parser/X3DFile.h>
#include <Parser/X3DParser.h>
#include <iostream>


using namespace X3D;

//-----------------------------------------------------------------------
template<> X3DFileManager* Ogre::Singleton<X3DFileManager>::msSingleton = 0;

X3DFilePtr X3DFileManager::load(const Ogre::String& filename,
                                     const Ogre::String& groupName,
                                     ScenePtr& scene, const Ogre::String& nameSpace) {
    X3DFilePtr pFile = Ogre::static_pointer_cast<X3DFile>(createOrRetrieve(filename,groupName,scene, nameSpace).first);
    pFile->load();
    return pFile;
}

X3DFileManager::ResourceCreateOrRetrieveResult
X3D::X3DFileManager::createOrRetrieve(
    const Ogre::String& filename, const Ogre::String& groupName, ScenePtr& scene, const Ogre::String& nameSpace) {
    ResourceCreateOrRetrieveResult res =
        ResourceManager::createOrRetrieve(filename,groupName,true);
    X3DFilePtr pFile = Ogre::static_pointer_cast<X3DFile>(res.first);
    pFile->setScene(scene);
    pFile->setNameSpace(nameSpace);
    return res;


}

X3DFileManager* X3DFileManager::getSingletonPtr(void) { return msSingleton; }
X3DFileManager& X3DFileManager::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}

X3DFileManager::X3DFileManager() {
    mLoadOrder = 1;
    mResourceType = "X3DFile";
}

X3DFileManager::~X3DFileManager() {
}

Ogre::Resource* X3DFileManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
             const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
             const Ogre::NameValuePairList *createParams) {

	// Creating an X3DFile and adding it to the Resources
	return new X3DFile(this, name, getNextHandle(), group);
}
