/*
 * Field.cpp
 *
 *  Created on: 27.08.2014
 *      Author: baudenri
 */

#include <Appearance/Field.h>
#include <OgreStringConverter.h>
#include <OgreLogManager.h>

using namespace X3D;

void Field::name(const std::string& name) {
	_name = name;
}
void Field::type(const std::string& type) {
	_type = type;
}
void Field::accessType(const std::string& accessType){
	_accessType = accessType;
}

void Field::value(const std::string& value) {
	_value = value;
}

void Field::apply(Ogre::GpuProgramParameters* params) {
	if (not params) {
		return;
	}

	if (_value.empty()) {
	    Ogre::LogManager::getSingleton().logMessage("Field Node '"+_name+"' has no value", Ogre::LML_NORMAL);
		return;
	}

	if (stricmp(_type.c_str(), "sfint32") == 0) {
		int i;
		Ogre::StringConverter::parse(_value,i);
		params->setNamedConstant(_name, i);
	} else if (stricmp(_type.c_str(), "sffloat") == 0){
		float f;
		Ogre::StringConverter::parse(_value,f);
		params->setNamedConstant(_name, f);
	} else if (stricmp(_type.c_str(), "sfvec3f") == 0){
		Ogre::Vector3 vec3f;
		Ogre::StringConverter::parse(_value,vec3f);
		params->setNamedConstant(_name, vec3f);
	} else {
	    Ogre::LogManager::getSingleton().logMessage("Field Node '"+_name+"': type '"+_type+"' is not implemented", Ogre::LML_NORMAL);
	}

}
