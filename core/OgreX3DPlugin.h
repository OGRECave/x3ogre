/*
 * OgrePlugin.h
 *
 *  Created on: 21.04.2018
 *      Author: pavel
 */

#ifndef CORE_OGREX3DPLUGIN_H_
#define CORE_OGREX3DPLUGIN_H_

#include "OgrePlugin.h"
#include "OgreSceneLoader.h"

namespace X3D
{

class OgreX3DPlugin : public Ogre::Plugin
{
    const Ogre::String& getName() const;

    void install() {}
    void initialise();
    void shutdown();
    void uninstall() {}
protected:
    Ogre::SceneLoader* mX3DSceneLoader;
};

} /* namespace X3D */

#endif /* CORE_OGREX3DPLUGIN_H_ */
