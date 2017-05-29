/*
 * RTShaderSystem.h
 *
 *  Created on: 05.09.2014
 *      Author: baudenri
 */

#pragma once
#include <core/Node.h>
#include <OgreScriptCompiler.h>

namespace X3D {

class RTShaderSystem: public Node {
    Ogre::SharedPtr<Ogre::PropertyAbstractNode> _node;
public:
	RTShaderSystem();

	void property(const std::string& property);
	void value(const std::vector<std::string>& values);

	Ogre::SharedPtr<Ogre::PropertyAbstractNode>& node() {
	    return _node;
	}
};

}
