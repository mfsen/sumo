/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2021 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    AdditionalHandler.cpp
/// @author  Jakob Erdmann
/// @date    Feb 2015
///
// The XML-Handler for network loading
/****************************************************************************/
#include <config.h>

#include <utils/common/MsgHandler.h>
#include <utils/xml/XMLSubSys.h>
#include <utils/common/SUMOVehicleClass.h>
#include <utils/common/RGBColor.h>

#include "AdditionalHandler.h"


// ===========================================================================
// method definitions
// ===========================================================================

AdditionalHandler::AdditionalHandler(const std::string& file) :
    SUMOSAXHandler(file) {
}


AdditionalHandler::~AdditionalHandler() {}


bool
AdditionalHandler::parse() {
    // run parser and save result
    const bool parserResult = XMLSubSys::runParser(*this, getFileName());
    // now parse over SumoBaseObjects
    if (myCommonXMLStructure.getSumoBaseObjectRoot()) {
        parseSumoBaseObject(myCommonXMLStructure.getSumoBaseObjectRoot());
    }

    return parserResult;
}


void
AdditionalHandler::myStartElement(int element, const SUMOSAXAttributes& attrs) {
    // obtain tag
    const SumoXMLTag tag = static_cast<SumoXMLTag> (element);
    // check tag
    try {
        switch (tag) {
            // Stopping Places
            case SUMO_TAG_BUS_STOP:
            case SUMO_TAG_TRAIN_STOP:
                parseBusStopAttributes(attrs);
                break;
            case SUMO_TAG_ACCESS:
                parseAccessAttributes(attrs);
                break;
            case SUMO_TAG_CONTAINER_STOP:
                parseContainerStopAttributes(attrs);
                break;
            case SUMO_TAG_CHARGING_STATION:
                parseChargingStationAttributes(attrs);
                break;
            case SUMO_TAG_PARKING_AREA:
                parseParkingAreaAttributes(attrs);
                break;
            case SUMO_TAG_PARKING_SPACE:
                parseParkingSpaceAttributes(attrs);
                break;
            // Detectors
            case SUMO_TAG_E1DETECTOR:
            case SUMO_TAG_INDUCTION_LOOP:
                parseE1Attributes(attrs);
                break;
            case SUMO_TAG_E2DETECTOR:
            case SUMO_TAG_E2DETECTOR_MULTILANE:
            case SUMO_TAG_LANE_AREA_DETECTOR:
                parseE2Attributes(attrs);
                break;
            case SUMO_TAG_E3DETECTOR:
            case SUMO_TAG_ENTRY_EXIT_DETECTOR:
                parseE3Attributes(attrs);
                break;
            case SUMO_TAG_DET_ENTRY:
                parseEntryAttributes(attrs);
                break;
            case SUMO_TAG_DET_EXIT:
                parseExitAttributes(attrs);
                break;
            case SUMO_TAG_INSTANT_INDUCTION_LOOP:
                parseE1InstantAttributes(attrs);
                break;
            // TAZs
            case SUMO_TAG_TAZ:
                parseTAZAttributes(attrs);
                break;
            case SUMO_TAG_TAZSOURCE:
                parseTAZSourceAttributes(attrs);
                break;
            case SUMO_TAG_TAZSINK:
                parseTAZSinkAttributes(attrs);
                break;
            // Variable Speed Sign
            case SUMO_TAG_VSS:
                parseVariableSpeedSignAttributes(attrs);
                break;
            case SUMO_TAG_STEP:
                parseVariableSpeedSignStepAttributes(attrs);
                break;
            // Calibrator
            case SUMO_TAG_CALIBRATOR:
            case SUMO_TAG_LANECALIBRATOR:
                parseCalibratorAttributes(attrs);
                break;
            case SUMO_TAG_FLOW_CALIBRATOR:
                parseCalibratorFlowAttributes(attrs);
                break;
            // Rerouter
            case SUMO_TAG_REROUTER:
                parseRerouterAttributes(attrs);
                break;
            case SUMO_TAG_INTERVAL:
                parseRerouterIntervalAttributes(attrs);
                break;
            case SUMO_TAG_CLOSING_LANE_REROUTE:
                parseClosingLaneRerouteAttributes(attrs);
                break;
            case SUMO_TAG_CLOSING_REROUTE:
                parseClosingRerouteAttributes(attrs);
                break;
            case SUMO_TAG_DEST_PROB_REROUTE:
                parseDestProbRerouteAttributes(attrs);
                break;
            case SUMO_TAG_PARKING_ZONE_REROUTE:
                parseParkingAreaRerouteAttributes(attrs);
                break;
            case SUMO_TAG_ROUTE_PROB_REROUTE:
                parseRouteProbRerouteAttributes(attrs);
                break;
            // Route probe
            case SUMO_TAG_ROUTEPROBE:
                parseRouteProbeAttributes(attrs);
                break;
            // Vaporizer (deprecated)
            case SUMO_TAG_VAPORIZER:
                parseVaporizerAttributes(attrs);
                break;
            // parameters
            case SUMO_TAG_PARAM:
                parseParameters(attrs);
                break;
            default:
                break;
        }
    } catch (InvalidArgument& e) {
        WRITE_ERROR(e.what());
    }
}


