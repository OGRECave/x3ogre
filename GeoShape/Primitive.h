/*
 * Primitive.h
 *
 *  Created on: 03.03.2014
 *      Author: parojtbe
 */

#pragma once

#include <vector>
#include <stdint.h>

#define GS_USE_OGRE

#ifdef GS_USE_OGRE
#include <OgreVector2.h>
#include <OgreVector3.h>
#else
#include <Eigen/Core>
#endif

namespace GeoShape {

#ifdef GS_USE_OGRE
typedef Ogre::Vector3 Vector3;
typedef Ogre::Vector2 Vector2;
#else
typedef Eigen::Vector3f Vector3;
typedef Eigen::Vector2f Vector2;
#endif

/**
 * has to be rendered either as GL_TRIANGLE_STRIP or GL_TRIANGLE_FAN
 * @param vertex
 * @param normal
 * @param texCoord
 * @param mode render mode
 * @param outr
 * @param inr
 * @param res
 */
void Disk2D(std::vector<Vector2>& vertex,
        std::vector<Vector3>& normal,
        std::vector<Vector2>& texCoord,
        uint32_t& mode,
        float outr = 1.0, float inr = 0.0, size_t res = 16);

void Box(std::vector<uint16_t>& idx,
        std::vector<Vector3>& vertex,
        std::vector<Vector3>& normal,
        std::vector<Vector2>& texCoord,
        const Vector3& size = Vector3(1, 1, 1));

/**
 * has to be rendered as GL_TRIANGLE_FAN
 * @param vertex
 * @param normal
 * @param texCoord
 * @param size
 */
void Rectangle(std::vector<Vector2>& vertex,
        std::vector<Vector3>& normal,
        std::vector<Vector2>& texCoord,
        const Vector2& size = Vector2(1, 1));

void Sphere(std::vector<uint16_t>& idx,
        std::vector<Vector3>& vertex,
        std::vector<Vector3>& normal,
        std::vector<Vector2>& texCoord,
        const Vector3& size = Vector3(1, 1, 1),
        size_t res = 16);

void Cylinder(std::vector<uint16_t>& idx,
        std::vector<Vector3>& vertex,
        std::vector<Vector3>& normal,
        std::vector<Vector2>& texture,
        const Vector3& size = Vector3(1, 1, 1),
        size_t res = 16);

void Cone(std::vector<uint16_t>& idx,
        std::vector<Vector3>& vertex,
        std::vector<Vector3>& normal,
        std::vector<Vector2>& texture,
        const Vector3& size = Vector3(1, 1, 1),
        size_t res = 16);

} /* namespace GeoShape */
