/*
 * Transform.h
 *
 *  Created on: 17.01.2014
 *      Author: baudenri_local
 */

#pragma once
#include <World/Group.h>

#include <OgreVector3.h>
#include <OgreQuaternion.h>

#include <iostream>
#include <vector>

namespace X3D {

class PositionInterpolator;
class OrientationInterpolator;
class ClonePropagationListener;

/**
 * Transform corresponds to the Transform-Node in X3D
 * It geometrically transforms all underlying nodes
 *
 * There must not be detached Transforms.
 */
class Transform: public Group {
	std::string _name;

	/**
	 * NodeList is partitioned as:
	 * 0    = SceneNode representing this transform
	 * 1..N = clones of this SceneNode created by def/ use mechanism
	 * when this node gets updated, all clones are updated by clonepropagationlistner
	 */
	std::vector<Ogre::SceneNode*> _topNodeList;
	std::vector<Ogre::SceneNode*> _groupParentList;

    Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE;
    Ogre::Vector3 _center = Ogre::Vector3::ZERO;
    Ogre::Vector3 _translation = Ogre::Vector3::ZERO;

    Ogre::Quaternion _orientation = Ogre::Quaternion();
    Ogre::Quaternion _scaleOrientation = Ogre::Quaternion();

    std::shared_ptr<PositionInterpolator> _positionInterpolator;
    std::shared_ptr<OrientationInterpolator> _orientationInterpolator;

    std::unique_ptr<ClonePropagationListener> _clonePropagationListener;

public:
    Transform();  // this and
    ~Transform(); // this is needed to make unique_ptr (used above) happy

    virtual void id(const std::string& id) {
    	_name = id;
	}

    const std::string& name() const {
        return _name;
    }

    /**
     * @param node SceneNode
     */
    void detachFrom(Ogre::SceneNode* node);

    /**
     * @param node SceneNode
     */
    void attachTo(Ogre::SceneNode* node);

    /**
     * Specifies the Translation offset from origin of local coordinate system.
     * @param center 3D Vector
     */
    void center(const Ogre::Vector3& center);

    /**
     * Specifies the Non-uniform x-y-z scale of child coordinate system,
     * adjusted by center and scaleOrientation.
     * @param center 3D Vector
     */
    void scale(const Ogre::Vector3& scale);

    /**
     * Specifies the Position of the children relative to local coordinate system
     * @param translation 3D Vector
     */
    void translation(const Ogre::Vector3& translation);

    /**
     * Specifies the Orientation of children relative to local coordinate system.
     * @param rotation Quaternion which represents the rotation
     */
    void rotation(const Ogre::Quaternion& rotation);

    /**
     * Specifies the  	Preliminary rotation of coordinate system before scaling
     * (to allow scaling around arbitrary orientations).
     * @param scaleOrientation Quaternion which represents the rotation
     */
    void scaleOrientation(const Ogre::Quaternion& scaleOrientation);

    /**
     * Initializes the Transform Node and propagates the Initialization Process to
     * its children
     * @param sceneManager
     */
    void initialise(World& world);

    /**
	 * Adds an animation to the Node.
	 * @note this function is names set_translation due to X3D compability. But it really is not a setter but more of a addChild
	 * @param ci
	 */
	void set_translation(const std::shared_ptr<PositionInterpolator>& ci);

	/**
	 * Adds an animation to the Node.
	 * @note this function is names set_rotation due to X3D compability. But it really is not a setter but more of a addChild
	 * @param ci
	 */
	void set_rotation(const std::shared_ptr<OrientationInterpolator>& ci);
	
	void updateSceneNode();

	Ogre::SceneNode* getTopNode() const {
	    return _topNodeList.empty() ? nullptr : _topNodeList.front();
	}

	/**
	 * Returns true, if Transform is USEd (via DEF/USE)
	 */
	bool isUsed() const {
		return _groupParentList.size()>1;
	}

    const Ogre::Vector3& scale() const {
        return _scale;
    }

	const Ogre::Vector3& center() const {
		return _center;
	}

    const Ogre::Vector3& translation() const {
        return _translation;
    }
};

}