void
AdditionalHandler::myEndElement(int element) {
    // get object
    CommonXMLStructure::SumoBaseObject* obj = myCommonXMLStructure.getLastInsertedSumoBaseObject();
    // check object
    if (obj) {
        // obtain tag
        const SumoXMLTag tag = static_cast<SumoXMLTag> (element);
        // check tag
        switch (tag) {
            // Stopping Places
            case SUMO_TAG_BUS_STOP:
            case SUMO_TAG_TRAIN_STOP:
            case SUMO_TAG_ACCESS:
            case SUMO_TAG_CONTAINER_STOP:
            case SUMO_TAG_CHARGING_STATION:
            case SUMO_TAG_PARKING_AREA:
            // detectors
            case SUMO_TAG_E1DETECTOR:
            case SUMO_TAG_INDUCTION_LOOP:
            case SUMO_TAG_E2DETECTOR:
            case SUMO_TAG_E2DETECTOR_MULTILANE:
            case SUMO_TAG_LANE_AREA_DETECTOR:
            case SUMO_TAG_E3DETECTOR:
            case SUMO_TAG_ENTRY_EXIT_DETECTOR:
            case SUMO_TAG_INSTANT_INDUCTION_LOOP:
            // TAZs
            case SUMO_TAG_TAZ:
            // Variable Speed Sign
            case SUMO_TAG_VSS:
            // Calibrator
            case SUMO_TAG_CALIBRATOR:
            case SUMO_TAG_LANECALIBRATOR:
            // Rerouter
            case SUMO_TAG_REROUTER:
            // Route probe
            case SUMO_TAG_ROUTEPROBE:
            // Vaporizer (deprecated)
            case SUMO_TAG_VAPORIZER:
                // parse object and all their childrens
                parseSumoBaseObject(obj);
                // just close node
                myCommonXMLStructure.closeTag();
                // delete object
                delete obj;
                break;
            default:
                // just close node
                myCommonXMLStructure.closeTag();
                break;
        }
    }
}


void
AdditionalHandler::parseBusStopAttributes(const SUMOSAXAttributes& attrs) {
    // declare Ok Flag
    bool parsedOk = true;
    // now obtain attributes
    const std::string id = attrs.get<std::string>(SUMO_ATTR_ID, "", parsedOk, false);
    const std::string laneId = attrs.get<std::string>(SUMO_ATTR_LANE, id.c_str(), parsedOk, false);
    const std::string startPos = attrs.get<std::string>(SUMO_ATTR_STARTPOS, id.c_str(), parsedOk, false);
    const std::string endPos = attrs.get<std::string>(SUMO_ATTR_ENDPOS, id.c_str(), parsedOk, false);
    const std::string name = attrs.get<std::string>(SUMO_ATTR_NAME, id.c_str(), parsedOk, false);
    const std::vector<std::string> lines = attrs.get<std::vector<std::string> >(SUMO_ATTR_LINES, id.c_str(), parsedOk, false);
    const int personCapacity = attrs.get<int>(SUMO_ATTR_PERSON_CAPACITY, id.c_str(), parsedOk, false);
    const double parkingLength = attrs.get<double>(SUMO_ATTR_PARKING_LENGTH, id.c_str(), parsedOk, false);
    const bool friendlyPos = attrs.get<bool>(SUMO_ATTR_FRIENDLY_POS, id.c_str(), parsedOk, false);
    // continue if flag is ok
    if (parsedOk && myCommonXMLStructure.getLastInsertedSumoBaseObject()) {
        // first open tag
        myCommonXMLStructure.openTag(SUMO_TAG_BUS_STOP);
        // add all attributes
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_ID, id);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_LANE, laneId);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_STARTPOS, startPos);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_ENDPOS, endPos);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_NAME, name);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringListAttribute(SUMO_ATTR_LINES, lines);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addIntAttribute(SUMO_ATTR_PERSON_CAPACITY, personCapacity);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addDoubleAttribute(SUMO_ATTR_PARKING_LENGTH, parkingLength);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addBoolAttribute(SUMO_ATTR_FRIENDLY_POS, friendlyPos);
    }
}


