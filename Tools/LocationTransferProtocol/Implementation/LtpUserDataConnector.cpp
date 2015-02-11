//
// This file contain implementation of NLtp::ILtpUserDataConnector for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpUserDataConnector.h"
#include "ILogging.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <stdio.h>

using namespace NLtp;

extern logging::TLogComponent iLtpLogId;

CLtpUserDataConnector::CLtpUserDataConnector(std::string& instance, TSocketType aSocketType) : 
  rpcOverSpp(NULL), 
  tconnectionStatus(EConnectionStatusNOT_CONNECTED), 
  connectionListener(NULL),
  incomingRequestHandler(NULL)
{
  LogInterface( iLtpLogId, "");
  
  // The instance will be "HU: " or "SP: "
  this->instance = instance;

  // Create a lower layer CLtpConnection, and register myself(CLtpRpcOverSpp) as its Listener.
  rpcOverSpp = new CLtpRpcOverSpp(instance, aSocketType);
  rpcOverSpp->registerConnectionListener(this);
  rpcOverSpp->registerRequestListener(this);
  rpcOverSpp->registerResponseListener(this);

}

CLtpUserDataConnector::~CLtpUserDataConnector()
{
  if (rpcOverSpp)
  {
    rpcOverSpp->unregisterConnectionListener(this);
    delete rpcOverSpp;
    rpcOverSpp = NULL;
  }

  this->instance = "";

  tconnectionStatus = EConnectionStatusNOT_CONNECTED;
  connectionListener = NULL;
  incomingRequestHandler = NULL;
}

void CLtpUserDataConnector::addConnectionListener(ILtpUserDataConnectorListener* listener) 
{
  // TODO Add listener to list of listeners

  this->connectionListener = listener; // registered UserDataService layer as listener of this layer

  listener->connectionStatus(tconnectionStatus); //let UserDataService layer know the connection status of UserDataConnector layer.
}

void CLtpUserDataConnector::removeConnectionListener(ILtpUserDataConnectorListener* listener)
{
  this->connectionListener = NULL;
}

void CLtpUserDataConnector::sendLocationsRequest(int requestId, std::vector<TLocationType>& locationTypes, ILtpUserDataConnectorListener* listener)
{
  LogInterface( iLtpLogId, "");

  //openRequestListeners.put(requestId, listener);
  openRequestListeners.insert(std::pair<int, ILtpUserDataConnectorListener*>(requestId, listener));

  // Convert std::vector<TLocationType> to rapidjson::Value .
  //###############################################################################
  rapidjson::Document document;// Set this document as an empty object.
  document.SetObject();
  
  rapidjson::Value json;// Set this value as an empty array.
  json.SetArray();
  json.Clear();
  
  CLtpJsonTools::TypesToJsonValue(locationTypes, document, json);
  //###############################################################################
  
  rpcOverSpp->sendRequest(requestId, this, Ltp_methods_getUserData, document, json);
}

void CLtpUserDataConnector::registerRequestHandler(ILtpUserDataConnectorListener* listener)
{
  this->incomingRequestHandler = listener;
}

void CLtpUserDataConnector::sendLocationsResponse(int requestId, std::vector<CLtpTypedLocation>& typedLocations)
{
  LogInterface( iLtpLogId, " requestId="<<requestId);

  // Convert std::vector<CLtpTypedLocation> to rapidjson::Value .
  //###############################################################################
  rapidjson::Document document;// Set this document as an empty object.
  document.SetObject();
  
  rapidjson::Value json;// Set this value as an empty array.
  json.SetArray();
  json.Clear();
  
  CLtpJsonTools::TypesToJsonValue(typedLocations, document, json);
  //###############################################################################
  rpcOverSpp->sendResponse(requestId, Ltp_methods_getUserData, document, json);
}

