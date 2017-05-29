/*
 * Geometry.cpp
 *
 *  Created on: 26.03.2014
 *      Author: parojtbe
 */

#include <Geometry/Geometry.h>
#include <OgreMeshManager.h>

namespace X3D {

Ogre::NameGenerator Geometry::_nameGenerator("Geometry_");
std::string Geometry::_noAnimation;

void Geometry::create() {
    if (_name.empty()) {
        _name = _nameGenerator.generate();
    }

    if(Ogre::MeshManager::getSingleton().resourceExists(_name, "X3D")) {
        return;
    }

    auto mesh = Ogre::MeshManager::getSingleton().createManual(_name, "X3D", this);
    mesh->load();

}

Geometry::~Geometry() {
    if(_name.empty() or not Ogre::MeshManager::getSingleton().resourceExists(_name, "X3D")) {
        return;
    }

    Ogre::MeshManager::getSingleton().remove(_name, "X3D");
}

}
