# Maya Velocity Node

Calculates the instantaneous speed of an input point. Designed to be connected to a transform node, providing a technique for accurately calculating speed of a node.

In the future, this project will also provide the velocity vector and the (normalised) direction of velocity.

## Features

- Calculates speed of an input point attribute.
- Text Output for speed.
- Speed display unit is configurable - km/h, mph, etc.
- Configurable scale unit - centimetres, metres, etc.

## Usage

For an example usage, run the following Python code after installation.

```python
import maya.cmds

# Load and create velocity node.
maya.cmds.loadPlugin('velocity')
node = maya.cmds.createNode('velocity')
maya.cmds.connectAttr('time1.outTime', '%s.time' % node)

# transform node
tfm = maya.cmds.createNode('transform')
maya.cmds.connectAttr('%s.translate' % tfm, '%s.translate' % node)

# create 'speed' attribute
maya.cmds.addAttr(tfm, ln='speed', at='double' )
maya.cmds.setAttr('%s.speed' % tfm, edit=True, keyable=True)
maya.cmds.connectAttr('%s.outSpeed' % node, '%s.speed' % tfm)

# Add example animation
maya.cmds.setKeyframe(tfm, attribute='translateX', time=0, value=0)
maya.cmds.setKeyframe(tfm, attribute='translateX', time=24, value=100)
```

_See 'test.py' for more details_

## Building and Install

To build and install the plug-in, follow the steps below.

## Dependencies

- C++ compiler with support for C++11
- CMake 2.6+
- Autodesk Maya 2016+

## Build and Install

A build assumes the CMakeLists.txt is set up correctly MAYA_ROOT are set correctly. 
  
Run the following in a Bash-like shell.

### Build

```commandline
$ cd <project root>
$ mkdir build
$ cd build
$ cmake ..
$ make -j 4
```

### Install

```commandline
$ cd <project root>/build
$ mkdir ~/maya/<maya version>/plug-ins
$ cp velocity.so ~/maya/<maya version>/plug-ins
```

You may now open Autodesk Maya and load the plugin using the "maya.cmds.loadPlugin" command (see Usage section above). 

## Limitations and Known Bugs 

- None known.