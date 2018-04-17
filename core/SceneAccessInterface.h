/*
 * Viewer.h
 *
 *  Created on: 24.03.2014
 *      Author: parojtbe
 */

#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <mutex>
#include <map>

#include <OgreShaderGenerator.h>
#include <OgreAxisAlignedBox.h>
#include <OgreRenderWindow.h>
#include <OgreFrameListener.h>

#include <Parser/X3DFileManager.h>
#include <World/Scene.h>

#include <x3ogre_core_export.h>
#include "World.h"

namespace X3D {

/**
 * Scene Access Interface as mandated by the X3D standard
 * however we only roughly follow the standard here.
 * Correctness is not mandatory.
 * for details see:
 * http://www.web3d.org/documents/specifications/19775-2/V3.3/Part02/servRef.html
 */
class X3OGRECORE_EXPORT SceneAccessInterface : public Ogre::FrameListener  {
    bool init = false;
    Ogre::SceneNode* _rootNode = nullptr;

    std::string _fileName;
    std::string _basePath;

    std::unique_ptr<X3DFileManager> _x3dFM;

    bool _doDebugDrawing = false;

    // Updates associated by nodeAndField
    std::map<std::pair<std::string,std::string>,std::string> _updates;
    std::mutex _updateMutex;

    /**
     * enforce per pixel lighting for all RTSS materials
     */
    void forcePerPixelLighting();

    void setDefaultAnisotropy(uint32_t lvl);

    /**
     * Unloads current File and resets all necessary references to enable
     * 	reloading a new file
     */
    void clearWorld();

    Ogre::Vector3 _camTgt = Ogre::Vector3::ZERO;

    Ogre::RTShader::ShaderGenerator* mShaderGenerator;
public:
    SceneAccessInterface();
    ~SceneAccessInterface();

    /**
     * links active camera to the output window. must be called after world is initialized.
     * @param window
     */
    void setWindow(Ogre::RenderWindow* window);

    void setNodeAttribute(const std::string& nodeName, const std::string& fieldName, const std::string& fieldValue, bool buffer = true);

    std::string getNodeAttribute(const std::string& nodeName, const std::string& fieldName);

    void loadURL(const std::string& url, Ogre::SceneNode* rootNode);

    Scene* scene();

    bool frameStarted(const Ogre::FrameEvent& evt);

    /**
     * Enables/Disables DebugDrawing
     */
    void switchDebugDrawing();

    /**
	 * length of the world bbox diagonal
	 */
    float getWorldSize();
};

} /* namespace X3D */
