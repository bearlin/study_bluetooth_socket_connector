//
// This file contain LtpDemo using gtest/gmock framework.
//

#include "LtpUserDataConnector.h"

#include "LtpUserDataService.h"
#include "LtpDummyLocationProvider.h"

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */

using namespace NLtp;

int gExpect_Latitude = (51514271);
int gExpect_Longitude = (-139346);

// Implement this listener for SP to get the location.
//--------------------------------------------------------------------------------------
class CLtpUserDataServiceListener : public ILtpUserDataServiceListener
{ 
public: 
   void destinationReceived(CLtpTimestampedLocation timeStampedDestination);
};

// Implement this listener to get the location.
void CLtpUserDataServiceListener::destinationReceived(CLtpTimestampedLocation timeStampedDestination)
{
  std::cout << "CLtpUserDataServiceListener::destinationReceived!!!" << std::endl; 
  
  std::cout << "LtpDemo: Timestamp : " + timeStampedDestination.getTimestamp() << std::endl;
  const CLtpWGS84Coordinates& coordinates = timeStampedDestination.getCoordinates();
  std::cout << "LtpDemo: Coordinates : " << coordinates.getLatitude() << ", " << coordinates.getLongitude() << std::endl << std::endl;
}
//--------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
  std::string instance = "HU: "; //default as HU instance.
  TSocketType SocketType = ESocketType_IP;
  
  if(argc != 3)   
  {
    printf("usage: %s <0:HU|1:SP> <0:Use IP Socket|1:Use BT Socket>\n", argv[0]);    
    return -1;  
  }

  if ('0' == *argv[1])
    instance = "HU: ";
  else if ('1' == *argv[1])
    instance = "SP: ";

  if ('0' == *argv[2])
    SocketType = ESocketType_IP;
  else if ('1' == *argv[2])
    SocketType = ESocketType_BT;

  std::cout << "LtpDemo main() start!!!" << std::endl;
  std::cout << "LocalTimeStamp():" << CLtpTimestampedLocation::GetLocalTimeStamp() << std::endl;
  
  if (instance == std::string("SP: ")) 
  {
    printf("Press ENTER to send a request, press 'q' to disconnect and exit program.");
    
    CLtpUserDataService* UserDataService = new CLtpUserDataService(instance, SocketType);
    
    // SP will entering a loop to prevent close the socket connection.
    while ('q' != getchar()) 
    {
      // SP Add CLtpUserDataServiceListener as a destination listener to UserDataService.
      CLtpUserDataServiceListener* destinationListener = new CLtpUserDataServiceListener();
      UserDataService->addDestinationListener(destinationListener);

      // SP requesting.
      std::vector<TLocationType> locationTypes;
      locationTypes.push_back(ELocationType_DESTINATION);
      locationTypes.push_back(ELocationType_LAST_KNOWN_CAR_POSITION);
      UserDataService->GetLtpLocations(locationTypes);
    }  

    delete UserDataService;
  }
  else if (instance == std::string("HU: ")) 
  {
    bool isConnected = false;

    CLtpDummyLocationProvider dummyLocationProvider(instance, gExpect_Latitude, gExpect_Longitude);
    CLtpUserDataService* UserDataService = NULL;
    
    while (isConnected == false)
    {
      // Create a UserDataService connector.
      UserDataService = new CLtpUserDataService(instance, SocketType);
      // HU Add CLtpDummyLocationProvider as a location provider to UserDataService.
      UserDataService->addLocationProvider((CLtpDummyLocationProvider*)&dummyLocationProvider);
      // HU listening.
      isConnected = UserDataService->enterListeningLoop();

      delete UserDataService;

      sleep(1);
    }
  }
  else 
  {
    //throw CLtpIllegalArgumentException("Unknown instance: " + instance);
    std::cout << "Unknown instance: " + instance << std::endl;
  }

  std::cout << "LtpDemo main() Done!!!" << std::endl;
  return 0;
}