void
AdditionalHandler::parseAccessAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseContainerStopAttributes(const SUMOSAXAttributes& attrs) {
    // declare Ok Flag
    bool parsedOk = true;
    // now obtain attributes
    const std::string id = attrs.get<std::string>(SUMO_ATTR_ID, "", parsedOk, false);
    const std::string laneId = attrs.get<std::string>(SUMO_ATTR_LANE, id.c_str(), parsedOk, false);
    const std::string startPos = attrs.get<std::string>(SUMO_ATTR_STARTPOS, id.c_str(), parsedOk, false);
    const std::string endPos = attrs.get<std::string>(SUMO_ATTR_ENDPOS, id.c_str(), parsedOk, false);
    const std::string name = attrs.get<std::string>(SUMO_ATTR_NAME, id.c_str(), parsedOk, false);
    const std::vector<std::string> lines = attrs.get<std::vector<std::string> >(SUMO_ATTR_LINES, id.c_str(), parsedOk, false);
    const bool friendlyPos = attrs.get<bool>(SUMO_ATTR_FRIENDLY_POS, id.c_str(), parsedOk, false);
    // continue if flag is ok
    if (parsedOk && myCommonXMLStructure.getLastInsertedSumoBaseObject()) {
        // first open tag
        myCommonXMLStructure.openTag(SUMO_TAG_CONTAINER_STOP);
        // add all attributes
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_ID, id);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_LANE, laneId);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_STARTPOS, startPos);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_ENDPOS, endPos);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_NAME, name);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringListAttribute(SUMO_ATTR_LINES, lines);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addBoolAttribute(SUMO_ATTR_FRIENDLY_POS, friendlyPos);
    }
}


void
AdditionalHandler::parseChargingStationAttributes(const SUMOSAXAttributes& attrs) {
    // declare Ok Flag
    bool parsedOk = true;
    // now obtain attributes
    const std::string id = attrs.get<std::string>(SUMO_ATTR_ID, "", parsedOk, false);
    const std::string laneId = attrs.get<std::string>(SUMO_ATTR_LANE, id.c_str(), parsedOk, false);
    const std::string startPos = attrs.get<std::string>(SUMO_ATTR_STARTPOS, id.c_str(), parsedOk, false);
    const std::string endPos = attrs.get<std::string>(SUMO_ATTR_ENDPOS, id.c_str(), parsedOk, false);
    const std::string name = attrs.get<std::string>(SUMO_ATTR_NAME, id.c_str(), parsedOk, false);
    const std::vector<std::string> lines = attrs.get<std::vector<std::string> >(SUMO_ATTR_LINES, id.c_str(), parsedOk, false);
    const double chargingPower = attrs.get<double>(SUMO_ATTR_CHARGINGPOWER, id.c_str(), parsedOk, false);
    const double efficiency = attrs.get<double>(SUMO_ATTR_EFFICIENCY, id.c_str(), parsedOk, false);
    const bool chargeInTransit = attrs.get<bool>(SUMO_ATTR_CHARGEINTRANSIT, id.c_str(), parsedOk, false);
    const SUMOTime chargeDelay = attrs.get<SUMOTime>(SUMO_ATTR_CHARGEDELAY, id.c_str(), parsedOk, false);
    const bool friendlyPos = attrs.get<bool>(SUMO_ATTR_FRIENDLY_POS, id.c_str(), parsedOk, false);
    // continue if flag is ok
    if (parsedOk && myCommonXMLStructure.getLastInsertedSumoBaseObject()) {
        // first open tag
        myCommonXMLStructure.openTag(SUMO_TAG_BUS_STOP);
        // add all attributes
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_ID, id);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_LANE, laneId);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_STARTPOS, startPos);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_ENDPOS, endPos);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_NAME, name);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringListAttribute(SUMO_ATTR_LINES, lines);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addDoubleAttribute(SUMO_ATTR_CHARGINGPOWER, chargingPower);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addDoubleAttribute(SUMO_ATTR_EFFICIENCY, efficiency);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addBoolAttribute(SUMO_ATTR_CHARGEINTRANSIT, chargeInTransit);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addSUMOTimeAttribute(SUMO_ATTR_CHARGEDELAY, chargeDelay);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addBoolAttribute(SUMO_ATTR_FRIENDLY_POS, friendlyPos);
    }
}


