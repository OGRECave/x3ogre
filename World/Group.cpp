/*
 * Group.cpp
 *
 *  Created on: 06.03.2014
 *      Author: parojtbe
 */

#include <World/Group.h>

#include <World/Transform.h>
#include <Geometry/Shape.h>
#include <World/DirectionalLight.h>
#include <World/Viewpoint.h>

namespace X3D {

void Group::initialise(World& world) {
    // initialize children
    for (auto& child : _children) {
        child->initialise(world);
    }
}

void Group::attachChild(Node* obj, Ogre::SceneNode* groupParent) {
    auto geo = dynamic_cast<Shape*>(obj);
    if(geo) {
        geo->attachTo(groupParent);
        return;
    }

    auto gr = dynamic_cast<Group*>(obj);
    if(gr) {
        gr->attachTo(groupParent);
        return;
    }

    auto dl = dynamic_cast<DirectionalLight*>(obj);
    if(dl) {
        dl->attachTo(groupParent);
        return;
    }

    auto vp = dynamic_cast<Viewpoint*>(obj);
    if (vp) {
    	vp->attachTo(groupParent);
    	return;
    }
}

void Group::attachTo(Ogre::SceneNode* node) {
    if (_groupParent) {
        // was attached to something before -> re-attach children
    	for (auto child: _children) {
    	    attachChild(child.get(),node);
		}
    }

	_groupParent = node;
}

void Group::addChild(const std::shared_ptr<Node>& obj) {
	_children.push_back(obj);

	attachChild(obj.get(),_groupParent);
}

void Group::detachFrom(Ogre::SceneNode* node) {
    node->removeChild(_groupParent);
}


void Group::visible(const bool& visible) {
	if (_groupParent) {
		_groupParent->setVisible(visible,true);
	}
}

#if 0
std::shared_ptr<Node> Group::clone() {
    auto clone = std::make_shared<Group>();

    clone->attachTo(_groupParent);

	for (const auto& child: _children) {
		auto gr = std::dynamic_pointer_cast<Group>(child);

		if(gr) {
			addChild(gr->clone());
			continue;
		}

		clone->addChild(child);
	}

	return clone;
}
#endif
} /* namespace X3D */

