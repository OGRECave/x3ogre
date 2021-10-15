/*
 * ShaderPart.cpp
 *
 *  Created on: 26.03.2014
 *      Author: baudenri
 */

#include <Appearance/ShaderPart.h>
#include <OgrePass.h>
#include <OgreException.h>

using namespace X3D;

Ogre::NameGenerator ShaderPart::_nameGenerator("X3D.ShaderPart");

void ShaderPart::id(const std::string& id) {
    _shaderName = id;
}

void ShaderPart::type(const std::string& type) {
	_type = type;
}

void ShaderPart::url(const std::string& url) {
	_url = url;
}

const std::string& ShaderPart::type() const {
	return _type;
}

const std::string& ShaderPart::url() const {
	return _url;
}

void ShaderPart::init(const std::string& language) {
	if (_init) {
		return;
	}

	if(_shaderName.empty()) {
		_shaderName = _nameGenerator.generate();
	}

	_language = language;

	// Determine GpuProgramType
	if (Ogre::StringUtil::endsWith(_type, "fragment")) {
		_programType = Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM;
	} else if (Ogre::StringUtil::endsWith(_type, "vertex")) {
		_programType = Ogre::GpuProgramType::GPT_VERTEX_PROGRAM;
	} else if (Ogre::StringUtil::endsWith(_type, "compute")) {
		_programType = Ogre::GpuProgramType::GPT_COMPUTE_PROGRAM;
	} else {
		// If the ShaderType is not Specified: Throw Exception
		std::string errMsg = "No such Shader-Type found: ";
		throw Ogre::Exception(1, errMsg.append(type()),
				"ShaderPart::initialize()");
	}

	// Create and Load GpuProgram
	Ogre::HighLevelGpuProgramPtr program =
			Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
					_shaderName, "X3D", _language, _programType);
	program->setSourceFile(_url);
	program->load();

	if (not program->isSupported()) {
		std::stringstream ss;
		ss << type() << " Shader not supported: " << _url;
		throw Ogre::Exception(1, ss.str(), "ShaderPart::initialize()");
	}

	if (not _init) {
		_init = true;
	}
}

void ShaderPart::apply(Ogre::Pass* pass) {
	switch (_programType) {
	case Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM:
		pass->setFragmentProgram(_shaderName);
		break;
	case Ogre::GpuProgramType::GPT_VERTEX_PROGRAM:
		pass->setVertexProgram(_shaderName);
		break;
	case Ogre::GpuProgramType::GPT_COMPUTE_PROGRAM:
		pass->setComputeProgram(_shaderName);
		break;
	default:
		throw Ogre::Exception(1, "Shader not supported: "+_url, "ShaderPart::apply()");
		break;
	}
}
