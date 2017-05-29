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

#include <Parser/X3DFileManager.h>

#include <OgreLogManager.h>

using namespace X3D;

void X3DParser::recurse(rapidxml::xml_node<>* n, StackNode& parent) {
    StackNode node = parseNode(n, parent);

    // DFS traversal of Tree
    for(auto c = n->first_node(); c; c = c->next_sibling()) {
        recurse(c, node);
    }
}

X3DParser::X3DParser(std::string xml, Node& parent, Scene& scene, const std::string& nameSpace) :
_scene(scene), _nameSpace(nameSpace)
{
    const int flags = rapidxml::parse_validate_closing_tags | rapidxml::parse_no_data_nodes;

    rapidxml::xml_document<> doc;
    doc.parse<flags>(&xml[0]);

    reflection::alias("Route", "ROUTE");

    StackNode root{"X3DParser::parent", reflection::getTypeInfo(parent), &parent};

    recurse(doc.first_node(), root);
}

// functions for building the scene graph
StackNode X3DParser::parseNode(rapidxml::xml_node<>* xmlnode, StackNode& parent) {
    StackNode node;
    node.tag = xmlnode->name();

    if (stricmp(node.tag, "x3d") == 0) {
        // x3d has no counterpart in our scenegraph, so just skip it
        return parent;
    }

    if (stricmp(node.tag, "scene") == 0) {
        // we create the Scene node from outside
        return parent;
    }

    //std::cout << "Tag: " << node.tag << std::endl;

    node.ti = reflection::getTypeInfo(node.tag);

    if (not node.ti) {
        Ogre::LogManager::getSingleton().logMessage("no such node: '" + std::string(node.tag) + "'", Ogre::LML_NORMAL);
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
        Ogre::LogManager::getSingleton().logMessage(std::string(parent.tag) + "::*(" + node.tag + "): " + e.what(), Ogre::LML_NORMAL);
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

void X3DParser::resolveDefUse(std::shared_ptr<Node>& object, rapidxml::xml_node<>* node) {
    for(auto a = node->first_attribute(); a; a = a->next_attribute()) {
        const char* name = a->name();
        const char* value = a->value();

        if (stricmp(name, "use") == 0) {
            // make object point to previous instance. destroys temporary object.
            object = _scene._shareNode(resolveNamespace(value));
            break;
        }

        if (stricmp(name, "def") == 0 or strcmp(name, "id") == 0) {
            std::string qualifiedName = resolveNamespace(value);
            _scene.registerNode(object, qualifiedName);
            object->id(qualifiedName);
            continue;
        }
    }
}

void X3DParser::parseAttributes(rapidxml::xml_node<>* node, Node* object, reflection::TypeInfoCommon* ti) {
    for(auto a = node->first_attribute(); a; a = a->next_attribute()) {
        const char* name = a->name();
        const char* value = a->value();

        if (stricmp(name, "def") == 0 or strcmp(name, "id") == 0 or stricmp(name, "use") == 0) {
            continue;
        }

        try {
            ti->callMember(object, name, value);
        } catch (std::runtime_error& e) {
            Ogre::LogManager::getSingleton().logMessage(std::string(node->name())+": "+e.what(), Ogre::LML_NORMAL);
        }
    }
}
