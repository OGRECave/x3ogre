/*
 * ShaderPart.h
 *
 *  Created on: 26.03.2014
 *      Author: baudenri
 */

#pragma once

#include <core/Node.h>

#include <OgreHighLevelGpuProgram.h>
#include <OgreHighLevelGpuProgramManager.h>
#include <OgreGpuProgramParams.h>
#include <OgreNameGenerator.h>

namespace X3D {

class ShaderPart: public Node {

    static Ogre::NameGenerator _nameGenerator;

	std::string _type;
	std::string _url;
	std::string _language;
	std::string _shaderName;

	Ogre::GpuProgramType _programType = Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM;

public:


	void id(const std::string& id) override;

	/**
	 * Specified the Shader-Type ("VERTEX" or "FRAGMENT")
	 * @param type
	 */
	void type(const std::string& type);

	/**
	 * Returns the GpuProgramType of the URL
	 * @return programType
	 */
	//const Ogre::GpuProgramType& getProgramType() const;

	/**
	 * Getter for Shader-Type
	 * @return Returns the specified Shader Type
	 */
	const std::string& type() const;

	/**
	 * Specified the Adress of the shader
	 * @param url
	 */
	void url(const std::string& url);

	/**
	 * Getter for the URL/Name of the Shader
	 * @return Returns the url of the Shader File
	 */
	const std::string& url() const;

	/**
	 * Creates the shader
	 */
	void init(const std::string& language);

	/**
	 * Applies the Shader to the Pass
	 * @param Pass
	 */
	void apply(Ogre::Pass* pass);

};


}
