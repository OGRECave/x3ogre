/*
 * ComposedShader.cpp
 *
 *  Created on: 26.03.2014
 *      Author: baudenri
 */

#include <Appearance/ComposedShader.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>


using namespace X3D;

void ComposedShader::addChild(const std::shared_ptr<Node>& obj) {

	std::shared_ptr<ShaderPart> sp = std::dynamic_pointer_cast<ShaderPart>(obj);
	if (sp) {
		_parts.push_back(sp);
		return;
	}

	std::shared_ptr<Field> field = std::dynamic_pointer_cast<Field>(obj);
	if (field) {
		_parameters.push_back(field);
		return;
	}
}

void ComposedShader::activate(const bool& activate) {
	_activate = activate;
}

void ComposedShader::language(const std::string& language) {
	_language = language;
}


void ComposedShader::apply(Ogre::Pass* pass) {
	for (std::shared_ptr<ShaderPart> sPart : _parts) {
		sPart->init(_language);
		sPart->apply(pass);
	}

	Ogre::GpuProgramParametersSharedPtr fragment, vertex, compute;

	if (pass->hasFragmentProgram()) {
		fragment = pass->getFragmentProgramParameters();
		fragment->setIgnoreMissingParams(true);
	}

	if (pass->hasVertexProgram()) {
		vertex = pass->getVertexProgramParameters();
		vertex->setIgnoreMissingParams(true);

		//X3D Standard Parameter
		vertex->setNamedAutoConstant("model",
					Ogre::GpuProgramParameters::ACT_WORLD_MATRIX, 0);
		vertex->setNamedAutoConstant("view",
					Ogre::GpuProgramParameters::ACT_VIEW_MATRIX, 0);
		vertex->setNamedAutoConstant("projection",
					Ogre::GpuProgramParameters::ACT_PROJECTION_MATRIX, 0);
		vertex->setNamedAutoConstant("modelView",
					Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX, 0);
		vertex->setNamedAutoConstant("modelViewProjection",
					Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX, 0);
		vertex->setNamedAutoConstant("viewPosition",
					Ogre::GpuProgramParameters::ACT_CAMERA_POSITION, 0);
	}

	if (pass->hasComputeProgram()) {
		compute = pass->getComputeProgramParameters();
		compute->setIgnoreMissingParams(true);
	}

	for (std::shared_ptr<Field> field : _parameters) {
		field->apply(fragment.get());
		field->apply(vertex.get());
		field->apply(compute.get());
	}
}
