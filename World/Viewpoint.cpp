/*
 * Viewpoint.cpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#include <World/Viewpoint.h>
#include <core/Node.h>
#include <Geometry/Shape.h>

#include <OgreSceneNode.h>
#include <World/NavigationInfo.h>
#include <iostream>

#include <OgreCompositorManager.h>

using namespace X3D;

using namespace std;

Ogre::NameGenerator Viewpoint::_nameGenerator("X3D.Camera_");

// Variablen
void Viewpoint::position(const Ogre::Vector3& position) {
    _position = position;
    if (_cameraNode) {
        _cameraNode->setPosition(position);
    }
}
void Viewpoint::orientation(const Ogre::Quaternion& orientation) {
    _orientation = orientation;

    if (_cameraNode) {
        _cameraNode->setOrientation(orientation);
    }
}

void Viewpoint::modelviewMatrix(const Ogre::Matrix4& modelview) {
    OgreAssert(modelview.isAffine(), "The modelview matrix is not affine");

	Ogre::Vector3 pos(0,0,0), scale(0,0,0);
	Ogre::Quaternion rot(0,0,0,1);

	modelview.inverse().decomposition(pos,scale,rot);
	_cameraNode->setPosition(pos);
	_cameraNode->setOrientation(rot);
	_cameraNode->setScale(scale);
}

void Viewpoint::projectionMatrix(const Ogre::Matrix4& projection) {
	_camera->setCustomProjectionMatrix(true, projection);
}

void Viewpoint::setID(std::string id) {
    _id = id;
}
void Viewpoint::zNear(const double& zNear) {
    _zNear = zNear;
    if (_camera)
    	_camera->setNearClipDistance(_zNear);
}
void Viewpoint::zFar(const double& zFar) {
    _zFar = zFar;
    if (_camera)
    	_camera->setFarClipDistance(_zFar);
}

void Viewpoint::setAspect(float ratio) {
    _ratio = ratio;
    if (_camera != nullptr) {
        _camera->setAspectRatio(ratio);
    }
}

Ogre::Camera* Viewpoint::getCamera() {
    return _camera;
}

void Viewpoint::initialise(World& world) {
	if (_init) {
		return;
	}

    auto sceneManager = world.sceneManager();

    _camera = sceneManager->createCamera(_nameGenerator.generate());
    _camera->setNearClipDistance(_zNear);
    _camera->setFarClipDistance(_zFar);
    _camera->setAutoAspectRatio(true);

    _fovy = _camera->getFOVy();

    _cameraNode = sceneManager->createSceneNode();

    _cameraNode->attachObject(_camera);
    _cameraNode->setPosition(_position);
    _cameraNode->setOrientation(_orientation);
    attachTo(_groupParent);

	BindableNode<Viewpoint>::initialise(world);

    _init = true;
}

void Viewpoint::zoom(float relFactor) {
    Ogre::Radian fovy = _camera->getFOVy()*relFactor;
    _camera->setFOVy(std::min(fovy, Ogre::Radian(179.0/180*M_PI)));
}

void Viewpoint::resetZoom() {
	 _camera->setFOVy(_fovy);
}

void Viewpoint::attachTo(Ogre::SceneNode* node) {
    _groupParent = node;

    if (_cameraNode && _groupParent) {
         _groupParent->addChild(_cameraNode);
	}
}

void Viewpoint::detachFrom(Ogre::SceneNode* node) {
    node->removeChild(_cameraNode);
}

void Viewpoint::onBound(Scene& scene) {
    auto viewport = scene.getViewport();
    if (not viewport) {
        return;
    }

    // navigation info
	auto ni = scene.bound<NavigationInfo>();
	if (ni) {
		ni->getLight()->detachFromParent();
		ni->getLight()->attachTo(_cameraNode);
	}

    viewport->setCamera(getCamera());

    if(not _compositors.empty()) {
        std::vector<std::string> tmp;
        std::swap(tmp, _compositors); // _compositors must be empty
        applyCompositors(viewport, tmp);
    }
}

void Viewpoint::compositors(const std::vector<std::string>& names) {
    if(not _scene or _scene->bound<Viewpoint>() != this or not _scene->getViewport()) {
        _compositors = names;
        return;
    }

    applyCompositors(_scene->getViewport(), names);
}


void Viewpoint::applyCompositors(Ogre::Viewport* viewport, const std::vector<std::string>& newcomps) {
    // compositors
    auto& mgr = Ogre::CompositorManager::getSingleton();

    // disable old compositors
    for(const auto& comp : _compositors) {
        mgr.setCompositorEnabled(viewport, comp, false);
        mgr.removeCompositor(viewport, comp);
    }

    _compositors = newcomps;

    for(const auto& comp : _compositors) {
        // enable new compositor
        auto i = mgr.addCompositor(viewport, comp);
        if(not i) {
            Ogre::LogManager::getSingleton().logMessage("could not find compositor '"+comp+"'", Ogre::LML_NORMAL);
        }
        mgr.setCompositorEnabled(viewport, comp, true);
    }
}
