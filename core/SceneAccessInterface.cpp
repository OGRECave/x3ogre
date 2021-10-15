/*
 * Viewer.cpp
 *
 *  Created on: 24.03.2014
 *      Author: parojtbe
 */

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

        for (auto nextNode : node->getChildren()) {
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
SceneAccessInterface::SceneAccessInterface(Ogre::SceneNode* rootNode) {
    _rootNode = rootNode;
    Ogre::Root::getSingleton().addFrameListener(this);
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

void SceneAccessInterface::addEssentialNodes()
{
    auto _scene = scene();
    World world = {_scene};

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

SceneAccessInterface::~SceneAccessInterface() {
    Ogre::Root::getSingleton().removeFrameListener(this);
}

bool SceneAccessInterface::frameStarted(const Ogre::FrameEvent& evt) {
    _updateMutex.lock();
    for (const auto& up : _updates) {
        setNodeAttribute(up.first.first, up.first.second, up.second, false);
    }
    _updates.clear();
    _updateMutex.unlock();

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
