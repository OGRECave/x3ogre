Relation of X3D Nodes to Ogre Nodes
===========================
| X3D | Ogre |
|--------|---------|
| Appearance | Material |
| Geometry | SubMesh |
| Scene | SceneManager |

x3ogre Node hierarchy
================
| Node | Description | Aggregates |
|---------|-------------------|------------------|
| Scene |  Contains Nodes forming a single View. e.g. a Room, content of a TV screen | Light, Viewpoint, Shape, Transform ... |
| Group |  Node which holds child Nodes | Shape, Transform, Group |
| Transform |  Group Node which changes Rotation and Translation of all child Nodes | Shape, Transform, Group |

x3d Functionality in x3ogre
================
## DEF/USE 
If a node in the x3d file has a `DEF` or `id` field it will be registered at the world node (see void World::registerNode()). If the node is in a namespace (e.g. it is part of a script loaded via an Inline node) then the name of the registered node will have a prefix according to the namespace (e.g. nodeName = "Test", namespace = "World001" --> registeredNodeName = "World001Test"). If two nodes have the same name, this will lead to an exception. 

In the common case the parser creates a new X3DObject for every node in the x3d file. This is also the case if the node in the x3d file has a "USE" tag but the created X3DObject will be overwritten by the object stored before (see "DEF" tag).

### Use in Inline
If the used node is a Inline node the USE command will use the Inline node specified before and reparses the content of the specified URL.

### Use in Transform
If a transform is used, the parser in the first step just uses the Transform stored in the DEF step before. If it is attached to a Ogre::SceneNode (see Transform::attachTo()) a child node of this SceneNode (called TopNode) will be created an pushed onto a list. The Group of the transform (containing all the objects) will be attached to a childNode of the stored node (called groupParent). This attaches all the objects of the group to the groupParent. If one child is again a Transform this is done recursively, if it is a Shape the groupParent is added to a list. In the initialization step, the Geometry of the shape is added to all SceneNodes in that list.

All changes of the Transform are propagated to all TopNodes (and thus to all groupParents, child nodes, ...)

## Animations 
In general there are two types of possible animations implemented yet:
* Animations of Transform
* Animations of IndexedFaceSet

The general difference between the two types is the field of application. While animations for IndexedFaceSet only modify the position of the edges of a 3D object, the animation of a Transform changes the Position respectively the Orientation of a Transform an thus these values for all children (and their children and so on).

An animation is implemented in Ogre as a Track of KeyFrames, one KeyFrame for a specific time step. The values between these KeyFrames are interpolated. In X3Ogre we have three different Interpolators which behave like that: 
* CoordinateInterpolator (Animation of a mesh)
* OrientationInterpolator (Rotation of a Transform)
* PositionInterpolator (Translation of a Transform)

To control these Interpolators we have the AnimationController. It produces a value between 0 and 1 and applies the corresponding KeyFrame (respectively the interpolation of two KeyFrames). There are two types of Controller:
* TimeSensor (which can be a loop with specified time interval or a one-shot) 
* ScalarInterpolator (which maps the values of a TimeSensor to other values - so you can e.g. have parts of the animation which are moving faster and other parts which are moving slower)


### Animations of Transform
In the Transform all SceneNodes which are associated to that Transform are stored in a list. Thus one animation in Ogre can only manipulate one SceneNode there are two ways of propagating the changes to all nodes:
* One Animation for every Scene node
* Only one Animation but this SceneNode has a Listener which propagates the information to all other nodes in the Transform

Because of computational reasons the second approach is chosen. The first node in the the list will get a ClonePropagationListener which calls the Transform::updateSceneNode() function every time this node was changed. All animations (possibly a PositionInterpolator and/or an OrientationInterpolator) are done to this SceneNode.

In the Transform::updateSceneNode the values of the changed SceneNode will be propagated to all the other (This contains until now translation and orientation).

Some animations stop updating the SceneNodes if the change is propagated to all SceneNode of the same Transform. There is an additional function in DefaultViewer to force updates.

### Animations of IndexedFaceSet
The animation of an IndexedFaceSet is applied to a mesh. This is done in IndexedFaceSet::createMesh(). 

## Inline
A Inline node loads another x3d file specified by the URL of the inline node. This file is parsed as a regular file. In the hierarchy the root nodes of the parsed file are added to the parent node of Inline node. So the Inline node does not appear in the SceneNode graph. 

In the parsing step the used function is the same as for regular parsing of a x3d file except that the root is not the X3D::Scene but the parent scene node of the inline node, nameSpace is set (so every node from this file which has a name, gets this as a prefix for specification).

Resource handling in x3ogre
================
In Ogre there are classes specified for Resource handling: Ogre::ResourceManager. They have among other things specified script patterns. Whenever a possible resource location is scanned and a file with the specific pattern is found, the ResourceManager can get active.

To obtain this the ResourceManager has to be registered at the Ogre::ResourceGroupManager (See X3DFileManager::X3DFileManager()). After the ResourceGroupManager is initialized (See Viewer::loadFile) the ResourceManager is aware of the file and can possibly load it on demand (Note: It has been tried to use Ogre::ScriptLoader instead of Ogre::ResourceManager but this would parse all files in all ResourceGroups during the initialization. Such we need lazy loading we are using Ogre::ResourceManager).

If a File is loaded the sequence of called commands is:
+ X3DFileManager::createImpl() - Creates a X3DFile object and adds this to the resource list
+ X3DFile::loadImpl() - Tests if the file exists, loads it and calls X3DFile::parseFile
+ X3DFile::parseFile() - Calls the X3DParser

Shader use in x3ogre
================
Derived from x3D x3ogre combines different ShaderParts to one ComposedShader. The underlying Ogre API needs a Ogre::Material where the corresponding parameters are set. To apply a shader to an Object the Composed Shader needs underlying ShaderParts (added via addChild(...)) and Fields which specify the input parameter of the shader. Some standard parameters are set for the vertex shader in ComposedShader::apply(...).
