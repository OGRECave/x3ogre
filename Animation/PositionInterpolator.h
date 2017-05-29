/*
 * PositionInterpolator.h
 *
 *  Created on: 02.04.2014
 *      Author: baudenri
 */


#pragma once

#include <string>
#include <vector>
#include <core/Node.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <World/Transform.h>

#include <Animation/AnimationController.h>

namespace X3D {

class PositionInterpolator: public Node {
    std::vector<float> _key;
    std::vector<Ogre::Vector3> _keyValue;

    std::string _name;
    std::shared_ptr<AnimationController> _controller;
public:
    void id(const std::string& id) override;

    void key(const std::vector<float>& val);
    void keyValue(const std::vector<Ogre::Vector3>& val);

    const std::string& name() const;

    void configure(Ogre::SceneManager* sceneManager, Transform& animatedTransform) const;

    void set_fraction(const std::shared_ptr<AnimationController>& controller);
};

} /* namespace X3D */

