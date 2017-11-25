/*
 * ImageBackground.cpp
 *
 *  Created on: 08.09.2014
 *      Author: baudenri
 */

#include "PolygonBackground.h"
#include <World/Scene.h>

#include <OgreMaterialManager.h>
#include <OgrePass.h>
#include <OgreRenderQueue.h>
#include <OgreAxisAlignedBox.h>
#include <OgreSceneNode.h>
#include <OgreTechnique.h>
#include <OgreLogManager.h>

using namespace X3D;

const std::string PolygonBackground::_sceneNodeName = "X3D_PolygonBackground_SceneNode";

void PolygonBackground::initialise(World& world) {
	if (_init) {
		return;
	}
	if (not _appearance) {
		// If no appearance present, ImageBackground cann't be initialised.
		return;
	}

	// Create Appearance for Polygon background
	_appearance->initialise(world);
	_appearance->create();

	// Create background rectangle covering the whole screen
	_rect.reset(new Ogre::Rectangle2D(true));
	_rect->setCorners(-1, 1, 1, -1);
	_rect->setMaterial(_appearance->getOgreMaterial());

	// Render the background before everything else
	_rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

	// Render everything even if background is closer to camera than other objects
	_appearance->pass()->setDepthCheckEnabled(false);
	_appearance->pass()->setDepthWriteEnabled(false);

	// Apply texCoordinates
	if (_appearance->getTexture()) {

		// Set Lighting disabled only for Texture Background
		// Otherwise the pass will not be rendered which results in a white background
		_appearance->pass()->setLightingEnabled(false);

		if (_texCoords.size()==4) {
			_rect->setUVs(Ogre::Vector2(_texCoords[3].x,_texCoords[3].y),
					Ogre::Vector2(_texCoords[0].x,_texCoords[0].y),
					Ogre::Vector2(_texCoords[2].x,_texCoords[2].y),
					Ogre::Vector2(_texCoords[1].x,_texCoords[1].y) );
		} else {
			if (not _texCoords.empty()) {
			    Ogre::LogManager::getSingleton().logMessage("PolygonBackground: Texture Coordinates have wrong size", Ogre::LML_NORMAL);
			}
			_rect->setUVs(Ogre::Vector2(0,1),Ogre::Vector2(0,0), Ogre::Vector2(1,1), Ogre::Vector2(1,0));
		}
	}

	// Use infinite AAB to always stay visible.
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	_rect->setBoundingBox(aabInf);

	// Attach background to the scene
	_root = world.sceneManager()->getRootSceneNode();

	BindableNode<PolygonBackground>::initialise(world);

	_init = true;
}

void PolygonBackground::texCoords(const std::vector<Ogre::Vector3>& textureCoords) {
	_texCoords = textureCoords;
}

void PolygonBackground::setAppearance(const std::shared_ptr<Appearance>& appearance) {
	_appearance = appearance;
}

void PolygonBackground::onBound(Scene& scene) {
	if (_root) _root->attachObject(_rect.get());
}

void PolygonBackground::onUnbound(Scene& scene) {
	if (_root) _root->detachObject(_rect.get());
}