void
AdditionalHandler::parseParkingAreaAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseParkingSpaceAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseE1Attributes(const SUMOSAXAttributes& attrs) {
    // declare Ok Flag
    bool parsedOk = true;
    // now obtain attributes
    const std::string id = attrs.get<std::string>(SUMO_ATTR_ID, "", parsedOk, false);
    const std::string laneId = attrs.get<std::string>(SUMO_ATTR_LANE, id.c_str(), parsedOk, false);
    const double position = attrs.get<double>(SUMO_ATTR_POSITION, id.c_str(), parsedOk, false);
    const SUMOTime frequency = attrs.get<SUMOTime>(SUMO_ATTR_FREQUENCY, id.c_str(), parsedOk, false);
    const std::string file = attrs.get<std::string>(SUMO_ATTR_FILE, id.c_str(), parsedOk, false);
    const std::string vehicleTypes = attrs.get<std::string>(SUMO_ATTR_VTYPES, id.c_str(), parsedOk, false);
    const std::string name = attrs.get<std::string>(SUMO_ATTR_NAME, id.c_str(), parsedOk, false);
    const bool friendlyPos = attrs.get<bool>(SUMO_ATTR_FRIENDLY_POS, id.c_str(), parsedOk, false);
    // continue if flag is ok
    if (parsedOk && myCommonXMLStructure.getLastInsertedSumoBaseObject()) {
        // first open tag
        myCommonXMLStructure.openTag(SUMO_TAG_E1DETECTOR);
        // add all attributes
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_ID, id);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_LANE, laneId);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addDoubleAttribute(SUMO_ATTR_POSITION, position);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addSUMOTimeAttribute(SUMO_ATTR_FREQUENCY, frequency);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_FILE, file);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_VTYPES, vehicleTypes);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addStringAttribute(SUMO_ATTR_NAME, name);
        myCommonXMLStructure.getLastInsertedSumoBaseObject()->addBoolAttribute(SUMO_ATTR_FRIENDLY_POS, friendlyPos);
    }
}


void
AdditionalHandler::parseE2Attributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseE3Attributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseEntryAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseExitAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseE1InstantAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseTAZAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseTAZSourceAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseTAZSinkAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseVariableSpeedSignAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseVariableSpeedSignStepAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseCalibratorAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseCalibratorFlowAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseRerouterAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseRerouterIntervalAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseClosingLaneRerouteAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseClosingRerouteAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseDestProbRerouteAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseParkingAreaRerouteAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseRouteProbRerouteAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseRouteProbeAttributes(const SUMOSAXAttributes& attrs) {

}


void
AdditionalHandler::parseVaporizerAttributes(const SUMOSAXAttributes& attrs) {

}


