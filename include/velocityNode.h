/*
 *
 */

#ifndef VELOCITY_NODE_H
#define VELOCITY_NODE_H

#include <maya/MPxNode.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MString.h>
#include <maya/MVector.h>
#include <maya/MTime.h>

#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnEnumAttribute.h>

#define kVelocityNodeName "velocity"
#define kVelocityNodeTypeId 0x80028

#define kScaleMillimetres 0
#define kScaleCentimetres 1
#define kScaleMetres 2
#define kScaleDecimetres 3
#define kScaleKilometres 4

#define kDisplayUnitKmPerHour 0
#define kDisplayUnitMilesPerHour 1
#define kDisplayUnitMetresPerHour 2
#define kDisplayUnitMetresPerSecond 3

class velocityNode : public MPxNode {
public:
    velocityNode();

    virtual ~velocityNode();

    virtual MStatus compute(const MPlug &plug, MDataBlock &data);

    static void *creator();

    static MStatus initialize();

    static MTypeId id;

    static MObject aInputPoint;
    static MObject aInputPointX;
    static MObject aInputPointY;
    static MObject aInputPointZ;
    static MObject aTime;
    static MObject aTimeInterval;
    static MObject aFramesPerSecond;
    static MObject aUnitScale;
    static MObject aDisplayUnit;
    static MObject aTextPrecision;
    static MObject aOutSpeed;
    static MObject aOutSpeedText;
//    static MObject aOutDirection;
//    static MObject aOutDirectionX;
//    static MObject aOutDirectionY;
//    static MObject aOutDirectionZ;
//    static MObject aOutVelocity;
//    static MObject aOutVelocityX;
//    static MObject aOutVelocityY;
//    static MObject aOutVelocityZ;
};


#endif // VELOCITY_NODE_H
