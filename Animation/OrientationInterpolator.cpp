/*
 * OrientationInterpolator.cpp
 *
 *  Created on: 02.04.2014
 *      Author: baudenri
 */


#include <Animation/OrientationInterpolator.h>

#include <string>

#include <OgreAnimation.h>
#include <OgreKeyFrame.h>

#include <OgreControllerManager.h>
#include <OgrePredefinedControllers.h>

#include <iostream>
#include <algorithm>

using namespace std;

namespace X3D {

void OrientationInterpolator::set_fraction(const std::shared_ptr<AnimationController>& controller) {
    _controller = controller;
}

void OrientationInterpolator::configure(Ogre::SceneManager* sceneManager, Transform& transform) const {
	using namespace Ogre;

	if(not _controller) {
        throw std::runtime_error("OrientationInterpolator requires a Controller");
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
		tkf->setRotation(_keyValue[i]);
		tkf->setTranslate(transform.center());
	}

	AnimationState* state = sceneManager->createAnimationState(animationName);
	_controller->controlAnimation(state);
}

void OrientationInterpolator::key(const std::vector<float>& val) {
    _key = val;
}

void OrientationInterpolator::id(const std::string& id) {
    _name = id;
}

void OrientationInterpolator::keyValue(const std::vector<Ogre::Quaternion>& val) {
    _keyValue = val;
}

const std::string& OrientationInterpolator::name() const {
    return _name;
}

} /* namespace X3D */

