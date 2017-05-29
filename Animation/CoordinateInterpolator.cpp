/*
 * CoordinateInterpolator.cpp
 *
 *  Created on: 25.03.2014
 *      Author: parojtbe
 */

#include "CoordinateInterpolator.h"

#include <Animation/ScalarInterpolator.h>
#include <Animation/TimeSensor.h>

#include <string>

#include <OgreMesh.h>
#include <OgreMeshManager.h>
#include <OgreHardwareBuffer.h>
#include <OgreHardwareBufferManager.h>
#include <OgreEntity.h>
#include <OgreKeyFrame.h>

#include <iostream>
#include <algorithm>

using namespace std;

namespace X3D {

void CoordinateInterpolator::set_fraction(const std::shared_ptr<AnimationController>& controller) {
    _controller = controller;
}

void CoordinateInterpolator::configure(Ogre::AnimationState* anim) const {
    if(not _controller) {
        throw std::runtime_error("CoordinateInterpolator requires a Controller");
    }

    _controller->controlAnimation(anim);
}

void CoordinateInterpolator::key(const std::vector<float>& val) {
    _key = val;
}

void CoordinateInterpolator::id(const std::string& id) {
    _name = id;
}

void CoordinateInterpolator::keyValue(const std::vector<Ogre::Vector3>& val) {
    _keyValue = val;
}

void CoordinateInterpolator::addToMesh(Ogre::Mesh* mesh) {
    using namespace Ogre;
    size_t numVerts = _keyValue.size()/_key.size();

    Animation* anim = mesh->createAnimation(_name, 1.0);
    VertexAnimationTrack* track = anim->createVertexTrack(0, Ogre::VAT_MORPH);
    track->setTargetMode(VertexAnimationTrack::TM_HARDWARE);

    size_t vertexSize = VertexElement::getTypeSize(VET_FLOAT3);

    HardwareBufferManager& hbm = HardwareBufferManager::getSingleton();

    VertexMorphKeyFrame* kf;
    HardwareVertexBufferSharedPtr buf;

    for(size_t i = 0; i < _key.size(); i++) {
        kf = track->createVertexMorphKeyFrame(_key[i]);
        buf = hbm.createVertexBuffer(vertexSize, numVerts, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        buf->writeData(0, buf->getSizeInBytes(), _keyValue[i*numVerts].ptr(), true);
        kf->setVertexBuffer(buf);
    }
}

const std::string& CoordinateInterpolator::name() const {
    return _name;
}

} /* namespace X3D */
