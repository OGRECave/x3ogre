/*
 * Viewer.cpp
 *
 *  Created on: 24.03.2014
 *      Author: parojtbe
 */

#include <Parser/X3DFileManager.h>
#include <core/SceneAccessInterface.h>

#include <World/Viewpoint.h>
#include <World/NavigationInfo.h>
#include <World/Viewpoint.h>
#include <World/Background.h>
#include <World/PolygonBackground.h>

#include <reflection/db.h>

#include <OgreBuildSettings.h> // to get OGRE_STATIC_LIB define
#include <OgreConfig.h>
#include <OgreRoot.h>

#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgreLogManager.h>

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
SceneAccessInterface::SceneAccessInterface() {
    // Set default point size to 3
    Ogre::MaterialManager::getSingleton().getDefaultSettings()->setPointSize(3);

    _x3dFM.reset(new X3DFileManager);
}

void SceneAccessInterface::setNodeAttribute(const std::string& nodeName, const std::string& fieldName, const std::string& fieldValue, bool buffer) {
    if (buffer) {
        std::lock_guard<std::mutex> l(_updateMutex);
        _updates[std::pair<std::string,std::string>(nodeName,fieldName)]=fieldValue;
    } else {
        
        auto obj = scene()->getNode(nodeName);
        auto ti = reflection::getTypeInfo(*obj);
        ti->callMember(obj, fieldName, fieldValue);
    }
}

std::string SceneAccessInterface::getNodeAttribute(const std::string& nodeName, const std::string& fieldName) {
    Node* obj = scene()->getNode(nodeName);
    reflection::TypeInfoCommon* ti = reflection::getTypeInfo(*obj);
    return ti->callMemberString(obj, fieldName);
}

void SceneAccessInterface::clearWorld() {
	if (scene()) {
        // Reset World
	    _rootNode->getUserObjectBindings().eraseUserAny("x3d_scene");

		// Remove current basepath from FileSystem
		Ogre::ResourceGroupManager::getSingleton().removeResourceLocation(_basePath, "X3D");
        Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup("X3D");
	}
}

void SceneAccessInterface::loadURL(const std::string& url, Ogre::SceneNode* rootNode) {
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
    _rootNode = rootNode;

    try {
        auto stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, "X3D");
        _x3dFM->load(stream, "X3D", _rootNode);
        addEssentialNodes();
    } catch (Ogre::Exception& e) {
    	clearWorld();
    	throw e;
    }

    init = true;
}

void SceneAccessInterface::addEssentialNodes()
{
    auto _scene = scene();
    World world = {_rootNode->getCreator(), _scene};

    auto vp = _scene->bound<Viewpoint>();
    if (not vp) {
        auto node = std::make_shared<Viewpoint>();
        vp = node.get();
        _scene->addChild(node);
        vp->initialise(world);
    }

    auto ni = _scene->bound<NavigationInfo>();
    if (not ni) {
        auto node = std::make_shared<NavigationInfo>();
        ni = node.get();
        _scene->addChild(node);
        ni->initialise(world);
    }

    auto bg = _scene->bound<Background>();
    auto pb = _scene->bound<PolygonBackground>();
    if (not bg and not pb) {
        auto node = std::make_shared<Background>();
        bg = node.get();
        _scene->addChild(node);
        bg->initialise(world);
    }

    // Add Headlight to Camera
    ni->getLight()->attachTo(vp->getNode());
}

Scene* SceneAccessInterface::scene()
{
    auto scene = Ogre::any_cast<std::shared_ptr<Scene>>(_rootNode->getUserObjectBindings().getUserAny("x3d_scene"));
    return scene.get();
}

void SceneAccessInterface::setWindow(Ogre::RenderWindow* window) {
    // Remove old Viewport if present
    if (window->hasViewportWithZOrder(0)) {
    	window->removeViewport(0);
    }

    // By default create a viewport with full window size
    auto _scene = scene();
    auto vp = _scene->bound<Viewpoint>();
    auto fullscreenViewport = window->addViewport(vp->getCamera());
    _scene->setViewport(fullscreenViewport);
	vp->onBound(*_scene); // notify of viewport
}

SceneAccessInterface::~SceneAccessInterface() {
    clearWorld();
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
	auto sceneManager = _rootNode->getCreator();
	sceneManager->setDisplaySceneNodes(_doDebugDrawing);
	sceneManager->showBoundingBoxes(_doDebugDrawing);
}

float SceneAccessInterface::getWorldSize() {
    auto cam = scene()->bound<Viewpoint>()->getNode();
    auto bbox = getWorldBoundingBox(_rootNode, cam);
    return bbox.getSize().length();
}

} /* namespace X3D */
