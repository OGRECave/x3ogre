/*
 * PositionInterpolator.cpp
 *
 *  Created on: 02.04.2014
 *      Author: baudenri
 */


#include "PositionInterpolator.h"

#include <string>

#include <OgreAnimation.h>
#include <OgreKeyFrame.h>

#include <iostream>
#include <algorithm>

using namespace std;

namespace X3D {

void PositionInterpolator::set_fraction(const std::shared_ptr<AnimationController>& controller) {
    _controller = controller;
}

void PositionInterpolator::configure(Ogre::SceneManager* sceneManager, Transform& transform) const {
	using namespace Ogre;

	if(not _controller) {
        throw std::runtime_error("PositionInterpolator requires a Controller");
    }
	if(not transform.getTopNode()) {
		throw std::runtime_error("OrientationInterpolator needs target Transform");
	}

	std::string animationName = _name+"/"+transform.name();

	Animation* anim = sceneManager->createAnimation(animationName,1.0);
	NodeAnimationTrack* track = anim->createNodeTrack(0, transform.getTopNode());

	TransformKeyFrame* tkf;

	for(size_t i=0; i< _key.size(); i++) {
		tkf = track->createNodeKeyFrame(_key[i]);
		tkf->setTranslate(_keyValue[i] + transform.center());
	}

	AnimationState* state = sceneManager->createAnimationState(animationName);
	_controller->controlAnimation(state);
}

void PositionInterpolator::key(const std::vector<float>& val) {
    _key = val;
}

void PositionInterpolator::id(const std::string& id) {
    _name = id;
}

void PositionInterpolator::keyValue(const std::vector<Ogre::Vector3>& val) {
    _keyValue = val;
}

const std::string& PositionInterpolator::name() const {
    return _name;
}

} /* namespace X3D */

