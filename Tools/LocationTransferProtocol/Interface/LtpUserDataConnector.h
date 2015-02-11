//
// This file contain UserDataConnector interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPUSERDATACONNECTOR_H_
#define _LTPUSERDATACONNECTOR_H_

#include "LtpCommon.h"
#include "LtpExceptions.h"

#include "LtpRpcOverSpp.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace NLtp //Namespace is LocationTransferProtocol
{
  class ILtpUserDataConnectorListener 
  {
  public: 
    //! Caller:        UserDataConnector layer (to notify UserDataService layer listener about current connection status).
    //! Implement: UserDataService layer.
    virtual void connectionStatus(TConnectionStatus connectionStatus) = 0;

    //! Caller:        UserDataConnector layer (to notify UserDataService layer listener that a location request was received).
    //! Implement: UserDataService layer.
    virtual void getLocationsRequest(int requestId, const std::vector<TLocationType>& locationTypes) = 0;

    /**
    * If a requested location doesn't appear in the list, it wasn't available.
    * 
    * respCode values 1 .. 100 are reserved for the RpcOverSpp layer.
    */
    //! Caller:        UserDataConnector layer (to notify UserDataService layer listener that a location response was received).
    //! Implement: UserDataService layer.
    virtual void getLocationsReply(int requestId, int respCode, const std::vector<CLtpTypedLocation>& typedLocations) = 0;
  };

  class ILtpUserDataConnector 
  {
  private:

  public:
    virtual ~ILtpUserDataConnector() {};

    // Connection status
    // Add/remove listener. Status is received via connectionStatus() in UserDataConnectorListener.
    //! Caller:        UserDataService layer (to add him-self as a connection listener).
    //! Implement: UserDataConnector layer.
    virtual void addConnectionListener(ILtpUserDataConnectorListener* listener) = 0;

    //! Caller:        UserDataService layer (to remove him-self as a connection listener).
    //! Implement: UserDataConnector layer.
    virtual void removeConnectionListener(ILtpUserDataConnectorListener* listener) = 0;

    // Method to send requests (only needed on SP side ???)
    //! Caller:        UserDataService layer (to start a get location scenario).
    //! Implement: UserDataConnector layer.
    virtual void sendLocationsRequest(int requestId, std::vector<TLocationType>& locationTypes, ILtpUserDataConnectorListener* listener) = 0;

    /**
    * Register a handler for handling the incoming requests (only needed on HU side)
    */
    //! Caller:        UserDataService layer (to register him-self as a request handler).
    //! Implement: UserDataConnector layer.
    virtual void registerRequestHandler(ILtpUserDataConnectorListener* listener) = 0;

    // Method to initiate the sending of a response (only needed on HU side)

    //! Caller:        UserDataService layer (to let UserDataService sending location response after received a getNavDataRequest).
    //! Implement: UserDataConnector layer.
    virtual void sendLocationsResponse(int requestId, std::vector<CLtpTypedLocation>& typedLocations) = 0;

    //! Caller:        UserDataService layer (Entering Server loop).
    //! Implement: UserDataConnector layer.
    virtual bool startUserDataConnectorServerLoop() = 0;
  };

  class CLtpUserDataConnector : public ILtpUserDataConnector, public ILtpRpcOverSppListener 
  {
  private:
    CLtpRpcOverSpp* rpcOverSpp;
    TConnectionStatus tconnectionStatus;// = null;
    ILtpUserDataConnectorListener* connectionListener;// = null;  // TODO shall be a list of listeners
    std::map<int, ILtpUserDataConnectorListener*> openRequestListeners;// = new HashMap<>();
    ILtpUserDataConnectorListener* incomingRequestHandler;// = null;
    std::string instance;// = null; // Just for showing SP or HU in demo.

  public: 
    CLtpUserDataConnector(std::string& instance, TSocketType aSocketType);
    virtual ~CLtpUserDataConnector();

    //! See base class
    void addConnectionListener(ILtpUserDataConnectorListener* listener);

    //! See base class
    void removeConnectionListener(ILtpUserDataConnectorListener* listener);

    //! See base class
    void sendLocationsRequest(int requestId, std::vector<TLocationType>& locationTypes, ILtpUserDataConnectorListener* listener);
    
    //! See base class
    void registerRequestHandler(ILtpUserDataConnectorListener* listener);

    //! See base class
    void sendLocationsResponse(int requestId, std::vector<CLtpTypedLocation>& typedLocations);

    //! See base class
    void connectionStatus(TConnectionStatus connectionStatus);
    
    //! See base class
    //void requestReceived(int requestId, std::string methodName, const std::vector<TLocationType> parameter);
    void requestReceived(int requestId, std::string methodName, rapidjson::Value& parameters);

    //! See base class
    //void responseReceived(int requestId, std::string methodName, const std::vector<CLtpTypedLocation> parameter);
    void responseReceived(int requestId, std::string methodName, rapidjson::Value& parameters);

    //! See base class
    void responseErrorReceived(int requestId, std::string methodName, int respCode);

    //! See base class
    bool startUserDataConnectorServerLoop();
  };
  
} //namespace NLtp


#endif  // _LTPUSERDATACONNECTOR_H_
