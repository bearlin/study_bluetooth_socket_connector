//
// This file contain mock object of NLtp::ILtpConnectionListener for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _CONNECTION_LISTENER_MOCK_H_
#define _CONNECTION_LISTENER_MOCK_H_

#include "LtpConnection.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace NLtp //Namespace is LocationTransferProtocol
{

  class CConnectionListenerMock : public ILtpConnectionListener
  {
  public:
    MOCK_METHOD1(messageReceived, void(const std::string& message));
  };

}

#endif // _CONNECTION_LISTENER_MOCK_H_
