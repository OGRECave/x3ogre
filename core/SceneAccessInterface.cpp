/*
 * Viewer.cpp
 *
 *  Created on: 24.03.2014
 *      Author: parojtbe
 */

#include <Parser/X3DFileManager.h>
#include <core/SceneAccessInterface.h>

#include <World/Viewpoint.h>

#include <reflection/db.h>

#include <OgreBuildSettings.h> // to get OGRE_STATIC_LIB define
#include <OgreConfig.h>
#include <OgreRoot.h>

#include <OgreMaterialManager.h>
#include <OgreShaderRenderState.h>
#include <OgreShaderExPerPixelLighting.h>
#include <OgreTechnique.h>
#include <OgreSGTechniqueResolverListener.h>
#include <OgreLogManager.h>

#include <Parser/X3DFile.h>

#include <sstream>

#ifdef WIN32
#define __func__ __FUNCTION__
#endif

namespace {
Ogre::AxisAlignedBox getWorldBoundingBox(Ogre::SceneNode* node, Ogre::SceneNode* leftOut = nullptr) {
    Ogre::AxisAlignedBox bbox;
    bbox.setNull();

    // Update Bounding Box
    node->_updateBounds();
    bbox = node->_getWorldAABB();

    std::vector<Ogre::SceneNode*> nodes;
    nodes.push_back(node);

    while (not nodes.empty()) {
        // Walk through Child nodes
        Ogre::SceneNode* node = nodes.back();
        nodes.pop_back();

        node->_updateBounds();
        bbox.merge(node->_getWorldAABB());

        Ogre::SceneNode::ChildNodeIterator j = node->getChildIterator();
        while (j.hasMoreElements()) {
            auto nextNode = j.getNext();
            // Add Child Node only if it isn't marked as leftOut
            if (nextNode != leftOut) {
                nodes.push_back(static_cast<Ogre::SceneNode*>(nextNode));
            }
        }
    }
    return bbox;
}

}

