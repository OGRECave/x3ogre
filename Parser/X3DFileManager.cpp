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
#include <Parser/X3DParser.h>
#include <iostream>
#include <OgreLogManager.h>

using namespace X3D;

//-----------------------------------------------------------------------
template<> X3DFileManager* Ogre::Singleton<X3DFileManager>::msSingleton = 0;

void X3DFileManager::load(const Ogre::String& filename,
                          const Ogre::String& groupName,
                          Ogre::SceneNode* rootNode, const Ogre::String& nameSpace) {
    auto stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, groupName);

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
}

X3DFileManager* X3DFileManager::getSingletonPtr(void) { return msSingleton; }
X3DFileManager& X3DFileManager::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}

X3DFileManager::X3DFileManager() {
}

X3DFileManager::~X3DFileManager() {
}
