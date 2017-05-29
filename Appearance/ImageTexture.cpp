/*
 * ImageTexture2D.cpp
 *
 *  Created on: 20.12.2013
 *      Author: baudenri_local
 */

#include "ImageTexture.h"

#include <OgreShaderGenerator.h>
#include <OgreShaderScriptTranslator.h>
#include <OgreResourceGroupManager.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgreMaterialManager.h>
#include <OgreLogManager.h>

using namespace X3D;
using namespace std;

void ImageTexture::apply(Ogre::Pass* pass) {
	/*
	 *  Apply works only for a single pass
	 *  (so you can't use 'DEF' and 'USE'
	 *  for ImageTextures. If you would like to
	 *  do so, the _pass has to be a vector of Passes
	 *  and in the url setter all _passes
	 *  need to be updated).
	 */
	_pass = pass;

	if(_pass->getNumTextureUnitStates() == 0) {
	    _texUnit = _pass->createTextureUnitState();
	} else {
	    _texUnit = _pass->getTextureUnitState(0);
	}

    _texUnit->setTextureVScale(-1);

    // Don't use Mipmaps
    _texUnit->setNumMipmaps(0);

    // Updates URL if its set
	if(not _url.empty()) {
		url({_url});
		return;
	}

    if(not _rtss.empty()) {
        configureSubRenderStates();
    }
}

void ImageTexture::addRTShaderSystem(const std::shared_ptr<RTShaderSystem>& rtss) {
	_rtss.push_back(rtss);
}

const std::string& ImageTexture::url() const {
    return _url;
}

void ImageTexture::url(const std::vector<std::string>& url) {
    _url = url.front();

    std::string group = "X3D";

    try {
        Ogre::TextureManager::getSingleton().load(_url, group, Ogre::TEX_TYPE_2D);
    } catch(std::exception& e) {
        Ogre::LogManager::getSingleton().logMessage("could not load '"+_url+"': "+e.what(), Ogre::LML_NORMAL);
        return;
    }

    if(_texUnit) {
        _texUnit->setTextureName(_url);
    }
}


void ImageTexture::configureSubRenderStates(){
    Ogre::ScriptCompiler compiler;
    Ogre::RTShader::SGScriptTranslator translator;

    Ogre::ObjectAbstractNode parent(nullptr);
    parent.cls = "texture_unit";
    parent.context = _texUnit;

    Ogre::SharedPtr<Ogre::ObjectAbstractNode> obj(new Ogre::ObjectAbstractNode(&parent));

    for (auto& snippet: _rtss) {
        obj->children.push_back(snippet->node());
    }

    translator.translate(&compiler, obj);
}
