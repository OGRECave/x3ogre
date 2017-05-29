x3ogre
======

x3ogre is an adapter of the declarative X3D concept to the Ogre rendering engine.
It maps the core X3D Nodes to Ogre primitives and also allows using Ogre resources inside X3D.

The goal is to load X3D scenes while using the efficient Ogre Meshes and more advanced rendering features available through Ogre.

Unlike other exiting X3D renderers, it offers true cross-platform compatibility being deployable to the Web, Desktop and Mobile.

Citing
------
```
@inproceedings{rojtberg2017x3ogre,
  title={x3ogre: connecting X3D to a state of the art rendering engine},
  author={Rojtberg, Pavel and Audenrith, Benjamin},
  booktitle={Proceedings of the 22nd International Conference on 3D Web Technology},
  pages={2},
  year={2017},
  organization={ACM}
}
```

Documentation
-------------
There are two declarative APIs available for usage with x3ogre, the declarative X3D and the Ogre Material System
* [X3D Documentation](http://www.web3d.org/files/specifications/19775-1/V3.3/index.html)
* [Ogre Material System Documentation](https://ogrecave.github.io/ogre/api/1.10/Material-Scripts.html)

For more details on x3ogre see
* [Inherent Limitations](docs/limitations.md)
* [Supported X3D Nodes](docs/x3d-nodes.md)
* [System Architecture](docs/system-architecture.md)

Contributing
-----------------
To get your code into master, make your changes in a new branch and [submit a merge request](../pulls).
The acceptance criteria are:

* separate your changes: only one feature per branch/ merge request
* no debug output in source code
* no commented out code without documentation
* use as little `#ifdef`s as possible. document why if you do.
* no project specific files, only examples
