/*
 * ComposedShader.h
 *
 *  Created on: 26.03.2014
 *      Author: baudenri
 */

#pragma once

#include <Appearance/ShaderPart.h>
#include <Appearance/ImageTexture.h>
#include <Appearance/Field.h>
#include <core/Node.h>
#include <OgrePass.h>

namespace X3D {

class ComposedShader: public Node {

	bool _activate = true;
	std::string _language;

	std::vector<std::shared_ptr<ShaderPart>> _parts;
	std::vector<std::shared_ptr<Field>> _parameters;

public:

	/**
	 * Adds a child node (ShaderPart)
	 * @param obj ShaderPart
	 */
	void addChild(const std::shared_ptr<Node>& obj);

	/**
	 * Forces the shader to activate the contained objects
	 * @param activate
	 */
	void activate(const bool& activate);

	/**
	 * Specified the shading language
	 * @param language
	 */
	void language(const std::string& language);


	/**
	 * Crates the Shader and formulates the material
	 * @param Pass
	 */
	void apply(Ogre::Pass* pass);
};


}
