//
// This file contain DummyCloudConnector interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPDUMMYCLOUDCONNECTOR_H_
#define _LTPDUMMYCLOUDCONNECTOR_H_

#include "LtpCommon.h"
#include "LtpExceptions.h"

#include <stdio.h>
#include <string>

namespace NLtp //Namespace is LocationTransferProtocol
{
  class ILtpDummyCloudConnector 
  {
  private:
    
  public: 
    virtual ~ILtpDummyCloudConnector() {};
  };

  class CLtpDummyCloudConnector : public ILtpDummyCloudConnector
  {
  private:
    std::string instance;
    
  public: 
    CLtpDummyCloudConnector(std::string& instance)
    {
      this->instance = instance;
    }
    virtual ~CLtpDummyCloudConnector()
    {
    }
  };
} //namespace NLtp


#endif  // _LTPDUMMYCLOUDCONNECTOR_H_
