/*
 *
 */

#include <velocityNode.h>
#include <utilities/debugUtils.h>

#include <cstring>
#include <cmath>

MTypeId velocityNode::id(kVelocityNodeTypeId);

MObject velocityNode::aInputPoint;
MObject velocityNode::aInputPointX;
MObject velocityNode::aInputPointY;
MObject velocityNode::aInputPointZ;
MObject velocityNode::aTime;
MObject velocityNode::aTimeInterval;
MObject velocityNode::aFramesPerSecond;
MObject velocityNode::aTextPrecision;
MObject velocityNode::aUnitScale;
MObject velocityNode::aDisplayUnit;
MObject velocityNode::aOutSpeed;
MObject velocityNode::aOutSpeedText;
//MObject velocityNode::aOutDirection;
//MObject velocityNode::aOutDirectionX;
//MObject velocityNode::aOutDirectionY;
//MObject velocityNode::aOutDirectionZ;
//MObject velocityNode::aOutVelocity;
//MObject velocityNode::aOutVelocityX;
//MObject velocityNode::aOutVelocityY;
//MObject velocityNode::aOutVelocityZ;


velocityNode::velocityNode() {}

velocityNode::~velocityNode() {}


MStatus velocityNode::compute(const MPlug &plug, MDataBlock &data) {
    MStatus status = MS::kUnknownParameter;
    MObject thisNode = thisMObject();
    MFnDependencyNode fnThisNode(thisNode, &status);
    if (status != MS::kSuccess) {
        ERR("velocityNode::compute: MFnDependencyNode failed.");
        return MS::kUnknownParameter;
    }

    if ((plug == aOutSpeed) || (plug == aOutSpeedText)) {
        // Get the translate plug creating the MDataHandle with a DG context.
        MPlug pointPlug(thisNode, aInputPoint);

        // Get Data Handles
        MDataHandle timeHandle = data.inputValue(aTime);
        MDataHandle timeIntervalHandle = data.inputValue(aTimeInterval);
        MDataHandle fpsHandle = data.inputValue(aFramesPerSecond);
        MDataHandle precisionHandle = data.inputValue(aTextPrecision);
        MDataHandle pointNowHandle = data.inputValue(aInputPoint);

        // Get value
        MTime time = timeHandle.asTime();
        MTime timeInterval = timeIntervalHandle.asTime();
        double interval = timeInterval.asUnits(MTime::uiUnit()) * 2;
        double fps = fpsHandle.asDouble();
        int precision = precisionHandle.asInt();
        MVector pointNow = pointNowHandle.asVector();

        // Point at previous frame
        MDGContext dgContextPrev(time - timeInterval);
        MDataHandle pointPrevHandle = pointPlug.asMDataHandle(dgContextPrev, &status);
        MVector pointPrev = pointPrevHandle.asVector();

        // Point at next frame
        MDGContext dgContextNext(time + timeInterval);
        MDataHandle pointNextHandle = pointPlug.asMDataHandle(dgContextNext, &status);
        MVector pointNext = pointNextHandle.asVector();

        // Scale
        MDataHandle scaleHandle = data.inputValue(aUnitScale);
        double scaleFactor = 1.0;
        short scale = scaleHandle.asShort();
        switch (scale) {
            case kScaleMillimetres:
                scaleFactor = 0.001;
                break;
            case kScaleCentimetres:
                scaleFactor = 0.01;
                break;
            case kScaleDecimetres:
                scaleFactor = 0.1;
                break;
            default:
            case kScaleMetres:
                scaleFactor = 1.0;
                break;
            case kScaleKilometres:
                scaleFactor = 1000.0;
                break;
        }

        // Display Unit
        MDataHandle displayUnitHandle = data.inputValue(aDisplayUnit);
        MString displayUnitStr = "";
        double displayUnitFactor = 0.0;
        short displayUnit = displayUnitHandle.asShort();
        switch (displayUnit) {
            default:
            case kDisplayUnitKmPerHour:
                // km per hour
                displayUnitFactor = fps * 60 * 60 * 0.001;
                displayUnitStr = " km/h";
                break;
            case kDisplayUnitMilesPerHour:
                // miles per hour
                displayUnitFactor = fps * 60 * 60 * 0.000621371192;
                displayUnitStr = " mph";
                break;
            case kDisplayUnitMetresPerHour:
                displayUnitFactor = fps * 60 * 60;
                displayUnitStr = " m/h";
                break;
            case kDisplayUnitMetresPerSecond:
                displayUnitFactor = fps;
                displayUnitStr = " m/s";
                break;
        }

        // Distance
        double dx, dy, dz;
        double instantaneousSpeed = 0.0;
        dx = pointNow[0] - pointPrev[0];
        dy = pointNow[1] - pointPrev[1];
        dz = pointNow[2] - pointPrev[2];
        instantaneousSpeed = sqrt((dx * dx) + (dy * dy) + (dz * dz));
        dx = pointNow[0] - pointNext[0];
        dy = pointNow[1] - pointNext[1];
        dz = pointNow[2] - pointNext[2];
        instantaneousSpeed += sqrt((dx * dx) + (dy * dy) + (dz * dz));
        instantaneousSpeed = (instantaneousSpeed * scaleFactor * displayUnitFactor) / interval;

        // Output Speed
        MDataHandle outSpeedHandle = data.outputValue(aOutSpeed);
        outSpeedHandle.setDouble(instantaneousSpeed);
        outSpeedHandle.setClean();

        // Output Speed String
        MString speedStr;
        speedStr.set(instantaneousSpeed, precision);
        speedStr += displayUnitStr;
        MDataHandle outVelTextHandle = data.outputValue(aOutSpeedText);
        outVelTextHandle.setString(speedStr);
        outVelTextHandle.setClean();

        data.setClean(plug);
    } else {
        return MS::kUnknownParameter;
    }
    return MS::kSuccess;
}


