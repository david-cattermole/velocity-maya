# Test script for velocity node.
import maya.cmds

maya.cmds.file(new=True, force=True)
maya.cmds.unloadPlugin('velocityNode')
maya.cmds.loadPlugin('velocityNode')
node = maya.cmds.createNode('velocity')
maya.cmds.setAttr('%s.displayUnit' % node, 3)
maya.cmds.connectAttr('time1.outTime', '%s.time' % node)

tfm = maya.cmds.createNode('transform')
shp = maya.cmds.createNode('locator', parent=tfm)

maya.cmds.connectAttr('%s.translate' % tfm, '%s.translate' % node)

maya.cmds.addAttr(tfm, ln='speed', at='double' )
maya.cmds.setAttr('%s.speed' % tfm, edit=True, keyable=True)
maya.cmds.connectAttr('%s.outSpeed' % node, '%s.speed' % tfm)

annNode = maya.cmds.createNode('annotationShape', parent=tfm)
maya.cmds.setAttr('%s.displayArrow' % annNode, 0)
maya.cmds.setAttr('%s.overrideEnabled' % annNode, 1)
maya.cmds.setAttr('%s.overrideColor' % annNode, 14)

maya.cmds.connectAttr('%s.outSpeedText' % node, '%s.text' % annNode)

maya.cmds.setKeyframe(tfm, attribute='translateX', time=0, value=0)
maya.cmds.setKeyframe(tfm, attribute='translateX', time=24, value=100)
maya.cmds.keyTangent(tfm, inTangentType='linear', outTangentType='linear', time=())
maya.cmds.setInfinity(pri='linear', poi='linear')
