/*
 * Background.h
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once
#include <core/BindableNode.h>
#include <OgreColourValue.h>
#include <OgreViewport.h>

namespace X3D {

/**
 * Class which represents background
 * Equivalent to a Ogre::ColourValue
 */

class Background: public BindableNode<Background> {
    Ogre::ColourValue _skyColor = Ogre::ColourValue::White;
    Ogre::Viewport* _viewport = nullptr;
public:
    /**
     * Setter for the ColourValue
     * @param skyColour Colour-Value of the Background of the Scene
     */
    void skyColor(const Ogre::ColourValue& skyColour);

    /**
     * Getter for the ColourValue
     * @return Background Colour of the Scene
     */
    const Ogre::ColourValue& skyColor() const;

    void apply(Ogre::Viewport* viewport);
/*
    void detachFromParent() {
    	_viewport = nullptr;
    }*/


    void initialise(World& world) override {
    	BindableNode<Background>::initialise(world);
    }

    /**
     * Propagates setBind command to BindableNode
     */
    void setBind(const bool& bind) {
    	set_bind(bind);
    }

    void onBound(Scene& scene) override;

    void onUnbound(Scene& scene) override;


};

}
