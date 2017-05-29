/*
 * WorldInfo.cpp
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#include <World/WorldInfo.h>

using namespace X3D;

// Variablen
void WorldInfo::title(const std::string& title) {
    _title = title;
}

const std::string& WorldInfo::title() const {
    return _title;
}
