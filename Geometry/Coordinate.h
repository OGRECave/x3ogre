/*
 * Coordinates.h
 *
 *  Created on: 03.12.2013
 *      Author: baudenri_local
 */
#pragma once

#include <core/Node.h>
#include <vector>
#include <memory>

#include <Animation/CoordinateInterpolator.h>

namespace X3D
{
/**
 * Vector of 3D Positions
 */
class Coordinate: public Node{
	std::vector<Ogre::Vector3> _point;
	std::shared_ptr<CoordinateInterpolator> _interpolator;
public:
	/**
	 * Specifies the Position Vector
	 * @param val Vector of 3D Positions
	 */
	void point(const std::vector<Ogre::Vector3>& val);

	/**
	 * Returns the Position Vector
	 * @return Vector of 3D Positions
	 */
	std::vector<Ogre::Vector3>& point();

	/**
	 * Adds an animation to the Node.
	 * @note this function is names set_point due to X3D compability. But it really is not a setter but more of a addChild
	 * @param ci
	 */
	void set_point(const std::shared_ptr<CoordinateInterpolator>& ci);

	const std::shared_ptr<CoordinateInterpolator>& interpolator() const;
};

}
