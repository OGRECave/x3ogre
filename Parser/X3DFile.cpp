/*
 * X3DFile.cpp
 *
 *  Created on: 08.04.2014
 *      Author: baudenri
 */

#include <Parser/X3DFile.h>
#include <Parser/X3DFileManager.h>
#include <Parser/X3DParser.h>
#include <core/SceneAccessInterface.h>
#include <core/World.h>

#include <OgreLogManager.h>
#include <iostream>

using namespace X3D;

void X3DFile::loadImpl() {
    auto stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup);

    if(not stream) {
        OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, "File not Found in ResourceGroup","X3DFileManager");
    }
    parseFile(stream);
}

void X3DFile::setNameSpace(const std::string& nameSpace) {
	_nameSpace = nameSpace;
}

void X3DFile::parseFile(Ogre::DataStreamPtr& stream) {
    if(!stream) {
        std::cerr << "Unable to open file '" << stream->getName() << "'\n";
        return;
    }

    std::string xml = stream->getAsString();
    stream->close();

    mSize = xml.length();

    try {
        Ogre::LogManager::getSingleton().logMessage("X3DFile::parseFile: '"+stream->getName()+"'", Ogre::LML_TRIVIAL);
        X3DParser parser(xml, *_scene, *_scene, _nameSpace);
    } catch (std::exception &e) {
        Ogre::LogManager::getSingleton().logMessage("error parsing '"+stream->getName()+": "+e.what(), Ogre::LML_CRITICAL);
    }
}

void X3DFile::unloadImpl() {
	//TODO Unload File
}

size_t X3DFile::calculateSize(void) const {
	return mSize;
}