void CLtpUserDataConnector::connectionStatus(TConnectionStatus connectionStatus)
{
  switch (tconnectionStatus) 
  {
    case EConnectionStatusCONNECTED:
      this->tconnectionStatus = EConnectionStatusCONNECTED;
    break;

    case EConnectionStatusNOT_CONNECTED:
      this->tconnectionStatus = EConnectionStatusNOT_CONNECTED;
    break;
  }

  // TODO notify listeners.
}

void CLtpUserDataConnector::requestReceived(int requestId, std::string methodName, rapidjson::Value& parameters)//(int requestId, std::string methodName, const std::vector<TLocationType> parameter)
{
  if (0 == methodName.compare(Ltp_methods_getUserData))
  {
    // Convert rapidjson::Value to std::vector<TLocationType>.
    //###############################################################################
    if (!parameters.IsArray())
    {
      throw CLtpRuntimeException(this->instance + "requestReceived: parameters no an json array!");
      return;
    }
    
    std::vector<TLocationType> parameterVec;
    TLocationType locationType; 

    // Loop through this json array.
    //for (rapidjson::Value::ConstValueIterator itr = parametersArray.Begin(); itr != parametersArray.End(); ++itr)
    for (rapidjson::SizeType i = 0; i < parameters.Size(); i++)	// rapidjson uses SizeType instead of size_t.
    {
    #if 1
      // For REQ format: ("parameters" : [["DESTINATION","LAST_KNOWN_CAR_POSITION"]])
      if (!parameters[i].IsArray())
      {
        throw CLtpRuntimeException(this->instance + "requestReceived: parameters[i] not json Array!");
        return;
      }

      // Loop through this json array.
      const rapidjson::Value& paramInnerArray = parameters[i]; // Using a reference for consecutive access is handy and faster.
      //for (rapidjson::Value::ConstValueIterator itr = parametersArray.Begin(); itr != parametersArray.End(); ++itr)
      for (rapidjson::SizeType j = 0; j < paramInnerArray.Size(); j++) // rapidjson uses SizeType instead of size_t.
      {
        if (!paramInnerArray[j].IsString())
        {
          throw CLtpRuntimeException(this->instance + "requestReceived: paramInnerArray[j] not json String!");
          return;
        }
        
        if (0 == std::string(Ltp_methods_getUserData_param_type_dest).compare(paramInnerArray[j].GetString()))
          locationType = ELocationType_DESTINATION;
        else if (0 == std::string(Ltp_methods_getUserData_param_type_lkcp).compare(paramInnerArray[j].GetString()))
          locationType = ELocationType_LAST_KNOWN_CAR_POSITION;
        else
        {
          printf("Unknow TLocationType!\n");
          throw CLtpRuntimeException("Unknow TLocationType!");
          //continue;
        }
        
        parameterVec.push_back(locationType);
      }
    #else
      // For REQ format: ("parameters" : [0,1,...])
      if (!parameters[i].IsNumber() || 
        !parameters[i].IsInt() ) 
      {
        throw CLtpRuntimeException(this->instance + "requestReceived: parameters not json Int!");
        return;
      }
      
      if (ELocationType_DESTINATION == parameters[i].GetInt())
        locationType = ELocationType_DESTINATION;
      else
        locationType = ELocationType_LAST_KNOWN_CAR_POSITION;
      
      parameterVec.push_back(locationType);
    #endif
    }
    //###############################################################################

    //if (parameterVec.size() != 1) 
    //{
    //	throw CLtpIllegalArgumentException("Wrong number of parameters");
    //}

    incomingRequestHandler->getLocationsRequest(requestId, parameterVec);
  }
  else
  {
    LogInfo( iLtpLogId, " UserDataConnector: requestReceived => Unknow methodName=" << methodName.c_str());
  }
}

