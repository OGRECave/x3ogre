/*
 * RTShaderSystem.cpp
 *
 *  Created on: 05.09.2014
 *      Author: baudenri
 */

#include <Appearance/RTShaderSystem.h>
#include <iostream>
#include <core/StringConverter.h>

namespace X3D {

void RTShaderSystem::property(const std::string& property) {
	_node->name = property;
}

RTShaderSystem::RTShaderSystem() : _node(new Ogre::PropertyAbstractNode(nullptr)) {
    _node->file = "X3D::RTShaderSystem";
}

void RTShaderSystem::value(const std::vector<std::string>& values) {
	for (const auto& val: values) {
		auto aan = new Ogre::AtomAbstractNode(_node.get());
		aan->value = val;
		_node->values.push_back(Ogre::AbstractNodePtr(aan));
	}
}

}


