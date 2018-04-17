/*
 * Scene.c
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#include <World/Scene.h>

#include <OgreControllerManager.h>

#include <World/Background.h>
#include <Geometry/Shape.h>
#include <World/WorldInfo.h>

using namespace X3D;

Scene::~Scene() {
    Ogre::ControllerManager::getSingleton().clearControllers();
}

WorldInfo* Scene::worldInfo() {
	return _worldInfo;
}

void Scene::addChild(const std::shared_ptr<Node>& obj) {
	Group::addChild(obj);

	// check for some Scene specific child nodes
	auto wi = dynamic_cast<WorldInfo*>(obj.get());
	if (wi) {
		_worldInfo = wi;
		return;
	}
}

void Scene::setViewport(Ogre::Viewport* viewport) {
    _viewport = viewport;

    if(bound<Background>()) {
    	bound<Background>()->apply(_viewport);
    }
}

void Scene::registerNode(const std::shared_ptr<Node>& node, const std::string& name) {
    if(nodeExists(name)) {
        throw std::runtime_error("duplicate id='"+name+"' in World::registerNode");
    }

    _namedNodes[name] = node;
}

bool Scene::nodeExists(const std::string& name) const {
    return _namedNodes.find(name) != _namedNodes.end();
}

Node* Scene::getNode(const std::string& name) const {
    return _shareNode(name).get();
}

const std::shared_ptr<Node>& Scene::_shareNode(const std::string& name) const {
    auto it = _namedNodes.find(name);

    if(it == _namedNodes.end()) {
        throw std::runtime_error("no node with name '"+name+"' found");
    }

    return it->second;
}