namespace X3D {
SceneAccessInterface::SceneAccessInterface(Ogre::Root* root) : mRoot(root) {
    root->addFrameListener(this);
    mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

    // Set default point size to 3
    Ogre::MaterialManager::getSingleton().getDefaultSettings()->setPointSize(3);

    _x3dFM.reset(new X3DFileManager);
}

void SceneAccessInterface::forcePerPixelLighting() {
    using namespace Ogre::RTShader;
    RenderState* globalRS = mShaderGenerator->getRenderState(ShaderGenerator::DEFAULT_SCHEME_NAME);

    // Add PerPixelLighting. Notes:
    // - PerPixelLightingFactory already exists (Added in ShaderGenerator::initialize())
    // - globalRS does not need to be reset as we do this before loading anything
    globalRS->addTemplateSubRenderState(mShaderGenerator->createSubRenderState(PerPixelLighting::Type));
}

void SceneAccessInterface::setNodeAttribute(const std::string& nodeName, const std::string& fieldName, const std::string& fieldValue, bool buffer) {
    if (buffer) {
        std::lock_guard<std::mutex> l(_updateMutex);
        _updates[std::pair<std::string,std::string>(nodeName,fieldName)]=fieldValue;
    } else {
        
        auto obj = _scene->getNode(nodeName);
        auto ti = reflection::getTypeInfo(*obj);
        ti->callMember(obj, fieldName, fieldValue);
    }
}

std::string SceneAccessInterface::getNodeAttribute(const std::string& nodeName, const std::string& fieldName) {
    Node* obj = _scene->getNode(nodeName);
    reflection::TypeInfoCommon* ti = reflection::getTypeInfo(*obj);
    return ti->callMemberString(obj, fieldName);
}

void SceneAccessInterface::clearWorld() {
	if (_scene) {
        // Reset World
	    mShaderGenerator->removeSceneManager(_sceneManager);
	    mRoot->destroySceneManager(_sceneManager);
	    _scene.reset();
		_x3dFM->removeAll();

		// Remove current basepath from FileSystem
		Ogre::ResourceGroupManager::getSingleton().removeResourceLocation(_basePath, "X3D");
        Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup("X3D");

        if (mShaderGenerator) {
            mShaderGenerator->removeAllShaderBasedTechniques();
            mShaderGenerator->flushShaderCache();
        }
	}
}

void SceneAccessInterface::loadURL(const std::string& url) {
	if (not mShaderGenerator) {
        throw std::runtime_error(std::string("RTSS must be initialized before") + __func__);
    }

	// Reset values to default for reloading a second URL
	if (init) {
		clearWorld();
		init = false;
	}

    // RTSS has to be initialized before loading of the scripts
    // 		but AFTER creation of the rendering Window
    // 		Has to be implemented in the concrete viewer

    // add X3D path to Ogre resources
    Ogre::String filename, basepath;
    Ogre::StringUtil::splitFilename(url, filename, basepath);

    if (!basepath.empty() && !Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(basepath,"X3D")) {
        // Counts for android, since APK located files (crash if basepath is empty)
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(basepath, "FileSystem", "X3D", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("X3D");
    }
#ifdef ANDROID
    else {
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/", "APKFileSystem", "X3D");
    }
#endif

    _camTgt = Ogre::Vector3::ZERO;
    _fileName = filename;
    _basePath = basepath;

    _sceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC);
    mShaderGenerator->addSceneManager(_sceneManager);

    _scene.reset(new Scene);
    _scene->attachTo(_sceneManager->getRootSceneNode());
    _x3dFM->load(filename, "X3D", _scene);

    try {
        World world = {_sceneManager, _scene.get()};
    	_scene->initialiseAndFill(world);
    } catch (Ogre::Exception& e) {
    	clearWorld();
    	throw e;
    }

    showWorld();
    init = true;
}

void SceneAccessInterface::setWindow(Ogre::RenderWindow* window) {
    // Remove old Viewport if present
    if (window->hasViewportWithZOrder(0)) {
    	window->removeViewport(0);
    }

    // By default create a viewport with full window size
    auto vp = _scene->bound<Viewpoint>();
    auto fullscreenViewport = window->addViewport(vp->getCamera());
    _scene->setViewport(fullscreenViewport);
	vp->onBound(*_scene); // notify of viewport
}

SceneAccessInterface::~SceneAccessInterface() {
    clearWorld();
    if(_x3dFM) {
        Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(_x3dFM.get());
    }
}

void SceneAccessInterface::setDefaultAnisotropy(uint32_t lvl) {
    auto& matmgr = Ogre::MaterialManager::getSingleton();
    matmgr.setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    matmgr.setDefaultAnisotropy(lvl);
}


bool SceneAccessInterface::frameStarted(const Ogre::FrameEvent& evt) {
    _updateMutex.lock();
    for (const auto& up : _updates) {
        setNodeAttribute(up.first.first, up.first.second, up.second, false);
    }
    _updates.clear();
    _updateMutex.unlock();

    /*
     * Updates the SceneNodes which have been modified during update process.
     * This is necessary because calling needUpdate() of a DEF-Transform during
     * _update process sets the variable mParentNotified of the parents of the
     * USE-Transform such that its true even if the node isn't inserted in its
     * parents update process.
     */
    Ogre::Node::processQueuedUpdates();

    return true;
}


void SceneAccessInterface::switchDebugDrawing() {
	_doDebugDrawing = !_doDebugDrawing;
	_sceneManager->setDisplaySceneNodes(_doDebugDrawing);
	_sceneManager->showBoundingBoxes(_doDebugDrawing);
}

void SceneAccessInterface::addTranslation(int dx, int dy) {
    auto cam = _scene->bound<Viewpoint>()->getNode();
    auto dist = (cam->getPosition() - _camTgt).length();
    _camTgt += cam->getOrientation() * Ogre::Vector3(0.0025 * dx*dist, 0.0025 * dy*dist, 0);
    cam->setPosition(_camTgt);
    cam->translate(Ogre::Vector3(0, 0, dist), Ogre::Node::TS_LOCAL);
}

void SceneAccessInterface::addRotation(int dx, int dy) {
    auto cam = _scene->bound<Viewpoint>()->getNode();
    auto dist = (cam->getPosition() - _camTgt).length();

    cam->setPosition(_camTgt);

    cam->yaw(Ogre::Degree(-dx * 0.25f));
    cam->pitch(Ogre::Degree(-dy * 0.25f));
    cam->translate(Ogre::Vector3(0, 0, dist), Ogre::Node::TS_LOCAL);
}

void SceneAccessInterface::zoom(float relFactor) {
    auto cam = _scene->bound<Viewpoint>()->getNode();
    auto dist = cam->getPosition().length();
    relFactor = relFactor > 0 ? -1 : 1;
    cam->translate(Ogre::Vector3(0, 0, relFactor * 0.08f * dist), Ogre::Node::TS_LOCAL);
}

void SceneAccessInterface::showWorld() {
    auto cam = _scene->bound<Viewpoint>()->getNode();
    auto bbox = getWorldBoundingBox(_sceneManager->getRootSceneNode(), cam);
    _camTgt = Ogre::Vector3::ZERO;
    cam->setPosition(_camTgt);
    cam->translate(Ogre::Vector3(0, 0, bbox.getSize().length()), Ogre::Node::TS_LOCAL);
}

} /* namespace X3D */
