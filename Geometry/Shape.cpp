/*
 * Shape.cpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#include <Geometry/Shape.h>

#include <iostream>

#include <OgreLogManager.h>
#include <OgreTechnique.h>
#include <OgreMesh.h>
#include <OgreMeshManager.h>

#include <Geometry/Geometry.h>
#include <Appearance/Appearance.h>
#include <core/World.h>

using namespace X3D;

void Shape::id(const std::string& id) {
	_name = id;
	if (Ogre::StringUtil::endsWith(_name, ".mesh")) {
		_isMesh = true;
	}
}

void Shape::setGeometry(const std::shared_ptr<Geometry>& geo) {
	_geometry = geo;
}

void Shape::setAppearance(const std::shared_ptr<Appearance>& appearance) {
    _appearance = appearance;
}

void Shape::loadingComplete(Ogre::Resource* resource) {
    auto mat = static_cast<Ogre::Material*>(resource);
    auto pass = mat->getBestTechnique()->getPass(0);

    if(not _geometry->solid()) {
        pass->setCullingMode(Ogre::CULL_NONE);
    } else if(not _geometry->ccw()) {
        /**
         * Ogre and X3D/ OpenGL disagree here.
         * Ogre CW = X3D CCW
         * Ogre CCW = X3D CW
         */
        pass->setCullingMode(Ogre::CULL_ANTICLOCKWISE);
    }

    if(_geometry->hasVertexColors()) {
        pass->setVertexColourTracking(Ogre::TVC_DIFFUSE);
    }
}

void Shape::initialise(World& world) {
	if (_init) {
		return;
	}

	if (_isMesh) {
		// Create Geometry out of the Mesh
		_geometry = std::make_shared<Geometry>();
		_geometry->id(_name);
		Ogre::MeshManager::getSingleton().load(_name, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
	}

    if(not _geometry) {
        // some weird exporters do this
        return;
    }

    if(not _appearance) {
	    _appearance = std::make_shared<Appearance>();
	} else if (_isMesh) {
		Ogre::LogManager::getSingleton().logMessage("Appearance of Shape '"+_name+"' is ignored", Ogre::LML_NORMAL);
	}

    _appearance->initialise(world);
    _appearance->create();
    _appearance->addListener(this);

    _geometry->create();

    auto manager = world.sceneManager();

    for(Ogre::SceneNode* p : _parents) {
        Ogre::Entity* entity = manager->createEntity(_geometry->name());
        if (not _isMesh) {
			entity->setMaterialName(_appearance->name(), "X3D");
		}

        if(not _geometry->animation().empty()) {
            // enable and register animation
            auto anim = entity->getAnimationState(_geometry->animation());
            _geometry->configureAnimation(anim);
        }

        p->attachObject(entity);
    }

    if (not _init) {
    	_init = true;
    }
}

Shape::~Shape() {
    if(_appearance) {
        _appearance->removeListener(this);
    }
}

void Shape::attachTo(Ogre::SceneNode* node) {
    _parents.push_back(node);
}

void Shape::detachFrom(Ogre::SceneNode* node) {
    // FIXME implement
    //Ogre::Entity* entity = _entitymap[node->getName()];
    //node->detachObject(entity);
    std::cerr << "FIXME: Shape does not detach Ogre::Entity correctly from parent" << std::endl;
    _parents.remove(node);
}
