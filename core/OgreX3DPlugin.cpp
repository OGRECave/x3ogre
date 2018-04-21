/*
 * OgrePlugin.cpp
 *
 *  Created on: 21.04.2018
 *      Author: pavel
 */

#include "OgreX3DPlugin.h"
#include "SceneLoader.h"
#include "x3ogre_core_export.h"
#include "OgreRoot.h"

namespace X3D
{

    const Ogre::String& X3D::OgreX3DPlugin::getName() const {
        static Ogre::String name = "X3D Scene Loader";
        return name;
    }

    void X3D::OgreX3DPlugin::initialise() {
        mX3DSceneLoader = new SceneLoader();
    }

    void X3D::OgreX3DPlugin::shutdown() {
        delete mX3DSceneLoader;
    }

} /* namespace X3D */


#ifndef OGRE_STATIC_LIB
    extern "C" void X3OGRECORE_EXPORT dllStartPlugin();
    extern "C" void X3OGRECORE_EXPORT dllStopPlugin();

    static X3D::OgreX3DPlugin plugin;

    extern "C" void X3OGRECORE_EXPORT dllStartPlugin()
    {
        Ogre::Root::getSingleton().installPlugin(&plugin);
    }
    extern "C" void X3OGRECORE_EXPORT dllStopPlugin()
    {
        Ogre::Root::getSingleton().uninstallPlugin(&plugin);
    }
#endif
