/*
 * Appearance.cpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#include "Appearance.h"

#include <memory>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgreShaderGenerator.h>
#include <OgrePass.h>
#include <OgreShaderScriptTranslator.h>

using namespace X3D;

Ogre::NameGenerator Appearance::_nameGenerator("Appearance_");

void Appearance::addRTShaderSystem(const std::shared_ptr<RTShaderSystem>& rtss) {
	_rtss.push_back(rtss);
}

void Appearance::setShader(const std::shared_ptr<ComposedShader>& shader) {
	_shader = shader;
}

void Appearance::setMaterial(const std::shared_ptr<Material>& material) {
    _material = material;
}

const Ogre::String& Appearance::name() const {
    return _name;
}


void Appearance::setTexture(const std::shared_ptr<Texture>& texture) {
    _texture = texture;
    if (_texture && _material && (_rtss.empty() || !_shader)) {
		// when texture is set, the material color should be ignored
		// But if Shader is set too, the material properties should not be changed
		_material->diffuseColor(Ogre::ColourValue::White);
		// Additionally only the transparency of the image (and not of the material)
		// should be taken.
		_material->transparency(0);
	}
}

Texture* Appearance::getTexture() {
    return _texture.get();
}

Material* Appearance::getMaterial() {
	return _material.get();
}

void Appearance::phantom(const bool& phantom) {
	_phantom = phantom;
}

void Appearance::id(const std::string& id) {
    _name = id;
}

void Appearance::loadingComplete(Ogre::Resource* resource) {
    _pass = _mat->getTechniques().size() == 1 ?
            _mat->getTechnique(0)->getPass(0) : _mat->getBestTechnique()->getPass(0);

    if (_texture ) {
        if (_material and (_rtss.empty() || not _shader)) {
            // when texture is set, the material color should be ignored
            // But if Shader is set too, the material properties should not be changed
            _material->diffuseColor(Ogre::ColourValue::White);
        }
        if (_texture) {
            _texture->apply(_pass);
        }
    }

    if (_shader) {
        _shader->apply(_pass);
        _mat->compile(); // appearance needs recompile after shader were added
    } else if (_material) {
        _material->apply(_pass);
    } else {
        Material::DEFAULT.apply(_pass);
    }

    if (_phantom) {
    	_pass->setColourWriteEnabled(false);
    }
}

void Appearance::create() {
    if(_name.empty()) {
        _name = _nameGenerator.generate();
    }

    Ogre::MaterialManager& mgr = Ogre::MaterialManager::getSingleton();
    _mat = mgr.getByName(_name, "X3D");

    if(_mat) {
        // appearance was created and configured before
        return;
    }

    // Initialization of Material
    _mat = mgr.create(_name, "X3D");
    // RTSS creates shader a new based technique based on the existing one
    // make sure that it contains has the right values
    loadingComplete(_mat.get());
    _mat->addListener(this);

    if (not _rtss.empty()) {
        configureSubRenderStates(_mat);
    }
}

Appearance::~Appearance() {
    if(_mat) {
        _mat->removeListener(this);
        Ogre::MaterialManager::getSingleton().remove(_mat);
    }
}

void Appearance::configureSubRenderStates(Ogre::MaterialPtr mat) {
    Ogre::ScriptCompiler compiler;
    Ogre::RTShader::SGScriptTranslator translator;

    Ogre::ObjectAbstractNode parent(nullptr);
    parent.cls = "pass";
    parent.context = _pass;

    Ogre::SharedPtr<Ogre::ObjectAbstractNode> obj(new Ogre::ObjectAbstractNode(&parent));

    for (auto& snippet: _rtss) {
        obj->children.push_back(snippet->node());
    }

    translator.translate(&compiler, obj);
}

void Appearance::initialise(World& world) {
	if (_texture) {
		_texture->initialise(world);
	}
}

void Appearance::addListener(Ogre::Resource::Listener* listener) {
    _mat->addListener(listener);
}

void Appearance::removeListener(Ogre::Resource::Listener* listener) {
    _mat->removeListener(listener);
}
