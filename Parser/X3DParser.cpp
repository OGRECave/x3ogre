/*
 * X3DParser.cpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#include <Parser/X3DParser.h>
#include <exception>
#include <string>

#include <reflection/db.h>

#include <World/Group.h>
#include <World/Route.h>
#include <World/Scene.h>
#include <World/Transform.h>

#include <core/BindableNode.h>

#include <Appearance/Appearance.h>

#include <World/Inline.h>

#include <OgreLogManager.h>

using namespace X3D;

void X3DParser::recurse(const pugi::xml_node& n, StackNode& parent) {
    StackNode node = parseNode(n, parent);

    // DFS traversal of Tree
    for(auto c : n.children()) {
        recurse(c, node);
    }
}

X3DParser::X3DParser(std::string xml, Node& parent, Scene& scene, const std::string& nameSpace) :
_scene(scene), _nameSpace(nameSpace)
{
    pugi::xml_document doc;
    doc.load_buffer_inplace(&xml[0], xml.size());

    reflection::alias("Route", "ROUTE");

    StackNode root{"X3DParser::parent", reflection::getTypeInfo(parent), &parent};

    recurse(doc.document_element(), root);
}

// functions for building the scene graph
StackNode X3DParser::parseNode(const pugi::xml_node& xmlnode, StackNode& parent) {
    StackNode node;
    node.tag = xmlnode.name();

    if (Ogre::StringUtil::endsWith(node.tag, "x3d")) {
        // x3d has no counterpart in our scenegraph, so just skip it
        return parent;
    }

    if (Ogre::StringUtil::endsWith(node.tag, "scene")) {
        // we create the Scene node from outside
        return parent;
    }

    //std::cout << "Tag: " << node.tag << std::endl;

    node.ti = reflection::getTypeInfo(node.tag);

    if (not node.ti) {
        Ogre::LogManager::getSingleton().logError("no such node: '" + std::string(node.tag) + "'");
        return node;
    }

    std::shared_ptr<Node> sharedNode(static_cast<Node*>(node.ti->create()));

    if(not parent.ti) {
        // got a child of an undefined xmlnode
        return node;
    }

    // make pointer point to actual node and resolve the name
    resolveDefUse(sharedNode, xmlnode);

    node.object = sharedNode.get();
    Route* route = dynamic_cast<Route*>(node.object);
    if(route) {
        parseAttributes(xmlnode, node.object, node.ti);

        // apply route to world
        route->apply(_scene, _nameSpace);

        // do not add routes to scenegraph, they are just temporaries
        return node;
    }

    try {
        const std::string& member = parent.ti->memberByArgType(node.object);
        //std::cout << parent->tag << "::" << member << "(" << tag << ")" <<  std::endl;
        parent.ti->callMember(parent.object, member, sharedNode);
    } catch (std::runtime_error& e) {
        Ogre::LogManager::getSingleton().logError(std::string(parent.tag) + "::*(" + node.tag + "): " + e.what());
    }

    parseAttributes(xmlnode, node.object, node.ti);

    return node;
}

std::string X3DParser::resolveNamespace(const std::string& name) {
    std::string qualifiedName;
    if (_nameSpace.empty()) {
        qualifiedName = name;
    } else {
        qualifiedName = _nameSpace+"."+name;
    }

    return qualifiedName;
}

void X3DParser::resolveDefUse(std::shared_ptr<Node>& object, const pugi::xml_node& node) {
    for(auto a : node.attributes()) {
        const char* name = a.name();
        const char* value = a.value();

        if (Ogre::StringUtil::endsWith("use", name)) {
            // make object point to previous instance. destroys temporary object.
            object = _scene._shareNode(resolveNamespace(value));
            break;
        }

        if (Ogre::StringUtil::endsWith("def", name) || Ogre::StringUtil::endsWith("id", name)) {
            std::string qualifiedName = resolveNamespace(value);
            _scene.registerNode(object, qualifiedName);
            object->id(qualifiedName);
            continue;
        }
    }
}

void X3DParser::parseAttributes(const pugi::xml_node& node, Node* object, reflection::TypeInfoCommon* ti) {
    for(auto a : node.attributes()) {
        const char* name = a.name();
        const char* value = a.value();

        if (Ogre::StringUtil::endsWith("def", name) || Ogre::StringUtil::endsWith("id", name) || Ogre::StringUtil::endsWith("use", name)) {
            continue;
        }

        try {
            ti->callMember(object, name, value);
        } catch (std::runtime_error& e) {
            Ogre::LogManager::getSingleton().logError(std::string(name)+": "+e.what());
        }
    }
}
