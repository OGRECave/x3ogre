/*
 * WorldInfo.h
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once
#include <core/Node.h>

namespace X3D {

/**
 * WorldInfo contains persistent documentation information about the 3D world.
 */

class WorldInfo: public Node {
    std::string _title = "x3ogre";

public:

    /**
     * Sets the title of this world.
     * This title is placed in the window Title
     * @param title
     */
    void title(const std::string& title);

    /**
     * Getter for the title
     * @return Title of this world
     */
    const std::string& title() const;
};

}
