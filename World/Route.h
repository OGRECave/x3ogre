/*
 * Route.h
 *
 *  Created on: 27.03.2014
 *      Author: parojtbe
 */

#pragma once

#include <memory>
#include <string>
#include <core/Node.h>

#include <World/Scene.h>

namespace X3D {

class Route : public Node {
    std::string _fromNode;
    std::string _fromField;
    std::string _toNode;
    std::string _toField;
public:
    void fromField(const std::string& fromField);
    void fromNode(const std::string& fromNode);
    void toField(const std::string& toField);
    void toNode(const std::string& toNode);

    void apply(const Scene& world, const std::string& nameSpace) const;
};

} /* namespace X3D */
