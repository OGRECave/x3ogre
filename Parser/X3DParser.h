/*
 * X3DParser.h
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once

#include <string>
#include <vector>

#include <core/Node.h>
#include <World/Scene.h>
#include <Parser/X3DFileManager.h>
#include <reflection/db.h>

#include <cstddef>
#include <Parser/rapidxml.hpp>

namespace X3D
{

struct StackNode {
    const char* tag;
    reflection::TypeInfoCommon* ti;
    Node* object;
};

class X3DParser {
	Scene& _scene;
	std::string _nameSpace;

	void recurse(rapidxml::xml_node<>* n, StackNode& parent);

    StackNode parseNode(rapidxml::xml_node<>* node, StackNode& parent);

    std::string resolveNamespace(const std::string& name);
    void resolveDefUse(std::shared_ptr<Node>& object, rapidxml::xml_node<>* node);
    void parseAttributes(rapidxml::xml_node<>* node, Node* object, reflection::TypeInfoCommon* ti);
public:
	X3DParser(std::string xml, Node& parent, Scene& scene, const std::string& nameSpace = "");
};

}
