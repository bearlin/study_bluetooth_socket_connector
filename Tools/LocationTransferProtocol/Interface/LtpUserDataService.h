//
// This file contain UserDataService interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPUSERDATASERVICE_H_
#define _LTPUSERDATASERVICE_H_

#include "LtpCommon.h"
#include "LtpExceptions.h"

#include "LtpUserDataConnector.h"
#include "LtpDummyCloudConnector.h"

#include "LtpLocationProvider.h"
#include "LtpDummyLocationProvider.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

namespace NLtp //Namespace is LocationTransferProtocol
{
  class ILtpUserDataServiceListener 
  {
  public: 
    //! Method for SP to implement actions after received destination results from HU.
    //! Caller:        UserDataService layer..
    //! Implement: One of SP's class who new a UserDataService instance and use UserDataService to send a request.
    virtual void destinationReceived(CLtpTimestampedLocation timeStampedDestination) = 0;
  };

  /**
  * This service provides an interface which can be used by UI application to
  * send the 'active route' to Cloud Server and to receive an 'active route'
  * from Cloud Server.
  *
  */
  class ILtpUserDataService
  {
  public:

    virtual void shareDestination(CLtpWGS84Coordinates& destination) = 0;

    //! Method for SP to add itself as a listener of the destination results from HU.
    //! Caller:        After SP new a UserDataService instance.
    //! Implement: UserDataService layer.
    virtual void addDestinationListener(ILtpUserDataServiceListener* listener) = 0;

    // Method for SP to send "get Ltp locations" request.
    //! Caller:        After SP new a UserDataService instance.
    //! Implement: UserDataService layer.
    virtual void GetLtpLocations(std::vector<TLocationType>& locationTypes) = 0;

    //! Method for HU to entering the Request listening loop.
    //! Caller:        After HU new a UserDataService instance.
    //! Implement: UserDataService layer.
    virtual bool enterListeningLoop() = 0;

    //! Method for UIApplication or LocationProvider to add itself as a location provider.
    //! Caller:        One of UIApplication's or LocationProvider's class on HU who will provide Destination information to UserDataService layer.
    //! Implement: UserDataService layer.
    virtual void addLocationProvider(ILtpLocationProvider* aLocationProvider) = 0;
  };

  class CLtpUserDataService : public ILtpUserDataService, public ILtpUserDataConnectorListener 
  {
  private:
    std::string instance;
    ILtpLocationProvider* iLocationProvider;
    ILtpDummyCloudConnector* cloudConnector;
    ILtpUserDataConnector* userDataConnector;
    ILtpUserDataServiceListener* destinationListener;

    int iSessionRequestId;

  public: 
    CLtpUserDataService(std::string instance, TSocketType aSocketType);
    virtual ~CLtpUserDataService();

    //! See base class
    void shareDestination(CLtpWGS84Coordinates& destination);

    //! See base class
    void addDestinationListener(ILtpUserDataServiceListener* listener);

    //! See base class
    void GetLtpLocations(std::vector<TLocationType>& locationTypes);
      
    //! See base class
    bool enterListeningLoop();

    //! See base class
    void addLocationProvider(ILtpLocationProvider* aLocationProvider);
    
    //! See base class
    void connectionStatus(TConnectionStatus connectionStatus);

    //! See base class
    void getLocationsRequest(int requestId, const std::vector<TLocationType>& locationTypes);

    //! See base class
    void getLocationsReply(int requestId, int respCode, const std::vector<CLtpTypedLocation>& typedLocations);

  };
  
} //namespace NLtp


#endif  // _LTPUSERDATASERVICE_H_
