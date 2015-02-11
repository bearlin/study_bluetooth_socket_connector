//
// This file contain mock object of NLtp::ILtpRpcOverSppListener for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _RPCOVERSPP_LISTENER_MOCK_H_
#define _RPCOVERSPP_LISTENER_MOCK_H_

#include "LtpRpcOverSpp.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace NLtp //Namespace is LocationTransferProtocol
{
  class CRpcOverSppListenerMock : public ILtpRpcOverSppListener
  {
  public:
    virtual ~CRpcOverSppListenerMock() {}; 

    MOCK_METHOD1(connectionStatus, void(TConnectionStatus connectionStatus));
    MOCK_METHOD3(requestReceived, void(int requestId, std::string methodName, rapidjson::Value& parameters));
    MOCK_METHOD3(responseReceived, void(int requestId, std::string methodName, rapidjson::Value& parameters));
    MOCK_METHOD3(responseErrorReceived, void(int requestId, std::string methodName, int respCode));
  };
}

#endif // _RPCOVERSPP_LISTENER_MOCK_H_
