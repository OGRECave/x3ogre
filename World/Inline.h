/*
 * Inline.h
 *
 *  Created on: 24.04.2014
 *      Author: baudenri
 */

#pragma once

#include <World/Group.h>
#include <World/Scene.h>

namespace X3D
{

/**
 * Node which loads another X3D Scene and holds it
 */
class Inline: public Group {
private:
	static Ogre::NameGenerator _nameGenerator;

	std::string _filename, _basepath;
	std::string _nameSpace;
	std::string _url;
	bool _load = true;

	std::shared_ptr<Scene> _scene;

public:
	void id(const std::string& id) override {
		_nameSpace = id;
	}

	void initialise(World& world) override;

	const std::string nameSpace();

	/**
	 * nges the URL of the Inline node. It splits the url into filename and
	 * basepath and saves them acordingly. If a Resourcegroup with the name
	 * "X3D/"+basepath does not exist, it will be created and the location
	 * of the file added to this Resourcegroup.
	 * @param url
	 */
	void url(const std::string& url);

	void load(const bool& load);

	const std::string& url() const;
};

}
