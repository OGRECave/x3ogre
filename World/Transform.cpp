/*
 * Transform.cpp
 *
 *  Created on: 17.01.2014
 *      Author: baudenri_local
 */

#include <World/Transform.h>

#include <Geometry/Shape.h>
#include <Animation/PositionInterpolator.h>
#include <Animation/OrientationInterpolator.h>
#include <core/ClonePropagationListener.h>
#include <core/World.h>

using namespace X3D;

Transform::Transform() {
}

Transform::~Transform() {
    if(!_topNodeList.empty())
        _topNodeList.front()->setListener(0);
}

void Transform::center(const Ogre::Vector3& center) {
	_center = center;
	if (_topNodeList.empty()) return;
	_topNodeList.front()->setPosition(_translation + _center);
    _groupParentList.front()->setPosition(-_center);
}

void Transform::scale(const Ogre::Vector3& scale){
    _scale = scale;
    if (_topNodeList.empty()) return;
    _topNodeList.front()->setScale(_scaleOrientation*_scale);
}

void Transform::translation(const Ogre::Vector3& translation) {
    _translation = translation;
    if (_topNodeList.empty()) return;
    _topNodeList.front()->setPosition(_translation + _center);
}

void Transform::rotation(const Ogre::Quaternion& rotation) {
    _orientation = rotation;
    if (_topNodeList.empty()) return;
    _topNodeList.front()->setOrientation(_orientation);
}

void Transform::scaleOrientation(const Ogre::Quaternion& scaleOrientation) {
	_scaleOrientation = scaleOrientation;
	if (_topNodeList.empty()) return;
	_topNodeList.front()->setScale(_scaleOrientation*_scale);
}

void Transform::attachTo(Ogre::SceneNode* node) {
    // create Ogre Nodes
	Ogre::SceneNode* topNode = node->createChildSceneNode();
	_topNodeList.push_back(topNode);

	// TODO is this necessary? Can't we just access the "groupParent" form the topNode via "getChild"?
	Ogre::SceneNode* groupParent = topNode->createChildSceneNode();
    _groupParentList.push_back(groupParent);

	Group::attachTo(groupParent);
}

void Transform::detachFrom(Ogre::SceneNode* node) {
	for (Ogre::SceneNode* topNode: _topNodeList) {
		node->removeChild(topNode);
	}
}

// Ererbte Funktion
void Transform::initialise(World& world) {
	if (_init) {
		return;
	}

	updateSceneNode();

    Group::initialise(world);

    _init = true;

	_clonePropagationListener.reset(new ClonePropagationListener(this));
    _topNodeList.front()->setListener(_clonePropagationListener.get());

    if(_positionInterpolator) {
        _positionInterpolator->configure(world.sceneManager(), *this);
    }
    if(_orientationInterpolator) {
        _orientationInterpolator->configure(world.sceneManager(), *this);
    }
}

void Transform::set_translation(const std::shared_ptr<PositionInterpolator>& pi) {
	_positionInterpolator = pi;
}

void Transform::set_rotation(const std::shared_ptr<OrientationInterpolator>& oi) {
	_orientationInterpolator = oi;
}

void Transform::updateSceneNode() {
    // From now on take the values from the first topNode
    _center = -_groupParentList.front()->getPosition();
    _translation = _topNodeList.front()->getPosition() - _center;
    _orientation = _topNodeList.front()->getOrientation();
    _scale = _topNodeList.front()->getScale();

    // Propagate these Values to all other topNodes
    for (size_t i = 1; i < _topNodeList.size(); i++) {
        _topNodeList[i]->setPosition(_translation + _center);
        _topNodeList[i]->setOrientation(_orientation);
        _topNodeList[i]->setScale(_scaleOrientation * _scale);
        _groupParentList[i]->setPosition(-_center);

        // This Function will be called during update process
        // Thus its necessary to add every modified node to an Update list
        Ogre::Node::queueNeedUpdate(_topNodeList[i]);
    }
}