void CLtpUserDataConnector::responseReceived(int requestId, std::string methodName, rapidjson::Value& parameters)//(int requestId, std::string methodName, const std::vector<CLtpTypedLocation> parameter)
{
  if (0 == methodName.compare(Ltp_methods_getUserData))
  {
    // Get the UserDataConnectorListener from map according to the requestId.
    std::map<int, ILtpUserDataConnectorListener*>::iterator it = openRequestListeners.find(requestId);
    if (it == openRequestListeners.end()) 
    {
      throw CLtpRuntimeException(this->instance + "UserDataConnector: no listener found for requestId");
    }
    ILtpUserDataConnectorListener* requestListener = it->second;
    openRequestListeners.erase(it); //remove this listener mapping.
    
    // Convert rapidjson::Value to std::vector<CLtpTypedLocation>.
    //###############################################################################
  #if 1
    /*
    For RESP format: 
      "parameters": [
              {
                  "DESTINATION": {
                      "timestamp": "2014-02-25T16:04:51.000+0800",
                      "coordinates": {
                          "latitude": 51514271,
                          "longitude": -139346
                      }
                  },
                  "LAST_KNOWN_CAR_POSITION": {
                      "timestamp": "2014-02-25T16:04:51.000+0800",
                      "coordinates": {
                          "latitude": 51514271,
                          "longitude": -139346
                      }
                  }
              }
          ]
    */
    if (!parameters.IsArray())
    {
      throw CLtpRuntimeException(this->instance + "responseReceived: parameters no an json array!");
      return;
    }
    
    std::vector<CLtpTypedLocation> parameterVec;
    CLtpTypedLocation typedLocation;
    TLocationType location_type;
    CLtpTimestampedLocation timestamped_location;
    std::string timestamp;
    CLtpWGS84Coordinates coordinates;
    int latitude;
    int longitude;

    //rapidjson::Value JObjFinal(rapidjson::kObjectType);
    //rapidjson::Value JObjType(rapidjson::kObjectType);
    //rapidjson::Value JObjCoor(rapidjson::kObjectType);

    // Loop through this json array.
    for (rapidjson::Value::ConstValueIterator itr = parameters.Begin(); itr != parameters.End(); ++itr)
    //for (rapidjson::SizeType i = 0; i < parameters.Size(); i++)	// rapidjson uses SizeType instead of size_t.
    {
      const rapidjson::Value& JObjFinal = (*itr);
      if (!JObjFinal.IsObject())
      {
        throw CLtpRuntimeException(this->instance + "responseReceived: parameters element not an json Object!");
        return;
      }

      // Loop through this json Object.
      for (rapidjson::Value::ConstMemberIterator itr = JObjFinal.MemberBegin(); itr != JObjFinal.MemberEnd(); ++itr)
      {
        //printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);

        if (0 == std::string(Ltp_methods_getUserData_param_type_dest).compare(itr->name.GetString()))
        {
          location_type = ELocationType_DESTINATION;
        }
        else if (0 == std::string(Ltp_methods_getUserData_param_type_lkcp).compare(itr->name.GetString()))
        {
          location_type = ELocationType_LAST_KNOWN_CAR_POSITION;
          
        }
        else
        {
          printf("Unknow Location Type string!\n");
          throw CLtpRuntimeException("Unknow Location Type string!");
          //continue;
        }

        const rapidjson::Value& JObjType = itr->value;
        //for (rapidjson::Value::ConstMemberIterator itr = JObjType.MemberBegin(); itr != JObjType.MemberEnd(); ++itr)
        //  printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        timestamp = JObjType[Ltp_methods_getUserData_param_timestamp].GetString();

        const rapidjson::Value& JObjCoor = JObjType[Ltp_methods_getUserData_param_coordinates];
        //for (rapidjson::Value::ConstMemberIterator itr = JObjCoor.MemberBegin(); itr != JObjCoor.MemberEnd(); ++itr)
        //  printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        latitude = JObjCoor[Ltp_methods_getUserData_param_latitude].GetInt();
        longitude = JObjCoor[Ltp_methods_getUserData_param_longitude].GetInt();
        
        coordinates.setLatitude(latitude);
        coordinates.setLongitude(longitude);
        timestamped_location.setTimestamp(timestamp);
        timestamped_location.setCoordinates(coordinates);
        typedLocation.setNavDataType(location_type);
        typedLocation.setNavData(timestamped_location);
        
        parameterVec.push_back(typedLocation);
      }
    }
  #else
    /*
    For RESP format: 
      "parameters": [
              {
                  "location_type": 1,
                  "timestamp": "1997-07-16T19:20:30.45+01:00",
                  "latitude": 51514271,
                  "longitude": -139346
              },
              {
                  "location_type": 0,
                  "timestamp": "1998-07-16T19:20:30.45+01:00",
                  "latitude": 51514271,
                  "longitude": -139346
              }
          ]
    */
    if (!parameters.IsArray())
    {
      throw CLtpRuntimeException(this->instance + "responseReceived: parameters no an json array!");
      return;
    }
    
    std::vector<CLtpTypedLocation> parameterVec;
    CLtpTypedLocation typedLocation;
    TLocationType location_type;
    CLtpTimestampedLocation timestamped_location;
    std::string timestamp;
    CLtpWGS84Coordinates coordinates;
    int latitude;
    int longitude;

    // Loop through this json array.
    for (rapidjson::Value::ConstValueIterator itr = parameters.Begin(); itr != parameters.End(); ++itr)
    //for (rapidjson::SizeType i = 0; i < parameters.Size(); i++)	// rapidjson uses SizeType instead of size_t.
    {
      if (ELocationType_DESTINATION == (*itr)[Ltp_methods_getUserData_param_location_type].GetInt())
        location_type = ELocationType_DESTINATION;
      else
        location_type = ELocationType_LAST_KNOWN_CAR_POSITION;
      timestamp = (*itr)[Ltp_methods_getUserData_param_timestamp].GetString();
      latitude = (*itr)[Ltp_methods_getUserData_param_latitude].GetInt();
      longitude = (*itr)[Ltp_methods_getUserData_param_longitude].GetInt();
      coordinates.setLatitude(latitude);
      coordinates.setLongitude(longitude);
      timestamped_location.setTimestamp(timestamp);
      timestamped_location.setCoordinates(coordinates);

      typedLocation.setNavDataType(location_type);
      typedLocation.setNavData(timestamped_location);
      
      parameterVec.push_back(typedLocation);
    }
  #endif
    //###############################################################################

    //if (parameter.size() != 1) 
    //{
    //  throw CLtpIllegalArgumentException("Wrong number of parameters");
    //}

    requestListener->getLocationsReply(requestId, 0, parameterVec);
  }
  else
  {
    LogInfo( iLtpLogId, " UserDataConnector: responseReceived => Unknow methodName=" << methodName.c_str());
  }
}

void CLtpUserDataConnector::responseErrorReceived(int requestId, std::string methodName, int respCode)
{
  if (0 == methodName.compare(Ltp_methods_getUserData))
  {
    // Get the UserDataConnectorListener from map according to the requestId.
    std::map<int, ILtpUserDataConnectorListener*>::iterator it = openRequestListeners.find(requestId);
    if (it == openRequestListeners.end()) 
    {
      throw CLtpRuntimeException(this->instance + "UserDataConnector: no listener found for requestId");
    }
    ILtpUserDataConnectorListener* requestListener = it->second;
    openRequestListeners.erase(it); //remove this listener mapping.

    // For responseErrorReceived, just uplink a empty parameter list, to let upper layer know that we didn't get any location reply.
    std::vector<CLtpTypedLocation> parameter;
    parameter.clear(); 
    requestListener->getLocationsReply(requestId, respCode, parameter);
  }
  else
  {
    LogInfo( iLtpLogId, " UserDataConnector: responseErrorReceived => Unknow methodName=" << methodName.c_str());
  }
}

bool CLtpUserDataConnector::startUserDataConnectorServerLoop()
{
  return rpcOverSpp->startRcpOverSppServerLoop();
}

