Inherent Limitations
==============
there are following limitations in x3ogre. Some of them are due to underlying limitations on Ogre itself and some are necessary to make the rendering efficient.

* No Events. All logic is supposed to be on an higher level. (scripting language) Limited support for `onchanged` type events would be possible though.
* As a consequence: No general ROUTEs: you can only create Routes between a small set of predefined Nodes. This is needed for a decent performance. Yet animations are still resolved.
* No shearing support in Transform: i.e. the `scaleOrientation` field is only valid for multiples of 90°. However this field is hardly used anyway.
* No two sided lighting support (`TwoSidedMaterial`). DirectX lacks this and therefore Ogres Material lacks it too. Rarely used anyway, but could be emulated using the RTSS.
* There are some unresolved X3D compatibility issues. These are solvable but have a low priority right now.

Programming Interface
----------

SceneAccessInterface is the main Interface for interacting with X3D Scenes. Subset of the standard http://www.web3d.org/documents/specifications/19775-2/V3.3/Part02/servRef.html.

| Method | Description |
|--------|-------------|
| setNodeAttribute(node, field, value) | equivalent of [getNode][node](node).[getField][field](field).[setValue](http://www.web3d.org/documents/specifications/19775-2/V3.3/Part02/servRef.html#setValue)(value) |
| getNodeAttribute(node, field) | equivalent of [getNode][node](node).[getField][field](field).[getValue](http://www.web3d.org/documents/specifications/19775-2/V3.3/Part02/servRef.html#getValue)(value) |
| nodeExists(node) | equivalent of [getNode][node](node) != NULL |
| [loadURL](http://www.web3d.org/documents/specifications/19775-2/V3.3/Part02/servRef.html#loadURL) | load a X3D file (with x3ogre extensions) |
| [getCurrentFrameRate](http://www.web3d.org/documents/specifications/19775-2/V3.3/Part02/servRef.html#getCurrentFrameRate) ||

[node]: http://www.web3d.org/documents/specifications/19775-2/V3.3/Part02/servRef.html#getNode
[field]: http://www.web3d.org/documents/specifications/19775-2/V3.3/Part02/servRef.html#getField