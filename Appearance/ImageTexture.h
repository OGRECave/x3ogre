/*
 * ImageTexture2D.h
 *
 *  Created on: 20.12.2013
 *      Author: baudenri_local
 */

#pragma once
#include <Appearance/Texture.h>
#include <Appearance/RTShaderSystem.h>
#include <OgreTexture.h>
#include <OgreTextureManager.h>
#include <OgreImage.h>
#include <OgreString.h>
#include <OgrePass.h>

namespace X3D {

/**
 * Class for loading Textures and adding it to the ResourceManager
 */
class ImageTexture: public Texture {
    std::string _url;
    std::vector<std::shared_ptr<RTShaderSystem>> _rtss;

    Ogre::TextureUnitState* _texUnit = nullptr;
    Ogre::Pass* _pass = nullptr;

    void configureSubRenderStates();

public:
    /**
	 * Adds a RTShaderSystem part
	 * @param shader
	 */
	void addRTShaderSystem(const std::shared_ptr<RTShaderSystem>& rtss);


    /**
     * Returns the (relative) Path of the Texture-File
     * @return relative Path of TextureFile
     */
    const std::string& url() const;

    /**
     * Adds Texture File to ResourceManager for later use
     * @param fileName Name and (realtive) Path of the Texture-File
     */
    void url(const std::vector<std::string>& fileName);

    /**
	 * Loads texture and modifies it for later use
	 * @param pass Pass the texture is linked to
	 */
    void apply(Ogre::Pass* pass) override;
};

}
