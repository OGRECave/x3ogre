/*
 * NavigationInfo.cpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#include <World/NavigationInfo.h>
#include <core/World.h>
#include <World/Viewpoint.h>

using namespace X3D;

void NavigationInfo::headlight(const bool& headLight) {
	_headlight = headLight;

	_light->on(headLight);

	if (not _headlight && _sceneManager) {
		// Turn on ambient light if headlight is off
		_sceneManager->setAmbientLight(Ogre::ColourValue(1,1,1));
	} else if (_sceneManager){
		_sceneManager->setAmbientLight(Ogre::ColourValue(0,0,0));
	}

}

void NavigationInfo::initialise(World& world) {
	if (_init) {
		return;
	}
	_sceneManager = world.sceneManager();

	_light->initialise(world);
	headlight(_headlight);

	BindableNode<NavigationInfo>::initialise(world);

	_init = true;
}

const bool& NavigationInfo::headlight() const {
    return _headlight;
}

DirectionalLight* NavigationInfo::getLight(){
	return _light.get();
}

void NavigationInfo::onBound(Scene& scene) {
	auto vp = scene.bound<Viewpoint>();
	if (vp) {
		getLight()->attachTo(vp->getNode());
	}
}

void NavigationInfo::onUnbound(Scene& scene) {
	getLight()->detachFromParent();
}
