//
// This file contain implementation of NLtp::ILtpLocationProvider for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpDummyLocationProvider.h"

#include <iostream>
#include <string>
#include <stdio.h>

using namespace NLtp;

CLtpDummyLocationProvider::CLtpDummyLocationProvider(std::string aInstance, int aLatitude, int aLongitude) : 
  instance("")
  ,iLatitude(0)
  ,iLongitude(0)
{
  this->instance = aInstance;
  iLatitude = aLatitude;
  iLongitude = aLongitude;
  //printf("%sDummy LocationProvider created.\n", this->instance.c_str());
}

CLtpDummyLocationProvider::~CLtpDummyLocationProvider()
{
  //printf("%sDummy LocationProvider deleted.\n", this->instance.c_str());
}
    

CLtpWGS84Coordinates CLtpDummyLocationProvider::getDestination()
{
  CLtpWGS84Coordinates destination;// = new CLtpWGS84Coordinates();

  destination.setLatitude(iLatitude);
  destination.setLongitude(iLongitude);

  return destination;
}

CLtpWGS84Coordinates CLtpDummyLocationProvider::getCurrentCarPosition()
{
  CLtpWGS84Coordinates currentCarPosition;// = new CLtpWGS84Coordinates();

  currentCarPosition.setLatitude(iLatitude);
  currentCarPosition.setLongitude(iLongitude);

  return currentCarPosition;
}

