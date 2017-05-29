/*
 * RenderedTexture.cpp
 *
 *  Created on: 09.10.2014
 *      Author: baudenri
 */

#include <Appearance/RenderedTexture.h>
#include <core/World.h>

#include <OgreTextureManager.h>
#include <OgreResourceGroupManager.h>
#include <OgreViewport.h>
#include <OgreHardwarePixelBuffer.h>

using namespace X3D;

Ogre::NameGenerator RenderedTexture::_nameGenerator("RenderedTexture_");

void RenderedTexture::viewpoint(const std::shared_ptr<Viewpoint>& viewpoint) {
	_viewpoint = viewpoint;

	if (_viewpoint && _world) {
		_viewpoint->initialise(*_world);
		_renderTexture->removeViewport(_ZOrder);
		_rttViewport = _renderTexture->addViewport(_viewpoint->getCamera(),
				_ZOrder, 0.00f, 0.00f, 1.0f, 1.0f);
        _rttViewport->setOverlaysEnabled(false);
		_rttViewport->setBackgroundColour(_background->skyColor());
	}
}
void RenderedTexture::background(const std::shared_ptr<Background>& background) {
	_background = background;
	if (_rttViewport && _background) {
		_rttViewport->setBackgroundColour(_background->skyColor());
	}
}
void RenderedTexture::dimensions(const std::vector<int>& dimensions) {
	_dimensions = dimensions;

	// Apply default values if not present
	if (_dimensions.size()<2) {
		_dimensions = {128,128,4,1,1};
	} else if (_dimensions.size()<3) {
		_dimensions.push_back(4);
	}

	// Parse Dimensionality
	_width = _dimensions[0];
	_height = _dimensions[1];

	// Parse Pixel Format
	switch (_dimensions[2]) {
	case(3):
		_pixelFormat = Ogre::PF_R8G8B8;
		break;
	case(4):
	default:
		_pixelFormat = Ogre::PF_R8G8B8A8;
		break;
	}

	// Parse remaining values
	//	_MRTs = _dimensions[3];
	//	_NYI = _dimensions[4];
}
void RenderedTexture::update(const std::string& update) {
	_update = update;

	if (_world && _rttViewport) {
		if (stricmp(_update.c_str(), "always") == 0) {
			_renderTexture->setAutoUpdated(true);
			_rttViewport->setAutoUpdated(true);
		} else {
			_renderTexture->setAutoUpdated(false);
			_rttViewport->setAutoUpdated(false);
		}
	}
}

const std::shared_ptr<Viewpoint>& RenderedTexture::viewpoint() const {
	return _viewpoint;
}
const std::shared_ptr<Background>& RenderedTexture::background() const {
	return _background;
}
const std::vector<int>& RenderedTexture::dimensions() const {
	return _dimensions;
}
const std::string& RenderedTexture::update() const {
	return _update;
}

void RenderedTexture::initialise(World& world) {
	if (_init) {
		return;
	}

	if (_textureName.empty()) {
		_textureName = _nameGenerator.generate();
	}

	_world = &world;

	// Create Texture
	bool gammaCorrection = false;
	unsigned int antiAliasing = 0;

	Ogre::TexturePtr textureWithRtt = Ogre::TextureManager::getSingleton().createManual(
			_textureName, "X3D",
			Ogre::TEX_TYPE_2D, _width, _height, _numMipmaps,
			_pixelFormat, Ogre::TU_RENDERTARGET, 0, gammaCorrection, antiAliasing);

	// Put Render Texture into Texture
	_renderTexture = textureWithRtt->getBuffer()->getRenderTarget();

	// Initialize RenderTexture
	viewpoint(_viewpoint);
	update(_update);

	if (_pass) {
		apply(_pass);
	}

	_init = true;
}

/**
 * Loads texture and modifies it for later use
 * @param pass Pass the texture is linked to
 */
void RenderedTexture::apply(Ogre::Pass* pass) {
	_pass = pass;

	// Apply Texture to Pass
	if (_renderTexture){
		Ogre::TextureUnitState* textureUnitState;
		if(_pass->getNumTextureUnitStates() == 0) {
			textureUnitState = _pass->createTextureUnitState();
		} else {
			textureUnitState = _pass->getTextureUnitState(0);
		}
		textureUnitState->setTextureName(_textureName);
		textureUnitState->setNumMipmaps(_numMipmaps);
		textureUnitState->setTextureFiltering(Ogre::TFO_BILINEAR);
		textureUnitState->setTextureUScale(-1);
	}
}
