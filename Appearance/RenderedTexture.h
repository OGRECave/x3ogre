/*
 * RenderedTexture.h
 *
 *  Created on: 09.10.2014
 *      Author: baudenri
 */

#pragma once
#include <Appearance/Texture.h>
#include <World/Viewpoint.h>
#include <World/Background.h>

#include <OgrePass.h>
#include <OgreNameGenerator.h>
#include <OgreRenderTexture.h>
#include <OgrePixelFormat.h>

namespace X3D {

class RenderedTexture: public Texture {

	std::shared_ptr<Viewpoint> _viewpoint = nullptr;
	std::shared_ptr<Background> _background = nullptr;
	std::vector<int> _dimensions = std::vector<int>{128, 128, 4, 1, 1};
	Ogre::RenderTexture* _renderTexture = nullptr;
	unsigned int _numMipmaps = 0;
	std::string _update="none";
	int _width = 128;
	int _height = 128;
	Ogre::PixelFormat _pixelFormat = Ogre::PF_R8G8B8A8;
	int _ZOrder = 50;

	static Ogre::NameGenerator _nameGenerator;
	std::string _textureName;

	World* _world = nullptr;
	Ogre::Pass* _pass = nullptr;
	Ogre::Viewport* _rttViewport = nullptr;
public:

	void viewpoint(const std::shared_ptr<Viewpoint>& viewpoint);
	void background(const std::shared_ptr<Background>& background);
	void dimensions(const std::vector<int>& dimensions);
	void update(const std::string& update);

	const std::shared_ptr<Viewpoint>& viewpoint() const;
	const std::shared_ptr<Background>& background() const;
	const std::vector<int>& dimensions() const;
	const std::string& update() const;

	/**
	 * Initializes the Rendered Texture and its Children
	 * @param world
	 */
	void initialise(World& world) override;

	/**
	 * Loads texture and modifies it for later use
	 * @param pass Pass the texture is linked to
	 */
	void apply(Ogre::Pass* pass) override;
};

}
