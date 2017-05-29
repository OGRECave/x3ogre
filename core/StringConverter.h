/*
 * StringConverter.h
 *
 *  Created on: 06.02.2014
 *      Author: parojtbe
 */

#pragma once

#include <string>
#include <vector>
#include <stdint.h>

#include <OgreVector3.h>

namespace X3D {
namespace StringConverter {

template <typename T> void parse(const std::string& str, T& v);
template <typename T> void parse(const std::string& str, std::vector<T>& vec);

template <typename T> std::string toString(const T& v);
template <typename T> std::string toString(const std::vector<T>& vec);

}
}
