/*
 * X3DObject.h
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once

#include <string>
#include <ciso646>
#include <reflection/Object.h>
#include <memory>

namespace X3D {

struct World;

/**
 * Superclass every Node that has been parsed from the X3D-File inherits
 */
class Node : public reflection::Object {
protected:
    bool _init =  false;

public:
    virtual void id(const std::string& id) {
    }

    virtual void initialise(World& world) {
    }
};

}

