/*
 * Route.cpp
 *
 *  Created on: 27.03.2014
 *      Author: parojtbe
 */

#include <World/Route.h>

#include <reflection/db.h>
#include <World/Scene.h>
#include <OgreLogManager.h>

namespace X3D {

void Route::fromField(const std::string& fromField) {
    _fromField = fromField;
}

void Route::fromNode(const std::string& fromNode) {
    _fromNode = fromNode;
}

void Route::toField(const std::string& toField) {
    _toField = toField;
}

void Route::toNode(const std::string& toNode) {
    _toNode = toNode;
}

void Route::apply(const Scene& scene, const std::string& nameSpace) const {
    try {
    	std::string nsFromNode = nameSpace;
    	std::string nsToNode = nameSpace;
    	if (nameSpace.empty()) {
    		nsFromNode = _fromNode;
    		nsToNode = _toNode;
    	} else {
    		nsFromNode.append(".").append(_fromNode);
    		nsToNode.append(".").append(_toNode);
    	}


        auto from = scene._shareNode(nsFromNode);
        Node* to = scene.getNode(nsToNode);

        reflection::TypeInfoCommon* ti = reflection::getTypeInfo(*to);

        ti->memberByArgType(from.get()); // throws if member unsupported
        ti->callMember(to, _toField, from);
    } catch(std::exception& e) {
        Ogre::LogManager::getSingleton().logMessage("could not apply Route to Field '"+ _toNode + "." + _toField + "': "  + e.what(), Ogre::LML_NORMAL);
    }
}

} /* namespace X3D */

