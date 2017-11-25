#include <iostream>

#include <OgreApplicationContext.h>
#include <core/SceneAccessInterface.h>

#include <OgreOverlaySystem.h>
#include <OgreTrays.h>
#include <OgreAdvancedRenderControls.h>
#include <OgreCameraMan.h>

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

    Ogre::SceneNode::ObjectIterator mo = node->getAttachedObjectIterator();
    while(mo.hasMoreElements()) {
        std::cout << tabs+"\t" << mo.getNext()->getName() << std::endl;
    }

    Ogre::SceneNode::ChildNodeIterator j = node->getChildIterator();
    while(j.hasMoreElements()) {
        printSceneGraph(static_cast<Ogre::SceneNode*>(j.getNext()), tabs+"\t");
    }
}

struct X3Ogre : public OgreBites::ApplicationContext, OgreBites::InputListener {
    std::unique_ptr<X3D::SceneAccessInterface> _sai;
    std::unique_ptr<OgreBites::TrayManager> _trays;
    std::unique_ptr<OgreBites::AdvancedRenderControls> _controls;
    std::unique_ptr<OgreBites::CameraMan> _camman;

    bool _leftIsPressed = false;
    bool _rightIsPressed = false;
    int _oldXleftKey=0, _oldYleftKey=0;
    int _oldXrightKey=0, _oldYrightKey=0;

    X3Ogre() : OgreBites::ApplicationContext("x3ogre", false) {
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
        if(_controls) {
            removeInputListener(_controls.get());
            _controls.reset();
        }

        if(_camman) {
            removeInputListener(_camman.get());
            _camman.reset();
        }

        _sai->loadURL(file);
        _sai->setWindow(getRenderWindow());
        _sai->sceneManager()->addRenderQueueListener(getOverlaySystem());

        auto cam =_sai->scene()->bound<X3D::Viewpoint>()->getCamera();
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

        _trays.reset(new OgreBites::TrayManager("Interface", getRenderWindow()));
        addInputListener(_trays.get());
        _trays->showFrameStats(OgreBites::TL_BOTTOMLEFT);
        _trays->hideCursor();
        SDL_ShowCursor(SDL_TRUE);

        // SAI init
        _sai.reset(new X3D::SceneAccessInterface(getRoot()));
        addInputListener(this);
    }

    void loop() {
#if OGRE_PLATFORM == OGRE_PLATFORM_EMSCRIPTEN
        emscripten_set_main_loop_arg(emloop, getRoot(), 0, 0);
#else
        getRoot()->startRendering();
#endif
    }

    void shutdown() override {
        removeInputListener(this);
        removeInputListener(_trays.get());
        removeInputListener(_controls.get());
        _controls.reset();
        _sai.reset();
        _trays.reset();
    }

    bool keyPressed(const OgreBites::KeyboardEvent& evt) override {
        switch(evt.keysym.sym) {
        case SDLK_ESCAPE:
            getRoot()->queueEndRendering();
            break;
        case 'b':
            _sai->switchDebugDrawing();
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
