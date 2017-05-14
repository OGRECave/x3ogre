Animation
==================================
| Node | Notes |
|---------|------------------|
|[CoordinateInterpolator](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/interp.html#CoordinateInterpolator)|can be only connected to Geometry Nodes|
|[OrientationInterpolator](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/interp.html#OrientationInterpolator)|  |
|[PositionInterpolator](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/interp.html#PositionInterpolator)|  |
|[ScalarInterpolator](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/interp.html#ScalarInterpolator)|  |
|[TimeSensor](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/time.html#TimeSensor)|  |

Appearance
==================================
| Node | Description |
|---------|------------------|
|[Appearance](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/shape.html#Appearance)| To make use of the Ogre Material scripts, Appearance nodes can use such scripts if the `DEF` tag has a value which specifies a Material name. Appearance Nodes also can have RTShaderSystem nodes as children which specify additional use of specific shader snippets.|
|[ComposedShader](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/shaders.html#ComposedShader)| Can have Field nodes as children which specify parameters of the ShaderParts. |
|Field| Not contained in the ISO standard. Child node of Composed shader. This node specifies parameters of the shader. Therefore it has a `name`, a `type`, and a `value` tag. The name has to be the same as the variable used in the shader. The type must specify what kind of variable this is. Currently only `sfint32`, `sffloat`, and `sfvec3f` are supported. Value contains the content of the variable (e.g. a int, a float or three whitespace separated floats) |
|[ImageTexture](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/texturing.html#ImageTexture)| It can also have RTShaderSystem nodes as children which can specify behaviour of the Texture. |
|[Material](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/shape.html#Material)|  |
|RTShaderSystem| Not Contained in the ISO standard. Enables the use of the Real Time Shading System from Ogre. The behaviour is the same as the rtshader_system parts in the Ogre material scripts. The first part has to be specified as property, all values in one 'value' string separated by commas. Its a child node of Appearance or ImageTexture nodes|
|[ShaderPart](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/shaders.html#ShaderPart)|  |

Geometry
==================================
| Node | Notes |
|---------|------------------|
|[Box](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/geometry3D.html#Box)|  |
|[Color](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/rendering.html#Color)|  |
|[Cone](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/geometry3D.html#Cone)| Doesn't have boolean fields for specifying the appearance: `bottom`, and `side` |
|[Coordinate](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/rendering.html#Coordinate)|  |
|[Cylinder](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/geometry3D.html#Cylinder)| Doesn't have boolean fields for specifying the appearance: `bottom`, `top` and `side` |
|[IndexedFaceSet](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/geometry3D.html#IndexedFaceSet)| Only works with triangles for now. (bug #1) |
|[IndexedLineSet](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/rendering.html#IndexedLineSet)| Doesn't implement the color values |
|[IndexedPrimitiveSet](http://doc.instantreality.org/documentation/nodetype/IndexedPrimitiveSet/)| Not contained in the X3D standard. Follows InstantReality as parent of all IndexedSets. |
|[IndexedTriangleSet](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/rendering.html#IndexedTriangleSet)|  |
|[Inline](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/networking.html#Inline)| Group node, primarily to hold the URL of a file. This URL is read in and parsed by X3DFile object. Details see [system architecture](system-architecture#inline). All Details specified in the inline script are default active | 
|[Normal](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/rendering.html#Normal)|  |
|[Rectangle2D](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/geometry2D.html#Rectangle2D)|  |
|[Shape](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/shape.html#Shape)| `bbox*` fields are ignored and autocomputed by viewer |
|[Sphere](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/geometry3D.html#Sphere)|  |
|[TextureCoordinate](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/texturing.html#TextureCoordinate)|  |

World
==================================
| Node | Description |
|---------|------------------|
|[Background](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/enveffects.html#Background)| Only implements the `skyColor`. If ImageBackground is present, it will be preferred. |
|[DirectionalLight](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/lighting.html#DirectionalLight)| The `ambientIntensity` and the `global` field are not implemented. |
|[Group](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/group.html#Group)| Doesn't implement the bounding box. |
|[ImageBackground](http://doc.instantreality.org/documentation/nodetype/ImageBackground/)|  	Not contained in the X3D standard. Follows InstantReality. Only implements `Color`, `Texture`, and `TexCoords` (buggy). BackgroundColor is only visible, if texture is not found. Is preferred over Background. |
|[NavigationInfo](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/navigation.html#NavigationInfo)| Only implements the `headlight` field from the standard. Has additional field (control) which gets a node ID. If it's specified this node will be controlled by the input system instead of the active viewpoint. If it's not specified, the active viewpoint will be controlled. |
|[Route](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/core.html#ROUTEStatement)|you can only create Routes between a small set of predefined Nodes|
|[Scene](http://doc.instantreality.org/documentation/nodetype/Scene/) | Not contained in the X3D standard. Follows InstantReality |
|[Transform](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/group.html#Transform) |No shearing support. The `scaleOrientation` is only valid for multiples of 90°. `bbox*` fields are ignored and autocomputed by viewer|
|[Viewpoint](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/navigation.html#Viewpoint)| Only implements `FieldOfView`, `Orientation` and `Position`. Orientation is coded as an Quaternion. |
|[WorldInfo](http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/core.html#WorldInfo)| Doesn't implement the `info` field. |