/*
 * Geometry.h
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once
#include <core/Node.h>

#include <OgreNameGenerator.h>
#include <OgreManualObject.h>
#include <OgreEntity.h>
#include <OgreResource.h>
#include <OgreMesh.h>

namespace X3D {

/**
 * Class for covering all visible Objects
 */
class Geometry: public Node , public Ogre::ManualResourceLoader{
    static Ogre::NameGenerator _nameGenerator;

protected:
    static std::string _noAnimation;

    Ogre::String _name;
    bool _solid = true;

public:

    ~Geometry();

    void create();

    void id(const std::string& id) {
        _name = id;
    }

    /**
     * Setting "solid" true means draw only one side of polygons (backface culling on),
     * setting "solid" false means draw both sides of polygons (backface culling off).
     * Warning: default value true can completely hide geometry if viewed from wrong side!
     * @param solid
     */
    void solid(const bool& solid) {
        _solid = solid;
    }

    /**
     * Returns if the Geometry is solid
     * @return true if polygon is only visible from frontside
     */
    const bool& solid() const {
        return _solid;
    }

    /**
     * Name of the Geometry
     * @return Name
     */
    const Ogre::String& name() const {
        return _name;
    }

    virtual void configureAnimation(Ogre::AnimationState* anim) const {
    }

    virtual bool hasVertexColors() const {
    	return false;
    }

    virtual bool ccw() const {
        return true;
    }

    /**
     * If an animation is attached to the geometry, this function returns the name
     * returns empty string otherwise
     * @return
     */
    virtual const std::string& animation() const {
        return _noAnimation;
    }

    virtual void prepareResource(Ogre::Resource* resource){

    }

    virtual void loadResource(Ogre::Resource* resource) {

    }
};

}
