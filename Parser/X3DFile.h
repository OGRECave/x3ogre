/*
 * X3DFile.h
 *
 *  Created on: 08.04.2014
 *      Author: baudenri
 */

#pragma once

#include <World/Scene.h>
#include <OgreResourceManager.h>
#include <core/Node.h>

namespace X3D {

/**
 * Group node, primarily to hold the URL of a file. This URL is read in and
 * parsed by X3DFile object.
 */
class X3DFile: public Ogre::Resource {
	std::string _nameSpace = "";
	std::shared_ptr<Scene> _scene;
protected:

	/**
	 * Function inherit from Resource. Specifies behavior if the Resource is loaded.
	 * For the X3DFile this means reading and parsing (see void
	 * parseFile(Ogre::DataStreamPtr& stream)) the x3d-file from the datasystem.
	 */
	void loadImpl();
	void unloadImpl();
	size_t calculateSize(void) const;
public:
#if defined(_MSC_VER) && _MSC_VER < 1900 // MSVC does not support inheriting constructors
  	X3DFile(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
            const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0)
           : Ogre::Resource(creator,name,handle,group,isManual,loader) {
    }
#endif // else inherit constructor:
	using Ogre::Resource::Resource;

	void setScene(const std::shared_ptr<Scene>& scene) {
	    _scene = scene;
	}

	/**
	 * Setter for the nameSpace of an internalLoaded file. The namespace is the
	 * prefix, a node with definition from an internalLoaded file has to have, if it
	 * will be referenced in the main/parrent file.
	 * @param nameSpace
	 */
	void setNameSpace(const std::string& nameSpace);

	/**
	 * Creates a X3DParser object with specific features. If file is not internalLoaded
	 * (see void setInternalLoaded(bool internalLoaded)), all nodes will be added to the
	 * Root node of the world. On the other hand if it is internalLoaded, the root has
	 * to be specified and is the target where all child nodes are added to.
	 * @param stream
	 */
	void parseFile(Ogre::DataStreamPtr& stream);
};

typedef Ogre::SharedPtr<X3DFile> X3DFilePtr;

}
