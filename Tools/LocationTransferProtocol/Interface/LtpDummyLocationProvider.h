//
// This file contain Dummy LocationProvider interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPDUMMYLOCATIONPROVIDER_H_
#define _LTPDUMMYLOCATIONPROVIDER_H_

#include "LtpCommon.h"
#include "LtpExceptions.h"
#include "LtpLocationProvider.h"

#include <stdio.h>
#include <string>

namespace NLtp //Namespace is LocationTransferProtocol
{
  class CLtpDummyLocationProvider : public ILtpLocationProvider
  {
  private:
    std::string instance;
    int iLatitude;
    int iLongitude;
    
  public: 
    CLtpDummyLocationProvider(std::string aInstance, int aLatitude, int aLongitude);
    virtual ~CLtpDummyLocationProvider();

    //! See base class
    CLtpWGS84Coordinates getDestination();

    //! See base class
    CLtpWGS84Coordinates getCurrentCarPosition();
  };
} //namespace NLtp


#endif  // _LTPDUMMYLOCATIONPROVIDER_H_