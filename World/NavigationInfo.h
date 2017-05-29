/*
 * NavigationInfo.h
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once
#include <core/BindableNode.h>
#include <World/DirectionalLight.h>
#include <OgreSceneNode.h>

namespace X3D
{

/**
 * NavigationInfo describes the viewing model and physical characteristics of the
 * viewer's avater.
 * In current implementation it should handle the headlight.
 */

class NavigationInfo: public BindableNode<NavigationInfo> {
	bool _headlight = true;

	Ogre::SceneManager* _sceneManager = nullptr;

	std::string _control="";

	std::unique_ptr<DirectionalLight> _light = std::unique_ptr<DirectionalLight>(new DirectionalLight());
public:


	/**
	 * Enable/disable directional light that always points in
	 * the direction the user is looking.
	 * @param headLight
	 */
	void headlight(const bool& headLight);
	const bool& headlight() const;

	// Setting/Getting the NodeID of the Node which should be manipulated by input
	void control(const std::string& control) { _control = control;}
	const std::string& control() const {return _control;}

	/**
	 * Getter for the Headlight specified in the NavigationInfo
	 * @return Headlight
	 */
	DirectionalLight* getLight();

	/**
	 * Initializes the Light-Source and attaches it to the scene
	 * @param sceneManager
	 */
	void initialise(World& world) override;

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
