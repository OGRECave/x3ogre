/*
 * Appearance.h
 *
 *  Created on: 15.11.2013
 *      Author: baudenri_local
 */

#pragma once
#include <core/Node.h>
#include <OgreNameGenerator.h>

#include <Appearance/Material.h>
#include <Appearance/ComposedShader.h>
#include <Appearance/RTShaderSystem.h>
#include <Appearance/Texture.h>

namespace X3D {

/**
 * This represents an Ogre::Material Node
 * the optional children X3D::ImageTexture and X3D::Material
 * can be used to accordingly configure the Ogre::Material
 */
class Appearance: public Node, public Ogre::Resource::Listener {
	//FIXME unused???
	bool _phantom = false;

    std::shared_ptr<Material> _material;
    std::shared_ptr<Texture> _texture;
    std::shared_ptr<ComposedShader> _shader;
    std::vector<std::shared_ptr<RTShaderSystem>> _rtss;

    Ogre::Pass* _pass = nullptr;

    Ogre::MaterialPtr _mat;

    static Ogre::NameGenerator _nameGenerator;
    Ogre::String _name;

    void configureSubRenderStates(Ogre::MaterialPtr appearance);

public:
    ~Appearance();

    /** callback for material
     * @param resource
     */
    void loadingComplete(Ogre::Resource* resource) override;

    /**
	 * Adds a RTShaderSystem part
	 * @param shader
	 */
	void addRTShaderSystem(const std::shared_ptr<RTShaderSystem>& rtss);


    /**
     * Specifies the Shader of that Appearance
     * @param shader
     */
    void setShader(const std::shared_ptr<ComposedShader>& shader);

    /**
     * Specifies the Material
     * @param materials
     */
    void setMaterial(const std::shared_ptr<Material>& material);

    void id(const std::string& id) override;

    /**
     * Returns the Name of the Appearance
     * @return Name of the Appearance
     */
    const Ogre::String& name() const;

    /**
     * Specified the Texture
     * @param texture
     */
    void setTexture(const std::shared_ptr<Texture>& texture);

    /**
     * Returns the Texture
     * @return Texture
     */
    Texture* getTexture();

    /**
	 * Returns the Material
	 * @return Material
	 */
	Material* getMaterial();

    /**
     * Returns the Pass specified by the Appearance
     * @return specified Pass
     */
    Ogre::Pass* pass() const {
        return _pass;
    }

    //FIXME unused???
    void phantom(const bool& phantom);

    /**
     * creates an Ogre::Material, generating an unique name in the for of X3D/APP<N>.
     * And configures it based on the ImageTexture and Material children
     */
    void create();

    void initialise(World& world) override;

    void addListener(Ogre::Resource::Listener* listener);
    void removeListener(Ogre::Resource::Listener* listener);
};

}
