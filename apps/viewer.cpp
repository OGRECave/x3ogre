#include <iostream>

#include <OgreApplicationContext.h>
#include <core/SceneAccessInterface.h>
#include <core/OgreX3DPlugin.h>

#include <OgreOverlaySystem.h>
#include <OgreTrays.h>
#include <OgreAdvancedRenderControls.h>
#include <OgreCameraMan.h>
#include <OgreShaderGenerator.h>

#include <World/Viewpoint.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_EMSCRIPTEN
#include <emscripten/bind.h>

void emloop(void* target) {
    Ogre::WindowEventUtilities::messagePump();
    try {
        static_cast<Ogre::Root*>(target)->renderOneFrame();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        emscripten_cancel_main_loop();
    }
}
#endif

namespace {
void printSceneGraph(Ogre::SceneNode* node, const std::string& tabs = "") {
    std::cout << tabs << node->getName() << std::endl;

    for(auto mo : node->getAttachedObjects()) {
        std::cout << tabs+"\t" << mo->getName() << std::endl;
    }

    for(auto n : node->getChildren()) {
        printSceneGraph(static_cast<Ogre::SceneNode*>(n), tabs+"\t");
    }
}

struct X3Ogre : public OgreBites::ApplicationContext, OgreBites::InputListener {
    std::unique_ptr<X3D::SceneAccessInterface> _sai;
    std::unique_ptr<OgreBites::TrayManager> _trays;
    std::unique_ptr<OgreBites::AdvancedRenderControls> _controls;
    std::unique_ptr<OgreBites::CameraMan> _camman;

    Ogre::SceneManager* _sceneManager = nullptr;

    X3Ogre() : OgreBites::ApplicationContext("x3ogre") {
        initApp();
    }

    ~X3Ogre() {
        closeApp();
    }

    // SAI like API
    bool nodeExists(const std::string& node) {
        return _sai->scene()->nodeExists(node);
    }

    void setNodeAttribute(const std::string& node, const std::string& field, const std::string& value) {
        _sai->setNodeAttribute(node, field, value);
    }

    std::string getNodeAttribute(const std::string& node, const std::string& field) {
        return _sai->getNodeAttribute(node, field);
    }

    void loadFile(const std::string& file) {
        // add X3D path to Ogre resources
        Ogre::String filename, basepath;
        Ogre::StringUtil::splitFilename(file, filename, basepath);

        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(basepath, "FileSystem", "X3D", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("X3D");
        Ogre::ResourceGroupManager::getSingleton().setWorldResourceGroupName("X3D");

        if(_controls) {
            removeInputListener(_controls.get());
            _controls.reset();
        }

        if(_camman) {
            removeInputListener(_camman.get());
            _camman.reset();
        }

        if (_sceneManager) {
            mShaderGenerator->removeSceneManager(_sceneManager);
            mRoot->destroySceneManager(_sceneManager);

            mShaderGenerator->removeAllShaderBasedTechniques();
            mShaderGenerator->flushShaderCache();
        }

        // Remove old Viewport if present
        if (getRenderWindow()->hasViewportWithZOrder(0)) {
            getRenderWindow()->removeViewport(0);
        }

        _sceneManager = mRoot->createSceneManager();
        mShaderGenerator->addSceneManager(_sceneManager);
        _sceneManager->addRenderQueueListener(getOverlaySystem());

        _sceneManager->getRootSceneNode()->loadChildren(filename);

        // SAI init
        _sai.reset(new X3D::SceneAccessInterface(_sceneManager->getRootSceneNode()));
        _sai->addEssentialNodes(); // ensure we have a X3D::Viewpoint
        auto cam =_sai->scene()->bound<X3D::Viewpoint>()->getCamera();
        _sai->scene()->setViewport(getRenderWindow()->addViewport(cam));

        _controls.reset(new OgreBites::AdvancedRenderControls(_trays.get(),
                                                              cam));
        addInputListener(_controls.get());

        _camman.reset(new OgreBites::CameraMan(cam->getParentSceneNode()));
        _camman->setStyle(OgreBites::CS_ORBIT);
        _camman->setYawPitchDist(Ogre::Radian(0), Ogre::Radian(0), _sai->getWorldSize());
        addInputListener(_camman.get());
    }

    // internal API
    void setup() override {
        // Ogre setup
        OgreBites::ApplicationContext::setup();

#if OGRE_MIN_VERSION(14, 0, 0)
        // X3D assumes two sided lighting
        Ogre::RTShader::ShaderGenerator::getSingleton().getRenderState(Ogre::MSN_SHADERGEN)->getSubRenderState(Ogre::RTShader::SRS_PER_PIXEL_LIGHTING)->setParameter("two_sided", "true");
#endif

        _trays.reset(new OgreBites::TrayManager("Interface", getRenderWindow()));
        addInputListener(_trays.get());
        _trays->showFrameStats(OgreBites::TL_BOTTOMLEFT);
        _trays->hideCursor();

        addInputListener(this);

        // register x3d file loader
        getRoot()->installPlugin(new X3D::OgreX3DPlugin);
    }

    void loop() {
#if OGRE_PLATFORM == OGRE_PLATFORM_EMSCRIPTEN
        emscripten_set_main_loop_arg(emloop, getRoot(), 0, 0);
#else
        getRoot()->startRendering();
#endif
    }

    void shutdown() override {
        getRoot()->removeFrameListener(_sai.get());
        removeInputListener(this);
        removeInputListener(_trays.get());
        removeInputListener(_controls.get());
        _controls.reset();
        _sai.reset();
        _trays.reset();
    }

    bool keyPressed(const OgreBites::KeyboardEvent& evt) override {
        using namespace OgreBites;
        switch(evt.keysym.sym) {
        case SDLK_ESCAPE:
            getRoot()->queueEndRendering();
            break;
        case 'b':
            _sai->switchDebugDrawing();
            break;
        case 'n':
            loadFile("../examples/flipper.x3d");
            break;
        case 'w':
            _camman->setYawPitchDist(Ogre::Radian(0), Ogre::Radian(0), _sai->getWorldSize());
            break;
        case 'x':
            auto comp = getNodeAttribute("vp", "compositors").empty() ? "Night Vision" : "";
            setNodeAttribute("vp", "compositors", comp);
            break;
        }

        return true;
    }
};
}

#if OGRE_PLATFORM == OGRE_PLATFORM_EMSCRIPTEN
EMSCRIPTEN_BINDINGS(my_module) {
    using namespace emscripten;
    class_<X3Ogre>("X3Ogre")
            .constructor()
            .function("loop", &X3Ogre::loop)
            .function("setNodeAttribute", &X3Ogre::setNodeAttribute)
            .function("getNodeAttribute", &X3Ogre::getNodeAttribute)
            .function("nodeExists", &X3Ogre::nodeExists)
            .function("loadFile", &X3Ogre::loadFile);
}
#else
int main(int argc, char* argv[]) {
    if (argc < 2) return 1;

    // Ogre setup
    X3Ogre context;
    context.loadFile(argv[1]);
    context.loop();

    return 0;

}
#endif
