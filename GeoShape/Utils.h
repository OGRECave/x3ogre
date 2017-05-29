/*
 * Utils.h
 *
 *  Created on: 20.03.2014
 *      Author: parojtbe
 */

#pragma once

#include <vector>
#include <ciso646>
#include <stdint.h>

#define GS_USE_OGRE

#ifdef GS_USE_OGRE
#include <OgreVector2.h>
#include <OgreVector3.h>
#else
#include <Eigen/Geometry>
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
 * rescales texture coordinates
 * useful for NPOT textures and flipping the texture
 * @param scale
 * @param coords
 */
void RescaleTexcoords(const Vector2& scale, std::vector<Vector2>& coords);

/**
 * Generates Smooth Normals for given Indexed Triangle Set
 * @param index
 * @param points
 * @param normals
 */
void GenerateNormals(const std::vector<uint32_t>& index, const std::vector<Vector3>& points, std::vector<Vector3>& normals);

/**
 * Generates Flat Normals for given Triangle Set
 * @param points
 * @param normals
 */
void GenerateNormals(const std::vector<Vector3>& points, std::vector<Vector3>& normals);

/**
 * duplicates pointsIn according to index
 * @param index
 * @param pointsIn
 * @param pointsOut
 */
template<class T>
void DuplicateByIndex(const std::vector<uint32_t>& index, const std::vector<T>& pointsIn, std::vector<T>& pointsOut);

/**
 * triangulate index of polygon faces separated by -1 to triangulated index tri_index
 * @param index
 * @param tri_index
 */
void Triangulate(const std::vector<int>& index, std::vector<uint32_t>& tri_index);

/**
 * convert index of line strips separated by -1 to line list index lst_index
 * @param index
 * @param lst_index
 */
void LineStripToList(const std::vector<int>& index, std::vector<uint32_t>& lst_index);

/**
 * Resorts vector of Vector3 specified by an index
 * @param index
 * @param vector in-Vector
 * @param outVector
 */
void SortVectorByIndex(const std::vector<int>& index, const std::vector<Vector3>& vector, std::vector<Vector3> outVector);

} /* namespace GeoShape */