void 
AdditionalHandler::parseParameters(const SUMOSAXAttributes& attrs) {
    // declare Ok Flag
    bool parsedOk = true;
    // get key
    const std::string key = attrs.get<std::string>(SUMO_ATTR_KEY, nullptr, parsedOk);
    // continue if key was sucesfully loaded
    if (parsedOk && myCommonXMLStructure.getLastInsertedSumoBaseObject()) {
        // get tag str
        const std::string tagStr = toString(myCommonXMLStructure.getLastInsertedSumoBaseObject()->getTag());
        // circumventing empty string value
        const std::string value = attrs.hasAttribute(SUMO_ATTR_VALUE) ? attrs.getString(SUMO_ATTR_VALUE) : "";
        // show warnings if values are invalid
        if (key.empty()) {
            WRITE_WARNING("Error parsing key from " + tagStr + " generic parameter. Key cannot be empty");
        } else if (!SUMOXMLDefinitions::isValidParameterKey(key)) {
            WRITE_WARNING("Error parsing key from " + tagStr + " generic parameter. Key contains invalid characters");
        } else {
            WRITE_DEBUG("Inserting generic parameter '" + key + "|" + value + "' into " + tagStr);
            // insert parameter
            myCommonXMLStructure.getLastInsertedSumoBaseObject()->addParameter(key, value);
        }
    }
}


