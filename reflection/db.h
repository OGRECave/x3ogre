/*
 * db.h
 *
 *  Created on: 06.02.2014
 *      Author: parojtbe
 */

#pragma once

#include <string>

#include <reflection/TypeInfo.h>

namespace reflection {

TypeInfoCommon* getTypeInfo(const std::string& type);

TypeInfoCommon* getTypeInfo(const Object& obj);

std::string getTypeName(const Object& obj);

/**
 * allow to retrieve the same type under different names
 * the type has to be in the database before aliasing
 * @param type original typename
 * @param alias alias for typename
 */
void alias(const std::string& type, const std::string& alias);

}
