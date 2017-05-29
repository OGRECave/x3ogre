/*
 * OgreMeshAdapter.h
 *
 *  Created on: 26.03.2014
 *      Author: parojtbe
 */

#pragma once

#include <vector>
#include <ciso646>
#include <OgreMesh.h>
#include <OgreRenderOperation.h>

namespace X3D {

template<class T>
void OgreMeshAdapter(
		Ogre::Mesh* mesh,
        Ogre::RenderOperation::OperationType rop,
        const std::vector<T>& idx,
        const std::vector<Ogre::Vector3>& coord,
        const std::vector<Ogre::Vector3>& normal,
        const std::vector<Ogre::Vector2>& texCoord = std::vector<Ogre::Vector2>(),
        const std::vector<Ogre::Vector3>& color = std::vector<Ogre::Vector3>());

} /* namespace X3D */
