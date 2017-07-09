/*
 *
 */


#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MPxNode.h>

#include <velocityNode.h>

MStatus initializePlugin(MObject obj) {
    MStatus status;
    MFnPlugin plugin(obj, PLUGIN_COMPANY, "6.0", "Any");

    status = plugin.registerNode(
            kVelocityNodeName,
            velocityNode::id,
            velocityNode::creator,
            velocityNode::initialize);
    if (!status) {
        status.perror("velocityNode: registerNode");
        return (status);
    }

    return (status);
}

MStatus uninitializePlugin(MObject obj) {
    MStatus status;
    MFnPlugin plugin(obj);

    status = plugin.deregisterNode(velocityNode::id);
    if (!status) {
        status.perror("velocityNode: deregisterNode");
        return (status);
    }

    return (status);
}