void 
AdditionalHandler::parseSumoBaseObject(CommonXMLStructure::SumoBaseObject* obj) {
    // switch tag
    switch (obj->getTag()) {
        // Stopping Places
        case SUMO_TAG_BUS_STOP:
        case SUMO_TAG_TRAIN_STOP:
            buildBusStop(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getStringAttribute(SUMO_ATTR_LANE),
                obj->getStringAttribute(SUMO_ATTR_STARTPOS),
                obj->getStringAttribute(SUMO_ATTR_ENDPOS),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getStringListAttribute(SUMO_ATTR_LINES),
                obj->getIntAttribute(SUMO_ATTR_PERSON_CAPACITY),
                obj->getDoubleAttribute(SUMO_ATTR_PARKING_LENGTH),
                obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS),
                obj->getParameters());
            break;
        case SUMO_TAG_ACCESS:
            buildAccess(obj,
                obj->getStringAttribute(SUMO_ATTR_LANE),
                obj->getDoubleAttribute(SUMO_ATTR_POSITION),
                obj->getStringAttribute(SUMO_ATTR_LENGTH),
                obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS),
                obj->getParameters());
            break;
        case SUMO_TAG_CONTAINER_STOP:
            buildContainerStop(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getStringAttribute(SUMO_ATTR_LANE),
                obj->getStringAttribute(SUMO_ATTR_STARTPOS),
                obj->getStringAttribute(SUMO_ATTR_ENDPOS),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getStringListAttribute(SUMO_ATTR_LINES),
                obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS),
                obj->getParameters());
            break;
        case SUMO_TAG_CHARGING_STATION:
            buildChargingStation(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getStringAttribute(SUMO_ATTR_LANE),
                obj->getStringAttribute(SUMO_ATTR_STARTPOS),
                obj->getStringAttribute(SUMO_ATTR_ENDPOS),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getDoubleAttribute(SUMO_ATTR_CHARGINGPOWER),
                obj->getDoubleAttribute(SUMO_ATTR_EFFICIENCY),
                obj->getBoolAttribute(SUMO_ATTR_CHARGEINTRANSIT),
                obj->getSUMOTimeAttribute(SUMO_ATTR_CHARGEDELAY),
                obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS),
                obj->getParameters());
            break;
        case SUMO_TAG_PARKING_AREA:
            buildParkingArea(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getStringAttribute(SUMO_ATTR_LANE),
                obj->getStringAttribute(SUMO_ATTR_STARTPOS),
                obj->getStringAttribute(SUMO_ATTR_ENDPOS),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS),
                obj->getIntAttribute(SUMO_ATTR_ROADSIDE_CAPACITY),
                obj->getDoubleAttribute(SUMO_ATTR_ONROAD),
                obj->getDoubleAttribute(SUMO_ATTR_WIDTH),
                obj->getStringAttribute(SUMO_ATTR_LENGTH),
                obj->getDoubleAttribute(SUMO_ATTR_ANGLE),
                obj->getParameters());
            break;
        case SUMO_TAG_PARKING_SPACE:
            buildParkingSpace(obj,
                obj->getPositionAttribute(SUMO_ATTR_POSITION),
                obj->getDoubleAttribute(SUMO_ATTR_WIDTH),
                obj->getDoubleAttribute(SUMO_ATTR_LENGTH),
                obj->getDoubleAttribute(SUMO_ATTR_ANGLE),
                obj->getDoubleAttribute(SUMO_ATTR_SLOPE),
                obj->getParameters());
            break;
        // Detectors
        case SUMO_TAG_E1DETECTOR:
        case SUMO_TAG_INDUCTION_LOOP:
            // build E1
            buildE1Detector(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getStringAttribute(SUMO_ATTR_LANE),
                obj->getDoubleAttribute(SUMO_ATTR_POSITION),
                obj->getSUMOTimeAttribute(SUMO_ATTR_FREQUENCY),
                obj->getStringAttribute(SUMO_ATTR_FILE),
                obj->getStringAttribute(SUMO_ATTR_VTYPES),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS),
                obj->getParameters());
            break;
        case SUMO_TAG_E2DETECTOR:
        case SUMO_TAG_LANE_AREA_DETECTOR:
            buildSingleLaneDetectorE2(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getStringAttribute(SUMO_ATTR_LANE),
                obj->getDoubleAttribute(SUMO_ATTR_POSITION),
                obj->getDoubleAttribute(SUMO_ATTR_LENGTH),
                obj->getStringAttribute(SUMO_ATTR_FREQUENCY),
                obj->getStringAttribute(SUMO_ATTR_TLID),
                obj->getStringAttribute(SUMO_ATTR_FILE),
                obj->getStringAttribute(SUMO_ATTR_VTYPES),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getSUMOTimeAttribute(SUMO_ATTR_HALTING_TIME_THRESHOLD),
                obj->getDoubleAttribute(SUMO_ATTR_HALTING_SPEED_THRESHOLD),
                obj->getDoubleAttribute(SUMO_ATTR_JAM_DIST_THRESHOLD),
                obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS),
                obj->getParameters());
            break;
        case SUMO_TAG_E2DETECTOR_MULTILANE:
            buildMultiLaneDetectorE2(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getStringListAttribute(SUMO_ATTR_LANES),
                obj->getDoubleAttribute(SUMO_ATTR_POSITION),
                obj->getDoubleAttribute(SUMO_ATTR_LENGTH),
                obj->getStringAttribute(SUMO_ATTR_FREQUENCY),
                obj->getStringAttribute(SUMO_ATTR_TLID),
                obj->getStringAttribute(SUMO_ATTR_FILE),
                obj->getStringAttribute(SUMO_ATTR_VTYPES),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getSUMOTimeAttribute(SUMO_ATTR_HALTING_TIME_THRESHOLD),
                obj->getDoubleAttribute(SUMO_ATTR_HALTING_SPEED_THRESHOLD),
                obj->getDoubleAttribute(SUMO_ATTR_JAM_DIST_THRESHOLD),
                obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS),
                obj->getParameters());
            break;
        case SUMO_TAG_E3DETECTOR:
        case SUMO_TAG_ENTRY_EXIT_DETECTOR:
            buildDetectorE3(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getPositionAttribute(SUMO_ATTR_POSITION),
                obj->getSUMOTimeAttribute(SUMO_ATTR_FREQUENCY),
                obj->getStringAttribute(SUMO_ATTR_FREQUENCY),
                obj->getStringAttribute(SUMO_ATTR_VTYPES),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getSUMOTimeAttribute(SUMO_ATTR_HALTING_TIME_THRESHOLD),
                obj->getDoubleAttribute(SUMO_ATTR_HALTING_SPEED_THRESHOLD),
                obj->getParameters());
            break;
        case SUMO_TAG_DET_ENTRY:
            buildDetectorEntry(obj,
            obj->getStringAttribute(SUMO_ATTR_LANE),
            obj->getDoubleAttribute(SUMO_ATTR_POSITION),
            obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS));
            break;
        case SUMO_TAG_DET_EXIT:
            buildDetectorExit(obj,
            obj->getStringAttribute(SUMO_ATTR_LANE),
            obj->getDoubleAttribute(SUMO_ATTR_POSITION),
            obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS));
            break;
        case SUMO_TAG_INSTANT_INDUCTION_LOOP:
            buildDetectorE1Instant(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getStringAttribute(SUMO_ATTR_LANE),
                obj->getDoubleAttribute(SUMO_ATTR_POSITION),
                obj->getStringAttribute(SUMO_ATTR_FILE),
                obj->getStringAttribute(SUMO_ATTR_VTYPES),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getBoolAttribute(SUMO_ATTR_FRIENDLY_POS),
                obj->getParameters());
            break;
        // TAZs
        case SUMO_TAG_TAZ:
            buildTAZ(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getPositionVectorAttribute(SUMO_ATTR_SHAPE),
                RGBColor::parseColor(obj->getStringAttribute(SUMO_ATTR_COLOR)),
                obj->getStringListAttribute(SUMO_ATTR_EDGES),
                obj->getParameters());
            break;
        case SUMO_TAG_TAZSOURCE:
            buildTAZSource(obj,
                obj->getStringAttribute(SUMO_ATTR_EDGE),
                obj->getDoubleAttribute(SUMO_ATTR_WEIGHT),
                obj->getParameters());
            break;
        case SUMO_TAG_TAZSINK:
            buildTAZSink(obj,
                obj->getStringAttribute(SUMO_ATTR_EDGE),
                obj->getDoubleAttribute(SUMO_ATTR_WEIGHT),
                obj->getParameters());
            break;
        // Variable Speed Sign
        case SUMO_TAG_VSS:
            buildVariableSpeedSign(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getPositionAttribute(SUMO_ATTR_POSITION),
                obj->getStringListAttribute(SUMO_ATTR_LANES),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getParameters());
            break;
        case SUMO_TAG_STEP:
            buildVariableSpeedSignStep(obj,
                obj->getSUMOTimeAttribute(SUMO_ATTR_TIME),
                obj->getDoubleAttribute(SUMO_ATTR_SPEED));
            break;
        // Calibrator
        case SUMO_TAG_CALIBRATOR:
            buildEdgeCalibrator(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getStringAttribute(SUMO_ATTR_EDGE),
                obj->getDoubleAttribute(SUMO_ATTR_POSITION),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getStringAttribute(SUMO_ATTR_OUTPUT),
                obj->getSUMOTimeAttribute(SUMO_ATTR_FREQUENCY),
                obj->getStringAttribute(SUMO_ATTR_ROUTEPROBE),
                obj->getParameters());
            break;
        case SUMO_TAG_LANECALIBRATOR:
            buildLaneCalibrator(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getStringAttribute(SUMO_ATTR_LANE),
                obj->getDoubleAttribute(SUMO_ATTR_POSITION),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getStringAttribute(SUMO_ATTR_OUTPUT),
                obj->getSUMOTimeAttribute(SUMO_ATTR_FREQUENCY),
                obj->getStringAttribute(SUMO_ATTR_ROUTEPROBE),
                obj->getParameters());
            break;
        case SUMO_TAG_FLOW_CALIBRATOR:
            buildCalibratorFlow(obj,
                obj->getStringAttribute(SUMO_ATTR_TYPE),
                obj->getStringAttribute(SUMO_ATTR_ROUTE),
                obj->getStringAttribute(SUMO_ATTR_VEHSPERHOUR),
                obj->getStringAttribute(SUMO_ATTR_SPEED),
                RGBColor::parseColor(obj->getStringAttribute(SUMO_ATTR_COLOR)),
                obj->getStringAttribute(SUMO_ATTR_DEPARTLANE),
                obj->getStringAttribute(SUMO_ATTR_DEPARTPOS),
                obj->getStringAttribute(SUMO_ATTR_DEPARTSPEED),
                obj->getStringAttribute(SUMO_ATTR_ARRIVALLANE),
                obj->getStringAttribute(SUMO_ATTR_ARRIVALPOS),
                obj->getStringAttribute(SUMO_ATTR_ARRIVALSPEED),
                obj->getStringAttribute(SUMO_ATTR_LINE),
                obj->getIntAttribute(SUMO_ATTR_NUMBER),
                obj->getIntAttribute(SUMO_ATTR_CONTAINER_NUMBER),
                obj->getBoolAttribute(SUMO_ATTR_REROUTE),
                obj->getStringAttribute(SUMO_ATTR_DEPARTPOS_LAT),
                obj->getStringAttribute(SUMO_ATTR_ARRIVALPOS_LAT),
                obj->getSUMOTimeAttribute(SUMO_ATTR_BEGIN),
                obj->getSUMOTimeAttribute(SUMO_ATTR_END),
                obj->getParameters());
            break;
        // Rerouter
        case SUMO_TAG_REROUTER:
            buildRerouter(obj,
                obj->getStringAttribute(SUMO_ATTR_ID),
                obj->getPositionAttribute(SUMO_ATTR_POSITION),
                obj->getStringListAttribute(SUMO_ATTR_EDGES),
                obj->getDoubleAttribute(SUMO_ATTR_PROB),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getStringAttribute(SUMO_ATTR_FILE),
                obj->getBoolAttribute(SUMO_ATTR_OFF),
                obj->getSUMOTimeAttribute(SUMO_ATTR_HALTING_TIME_THRESHOLD),
                obj->getStringAttribute(SUMO_ATTR_VTYPES),
                obj->getParameters());
            break;
        case SUMO_TAG_INTERVAL:
            buildRerouterInterval(obj,
                obj->getSUMOTimeAttribute(SUMO_ATTR_BEGIN),
                obj->getSUMOTimeAttribute(SUMO_ATTR_END));
            break;
        case SUMO_TAG_CLOSING_LANE_REROUTE:
            buildClosingLaneReroute(obj,
                obj->getStringAttribute(SUMO_ATTR_LANE),
                parseVehicleClasses(obj->getStringAttribute(SUMO_ATTR_ALLOW), obj->getStringAttribute(SUMO_ATTR_DISALLOW)));
            break;
        case SUMO_TAG_CLOSING_REROUTE:
            buildClosingReroute(obj,
                obj->getStringAttribute(SUMO_ATTR_EDGE),
                parseVehicleClasses(obj->getStringAttribute(SUMO_ATTR_ALLOW), obj->getStringAttribute(SUMO_ATTR_DISALLOW)));
            break;
        case SUMO_TAG_DEST_PROB_REROUTE:
            builDestProbReroute(obj,
                obj->getStringAttribute(SUMO_ATTR_EDGE),
                obj->getDoubleAttribute(SUMO_ATTR_PROB));
            break;
        case SUMO_TAG_PARKING_ZONE_REROUTE:
            builParkingAreaReroute(obj,
                obj->getStringAttribute(SUMO_ATTR_PARKING),
                obj->getDoubleAttribute(SUMO_ATTR_PROB),
                obj->getBoolAttribute(SUMO_ATTR_VISIBLE));
            break;
        case SUMO_TAG_ROUTE_PROB_REROUTE:
            buildRouteProbReroute(obj,
                obj->getStringAttribute(SUMO_ATTR_ROUTE),
                obj->getDoubleAttribute(SUMO_ATTR_PROB));
            break;
        // Route probe
        case SUMO_TAG_ROUTEPROBE:
            buildRouteProbe(obj,
            obj->getStringAttribute(SUMO_ATTR_ID),
            obj->getStringAttribute(SUMO_ATTR_EDGE),
            obj->getStringAttribute(SUMO_ATTR_FREQUENCY),
            obj->getStringAttribute(SUMO_ATTR_NAME),
            obj->getStringAttribute(SUMO_ATTR_FILE),
            obj->getSUMOTimeAttribute(SUMO_ATTR_BEGIN),
            obj->getParameters());
            break;
        // Vaporizer (deprecated)
        case SUMO_TAG_VAPORIZER:
            buildVaporizer(obj,
                obj->getStringAttribute(SUMO_ATTR_EDGE),
                obj->getSUMOTimeAttribute(SUMO_ATTR_STARTTIME),
                obj->getSUMOTimeAttribute(SUMO_ATTR_END),
                obj->getStringAttribute(SUMO_ATTR_NAME),
                obj->getParameters());
            break;
        default:
            break;
    }
    // now iterate over childrens
    for (const auto &child : obj->getSumoBaseObjectChildren()) {
        // call this function recursively
        parseSumoBaseObject(child);
    }
}

/****************************************************************************/
