//
// This file contain CLtpUserDataService UnitTest for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpUserDataConnector.h"

#include "LtpUserDataService.h"
#include "LtpDummyLocationProvider.h"

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;

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
  
  std::cout << "Timestamp : " + timeStampedDestination.getTimestamp() << std::endl;
  const CLtpWGS84Coordinates& coordinates = timeStampedDestination.getCoordinates();
  std::cout << "Coordinates : " << coordinates.getLatitude() << ", " << coordinates.getLongitude() << std::endl << std::endl;

  EXPECT_TRUE(coordinates.getLatitude() == gExpect_Latitude);
  EXPECT_TRUE(coordinates.getLongitude() == gExpect_Longitude);
}
//--------------------------------------------------------------------------------------

void* ThreadSP(void* unused)
{
  std::string instance = "SP: ";
  std::cout << instance + " thread start at LocalTimeStamp():" << CLtpTimestampedLocation::GetLocalTimeStamp() << std::endl;
  
  CLtpUserDataService* UserDataService = new CLtpUserDataService(instance, ESocketType_IP);

  // SP Add CLtpUserDataServiceListener as a destination listener to UserDataService.
  CLtpUserDataServiceListener* destinationListener = new CLtpUserDataServiceListener();
  UserDataService->addDestinationListener(destinationListener);

  // SP requesting.
  std::vector<TLocationType> locationTypes;
  locationTypes.push_back(ELocationType_DESTINATION);
  locationTypes.push_back(ELocationType_LAST_KNOWN_CAR_POSITION);
  UserDataService->GetLtpLocations(locationTypes);

  //std::cout << "sleep(2)" << std::endl;
  //sleep(2);
  
  if (UserDataService)
    delete UserDataService;

  pthread_exit(NULL);
}

void* ThreadHU(void* unused)
{
  std::string instance = "HU: ";
  std::cout << instance + " thread start at LocalTimeStamp():" << CLtpTimestampedLocation::GetLocalTimeStamp() << std::endl;
  
  CLtpUserDataService* UserDataService = new CLtpUserDataService(instance, ESocketType_IP);

  // HU Add CLtpDummyLocationProvider as a location provider to UserDataService.
  CLtpDummyLocationProvider dummyLocationProvider(instance, gExpect_Latitude, gExpect_Longitude);
  UserDataService->addLocationProvider((CLtpDummyLocationProvider*)&dummyLocationProvider);
  
  // HU listening.
  UserDataService->enterListeningLoop();

  delete UserDataService;

  pthread_exit(NULL);
}

class CLtpUserDataServiceTest: public ::testing::Test
{
};

// This Unit Test assume TCP/IP Socket as the underlying physical connection by specifying ESocketType_IP. 
TEST_F(CLtpUserDataServiceTest, GetFixCoordinatesFromDummyLocationProvider)
{
  std::cout << "CLtpUserDataService unit testing Start!!!" << std::endl;
  std::cout << "LocalTimeStamp():" << CLtpTimestampedLocation::GetLocalTimeStamp() << std::endl;

  pthread_t thread_id1, thread_id2;
  
  // Create new threads. The new threads will run ThreadHU(act as HU to send RESP) and ThreadSP(act as SP to send REQ) functions, 
  // the procedures will be ThreadHU(will create a server socket) -> sleep 2 seconds for socket server creating and listening -> ThreadSP(will create a client socket and send REQ to server socket).
  // we should check the RESP received correct or not.
  pthread_create(&thread_id1, NULL, &ThreadHU, NULL);
  std::cout << "sleep(1)" << std::endl;
  sleep(1);
  
  pthread_create(&thread_id2, NULL, &ThreadSP, NULL);
  
  /* Wait till threads are complete before main continues. Unless we  */
  /* wait we run the risk of executing an exit which will terminate   */
  /* the process and all threads before the threads have completed.   */
  pthread_join(thread_id2, NULL); // Main thread should wait for ThreadSP() complete.
  
  std::cout << "CLtpUserDataService unit testing Done!!!" << std::endl;
}


