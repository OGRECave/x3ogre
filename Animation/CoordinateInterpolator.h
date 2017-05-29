/*
 * CoordinateInterpolator.h
 *
 *  Created on: 25.03.2014
 *      Author: parojtbe
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <core/Node.h>
#include <OgreVector3.h>

#include <Animation/AnimationController.h>

namespace X3D {

class CoordinateInterpolator: public Node {
    std::vector<float> _key;
    std::vector<Ogre::Vector3> _keyValue;

    std::string _name;
    std::shared_ptr<AnimationController> _controller;
public:
    void addToMesh(Ogre::Mesh* mesh);

    void id(const std::string& id) override;

    void key(const std::vector<float>& val);
    void keyValue(const std::vector<Ogre::Vector3>& val);

    const std::string& name() const;

    void configure(Ogre::AnimationState* anim) const;

    void set_fraction(const std::shared_ptr<AnimationController>& controller);
};

} /* namespace X3D */

