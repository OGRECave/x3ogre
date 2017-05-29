/*
 * Viewpoint.h
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once

#include <core/BindableNode.h>
#include <World/Group.h>
#include <string>

#include <OgreCamera.h>
#include <OgreNameGenerator.h>

namespace X3D
{
/**
 * The Viewpoint provides a specific location and direction where the user may view
 * the scene.
 * It contains a Ogre::Camera, a Ogre::SceneNode, where the Camara is fixed to.
 */
class Viewpoint : public BindableNode<Viewpoint> {

protected:
    Ogre::Vector3 _position = Ogre::Vector3::ZERO;
    Ogre::Quaternion _orientation;

	std::string _id = "vp";
	double _zNear = 0.5;
	double _zFar = 3000.0;
	float _ratio = 1;
	Ogre::Radian _fovy;

	// Holds the Camera and the Transformation of the Viewpoint
    Ogre::SceneNode* _cameraNode = nullptr;
    Ogre::SceneNode* _groupParent = nullptr;
	Ogre::Camera* _camera = nullptr;

    std::vector<std::string> _compositors;

	static Ogre::NameGenerator _nameGenerator;

	void applyCompositors(Ogre::Viewport* viewport, const std::vector<std::string>& newcomps);

public:

    void compositors(const std::vector<std::string>& names);

    const std::vector<std::string>& compositors() const {
        return _compositors;
    }

	/**
	 * Setter for the aspect ratio of the camera
	 * @param ration aspect ratio: width/height
	 */
	void setAspect(float ratio);

	/**
	 * Sets the position of the Viewpoint relative to local coordinate system.
	 * @param position
	 */
	void position(const Ogre::Vector3& position);
	const Ogre::Vector3& position() const { return _position;}

	/**
	 * Sets the orientation of the Camera using a Quaternion
	 * @param orientation
	 */
	void orientation(const Ogre::Quaternion& orientation);
	const Ogre::Quaternion& orientation() const { return _orientation; }

	/**
	 * Sets the modelview Matrix of the Camera.
	 * This should be an affine matrix.
	 * @param modelview
	 */
	void modelviewMatrix(const Ogre::Matrix4& modelview);

	/**
	 * Sets the projection Matrix of the Camera.
	 * @param projection
	 */
	void projectionMatrix(const Ogre::Matrix4& projection);

	//FIXME
	void setID(std::string id);

	/**
	 * Sets the z-near position;
	 * used for clipping
	 * @param zNear
	 */
	void zNear(const double& zNear);
	const double& zNear() const { return _zNear;}

	/**
	 * Sets the z-Far position
	 * used for clipping
	 * @param zFar
	 */
	void zFar(const double& zFar);
	const double& zFar() const { return _zFar;}

	/**
	 * Zooms in/out
	 * @param relFactor
	 */
	void zoom(float relFactor);

	/**
	 * Returns the Ogre::Camera
	 * @return current Camera
	 */
	Ogre::Camera* getCamera();

	Ogre::SceneNode* getNode() {
	    return _cameraNode;
	}

	/**
	 * Initializes the Camera and attaches it to the Scene
	 * @param sceneManager
	 */
	void initialise(World& world) override;

	/**
	 * Sets ZoomFactor to start
	 */
	void resetZoom();

	/**
	 * Propagates setBind command to BindableNode
	 */
	void setBind(const bool& bind) {
		set_bind(bind);
	}

	// Attach CameraNode to this SceneNode
	void attachTo(Ogre::SceneNode* groupParent);
	// Remove CameraNode from this SceneNode
	void detachFrom(Ogre::SceneNode* node);

	void onBound(Scene& scene) override;
};

}
