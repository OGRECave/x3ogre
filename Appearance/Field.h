/*
 * Field.h
 *
 *  Created on: 27.08.2014
 *      Author: baudenri
 */

#pragma once

#include <core/Node.h>
#include <OgreGpuProgramParams.h>

namespace X3D {

class Field: public Node {

	std::string _name;
	std::string _type;
	std::string _accessType;
	std::string _value;

public:

	void name(const std::string& name);
	void type(const std::string& type);
	void accessType(const std::string& accessType);
	void value(const std::string& value);

	void apply(Ogre::GpuProgramParameters* params);
};

}
