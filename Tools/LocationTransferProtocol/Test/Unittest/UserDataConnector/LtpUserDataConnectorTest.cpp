//
// This file contain CLtpUserDataConnector UnitTest for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "MockILtpUserDataConnectorListener.h"

#include <stdio.h>
#include <string>

using namespace NLtp;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;

class CLtpUserDataConnectorTest: public ::testing::Test
{
};

TEST_F(CLtpUserDataConnectorTest, CLtpUserDataConnectorConstructorAndDestructor)
{
  std::string instance("HU: ");
  CLtpUserDataConnector* puserDataConnector = new CLtpUserDataConnector(instance, ESocketType_IP);

  EXPECT_TRUE(puserDataConnector != NULL);
  
  delete puserDataConnector;
}



