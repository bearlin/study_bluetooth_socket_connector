//
// This file contain ILtpLocationProvider interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPLOCATION_PRODUCER_H_
#define _LTPLOCATION_PRODUCER_H_

#include "LtpCommon.h"

#include <stdio.h>
#include <string>

namespace NLtp //Namespace is LocationTransferProtocol
{
  /**
  * After UserDataService recieved a getLocationsRequest(), UserDataService can
  * use this LocationProvider to get the location information.
  *
  * UIApplication or LocationProvider shall implement this interface to provide the
  * Destination and Current Car Position information for UserDataService layer.
  *
  * UIApplication or LocationProvider shall register itself as the LocationProvider.
  */
  class ILtpLocationProvider
  {
  public: 
    //! Method for HU to get Destination from UIApplication or LocationProvider.
    //! Caller:        UserDataService layer.
    //! Implement: One of UIApplication's or LocationProvider's class on HU who will provide Destination information to UserDataService layer.
    virtual CLtpWGS84Coordinates getDestination() = 0;

    //! Method for HU to get CurrentCarPosition from UIApplication or LocationProvider.
    //! Caller:        UserDataService layer.
    //! Implement: One of UIApplication's or LocationProvider's class on HU who will provide CurrentCarPosition information to UserDataService layer.
    virtual CLtpWGS84Coordinates getCurrentCarPosition() = 0;
  };
} //namespace NLtp


#endif  // _LTPLOCATION_PRODUCER_H_