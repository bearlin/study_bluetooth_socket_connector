//
// This file contain CLtpRpcOverSpp UnitTest for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "MockILtpRpcOverSppListener.h"

#include <stdio.h>

using namespace NLtp;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;

class CLtpRpcOverSppTest: public ::testing::Test
{
};

TEST_F(CLtpRpcOverSppTest, CLtpRpcOverSppConstructorAndDestructor)
{
  std::string instance = "HU: ";
  CLtpRpcOverSpp* pRpcOverSppImpl = new CLtpRpcOverSpp(instance, ESocketType_IP);

  ASSERT_TRUE(pRpcOverSppImpl != NULL);

  delete pRpcOverSppImpl;
}

TEST_F(CLtpRpcOverSppTest, CLtpRpcOverSppRegisterAndUnregisterListener)
{
  std::string instance = "HU: ";
  CLtpRpcOverSpp* pRpcOverSppImpl = new CLtpRpcOverSpp(instance, ESocketType_IP);
  CRpcOverSppListenerMock* pUserDataConnectorImplMock = new CRpcOverSppListenerMock();

  EXPECT_CALL((*pUserDataConnectorImplMock), connectionStatus(EConnectionStatusCONNECTED))
    .Times(AtLeast(1))
    .WillRepeatedly(Return());
  pRpcOverSppImpl->registerConnectionListener(pUserDataConnectorImplMock);
  pRpcOverSppImpl->unregisterConnectionListener(pUserDataConnectorImplMock);
  pRpcOverSppImpl->registerRequestListener(pUserDataConnectorImplMock);
  pRpcOverSppImpl->registerResponseListener(pUserDataConnectorImplMock);
  
  delete pRpcOverSppImpl;
  delete pUserDataConnectorImplMock;
}

TEST_F(CLtpRpcOverSppTest, CreateResponseErrorMessages)
{
}

TEST_F(CLtpRpcOverSppTest, CreateResponseMessagesWithoutParameters)
{
}

TEST_F(CLtpRpcOverSppTest, CreateResponseMessagesWithParameters)
{
}

TEST_F(CLtpRpcOverSppTest, CreateRequestMessageWithoutParameters)
{
}

TEST_F(CLtpRpcOverSppTest, CreateRequestMessageWithParameters)
{
}

TEST_F(CLtpRpcOverSppTest, sendRequest)
{
}

TEST_F(CLtpRpcOverSppTest, sendResponse)
{
}

