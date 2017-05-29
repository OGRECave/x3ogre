/*
 * TypeNames.h
 *
 *  Created on: 24.10.2014
 *      Author: parojtbe
 */

#pragma once

#include <OgreColourValue.h>

namespace reflection {

template<typename T> struct TypeName {
	static const char* name(){
		return "UnknownType";
	}
};

template<> struct TypeName<bool> {
	static const char* name(){
		return "bool";
	}
};

template <> struct TypeName<Ogre::ColourValue> {
	static const char* name(){
		return "ColourValue";
	}
};

template <> struct TypeName<int> {
	static const char* name(){
		return "int";
	}
};

template <> struct TypeName<Ogre::Real> {
	static const char* name(){
		return "Real";
	}
};

template <> struct TypeName<Ogre::Vector3> {
    static const char* name(){
        return "Vector3";
    }
};

template <> struct TypeName<Ogre::Quaternion> {
    static const char* name(){
        return "Quaternion";
    }
};
}
