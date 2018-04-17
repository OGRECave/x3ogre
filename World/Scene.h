/*
 * Scene.hpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <typeindex>
#include <map>

#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreNode.h>
#include <OgreViewport.h>

#include <core/Node.h>
#include <World/Group.h>

namespace X3D
{
template<typename T> class BindableNode;
class Viewpoint;
class Background;
class PolygonBackground;
class NavigationInfo;
class WorldInfo;

/**
 * The Scene-Class contains several Information about the Scene:
 * 		Background,
 * 		NavigationInfo,
 * 		Viewpoint,
 * 		WorldInfo
 * It also contains pointer to all Transforms and Shapes of the Scene.
 * @see Transform
 * @see Shape
 * @see World
 */
class Scene: public Group {
	WorldInfo* _worldInfo = nullptr;

	std::map<std::type_index, Node*> bindables;

    std::map<std::string, std::shared_ptr<Node>> _namedNodes;

	Ogre::Viewport* _viewport = nullptr;
public:
	~Scene();

	/**
	 * Adds a Object to the Scene. It will fill the corresponding position
	 * (e.g. if its a Background-Object it will be fit in as Background).
	 * @param obj Object to add to the Scene
	 */
	void addChild(const std::shared_ptr<Node>& obj);

	/**
	 * Sets the Background-Colour of the viewport to the one specified in Background
	 * @param viewport Ogre::Viewport of the Scene
	 */
	void setViewport(Ogre::Viewport* viewport);

	/**
	 * Returns the current viewport of the scene
	 */
	Ogre::Viewport* getViewport() {
		return _viewport;
	}

    /**
     * register a name for a node (as in DEF in X3D)
     * @param node
     * @param name
     */
    void registerNode(const std::shared_ptr<Node>& node, const std::string& name);

    bool nodeExists(const std::string& name) const;

    /**
     * get a node by name. has to be registered before
     * @param name
     * @return
     */
    Node* getNode(const std::string& name) const;

    /**
     * retrieve named node for sharing with other nodes
     * @param name
     * @return
     */
    const std::shared_ptr<Node>& _shareNode(const std::string& name) const;

	WorldInfo* worldInfo();

	Ogre::SceneNode* getSceneNode() {
	    return _groupParent;
	}

	template<typename T>
	T* bound() {
	    return static_cast<T*>(bindables[std::type_index(typeid(T))]);
	}

	template<typename T>
	void activateBindable(BindableNode<T>* n) {
	    auto old = bound<T>();
	    if(old) {
	        old->onUnbound(*this);
	    }
	    bindables[std::type_index(typeid(T))] = n;
	    n->onBound(*this);
	}

};

}
