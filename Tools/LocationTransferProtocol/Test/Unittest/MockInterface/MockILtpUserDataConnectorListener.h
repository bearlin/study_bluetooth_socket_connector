//
// This file contain mock object of NLtp::ILtpUserDataConnectorListener for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _USERDATACONNECTOR_LISTENER_MOCK_H_
#define _USERDATACONNECTOR_LISTENER_MOCK_H_

#include "LtpUserDataConnector.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace NLtp //Namespace is LocationTransferProtocol
{
  class CUserDataConnectorListenerMock : public ILtpUserDataConnectorListener
  {
  public:
    MOCK_METHOD1(connectionStatus, void(TConnectionStatus connectionStatus));
    MOCK_METHOD2(getLocationsRequest, void(int requestId, const std::vector<TLocationType>& locationTypes));
    MOCK_METHOD3(getLocationsReply, void(int requestId, int respCode, const std::vector<CLtpTypedLocation>& typedLocations));
  };
}

#endif // _USERDATACONNECTOR_LISTENER_MOCK_H_