void *velocityNode::creator() {
    return (new velocityNode());
}


MStatus velocityNode::initialize() {
    MStatus status;
    MFnUnitAttribute unitFn;
    MFnTypedAttribute typedAttr;
    MFnNumericAttribute numericAttr;
    MFnEnumAttribute enumAttr;

    // Input Point X
    aInputPointX = numericAttr.create(
            "inputPointX", "ipx",
            MFnNumericData::kDouble, 0.0);
    CHECK_MSTATUS(numericAttr.setStorable(true));
    CHECK_MSTATUS(numericAttr.setKeyable(true));
    CHECK_MSTATUS(addAttribute(aInputPointX));

    // Input Point Y
    aInputPointY = numericAttr.create(
            "inputPointY", "ipy",
            MFnNumericData::kDouble, 0.0);
    CHECK_MSTATUS(numericAttr.setStorable(true));
    CHECK_MSTATUS(numericAttr.setKeyable(true));
    CHECK_MSTATUS(addAttribute(aInputPointY));

    // Input Point Z
    aInputPointZ = numericAttr.create(
            "inputPointZ", "ipz",
            MFnNumericData::kDouble, 0.0);
    CHECK_MSTATUS(numericAttr.setStorable(true));
    CHECK_MSTATUS(numericAttr.setKeyable(true));
    CHECK_MSTATUS(addAttribute(aInputPointZ));

    // Input Point (parent of input Point* attributes)
    aInputPoint = numericAttr.create(
            "inputPoint", "ip",
            aInputPointX,
            aInputPointY,
            aInputPointZ);
    CHECK_MSTATUS(numericAttr.setStorable(true));
    CHECK_MSTATUS(numericAttr.setKeyable(true));
    CHECK_MSTATUS(addAttribute(aInputPoint));

    // Time
    aTime = unitFn.create("time", "time", MFnUnitAttribute::kTime, 0.0);
    CHECK_MSTATUS(addAttribute(aTime));

    // Interval
    aTimeInterval = unitFn.create(
            "timeInterval", "timeInterval",
            MFnUnitAttribute::kTime, 1.0);
    CHECK_MSTATUS(addAttribute(aTimeInterval));

    // Frames Per Second
    aFramesPerSecond = numericAttr.create(
            "framesPerSecond", "framesPerSecond",
            MFnNumericData::kDouble, 24.0);
    CHECK_MSTATUS(numericAttr.setStorable(true));
    CHECK_MSTATUS(numericAttr.setKeyable(true));
    CHECK_MSTATUS(addAttribute(aFramesPerSecond));

    // Text Precision
    aTextPrecision = numericAttr.create(
            "textPrecision", "textPrecision",
            MFnNumericData::kInt, 3);
    CHECK_MSTATUS(numericAttr.setStorable(true));
    CHECK_MSTATUS(numericAttr.setKeyable(true));
    CHECK_MSTATUS(addAttribute(aTextPrecision));

    // Scene Scale
    aUnitScale = enumAttr.create(
            "scale", "scale",
            kScaleCentimetres);
    enumAttr.addField("millimetres", kScaleMillimetres);
    enumAttr.addField("centimetres", kScaleCentimetres);
    enumAttr.addField("metres", kScaleMetres);
    enumAttr.addField("decimetres", kScaleDecimetres);
    enumAttr.addField("kilometres", kScaleKilometres);
    CHECK_MSTATUS(addAttribute(aUnitScale));

    // Display Unit
    aDisplayUnit = enumAttr.create(
            "displayUnit", "displayUnit",
            kDisplayUnitKmPerHour);
    enumAttr.addField("km/h", kDisplayUnitKmPerHour);
    enumAttr.addField("mph", kDisplayUnitMilesPerHour);
    enumAttr.addField("m/h", kDisplayUnitMetresPerHour);
    enumAttr.addField("m/s", kDisplayUnitMetresPerSecond);
    CHECK_MSTATUS(addAttribute(aDisplayUnit));

    // Out Speed
    aOutSpeed = numericAttr.create(
            "outSpeed", "outSpeed",
            MFnNumericData::kDouble, 0.0);
    CHECK_MSTATUS(numericAttr.setStorable(false));
    CHECK_MSTATUS(numericAttr.setKeyable(false));
    CHECK_MSTATUS(numericAttr.setReadable(true));
    CHECK_MSTATUS(numericAttr.setWritable(false));
    CHECK_MSTATUS(addAttribute(aOutSpeed));

    // Out Speed Text
    MStatus stat2;
    MFnStringData stringData;
    MObject stringObj = stringData.create(&stat2);
    CHECK_MSTATUS(stat2);
    aOutSpeedText = typedAttr.create(
            "outSpeedText", "outSpeedText",
            MFnData::kString, stringObj, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(addAttribute(aOutSpeedText));

    CHECK_MSTATUS(attributeAffects(aInputPoint, aOutSpeed));
    CHECK_MSTATUS(attributeAffects(aInputPoint, aOutSpeedText));
    CHECK_MSTATUS(attributeAffects(aTime, aOutSpeed));
    CHECK_MSTATUS(attributeAffects(aTime, aOutSpeedText));
    CHECK_MSTATUS(attributeAffects(aTimeInterval, aOutSpeed));
    CHECK_MSTATUS(attributeAffects(aTimeInterval, aOutSpeedText));
    CHECK_MSTATUS(attributeAffects(aFramesPerSecond, aOutSpeed));
    CHECK_MSTATUS(attributeAffects(aFramesPerSecond, aOutSpeedText));
    CHECK_MSTATUS(attributeAffects(aTextPrecision, aOutSpeedText));
    CHECK_MSTATUS(attributeAffects(aUnitScale, aOutSpeed));
    CHECK_MSTATUS(attributeAffects(aUnitScale, aOutSpeedText));
    CHECK_MSTATUS(attributeAffects(aDisplayUnit, aOutSpeed));
    CHECK_MSTATUS(attributeAffects(aDisplayUnit, aOutSpeedText));

    return (MS::kSuccess);
}

