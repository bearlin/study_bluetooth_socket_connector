//
// This file contain common datas functions for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpUserDataService.h"
#include "ILogging.h"

using namespace NLtp;

logging::TLogComponent iLtpLogId;

CLtpUserDataService::CLtpUserDataService(std::string instance, TSocketType aSocketType) : 
  iLocationProvider(NULL), 
  cloudConnector(NULL),
  userDataConnector(NULL), 
  destinationListener(NULL),
  iSessionRequestId(100) // RequestId will start from 100.
{
  logging::IControl().RegisterComponent( "ExternalHandler", iLtpLogId);
  LogInterface( iLtpLogId, "");

  this->instance = instance;

  cloudConnector = new CLtpDummyCloudConnector(instance);
  userDataConnector = new CLtpUserDataConnector(instance, aSocketType);
  userDataConnector->addConnectionListener(this);
  userDataConnector->registerRequestHandler(this);

}

CLtpUserDataService::~CLtpUserDataService()
{
  if (cloudConnector)
  {
    delete cloudConnector;
  }
  
  if (userDataConnector)
  {
    delete userDataConnector;
  }
}

void CLtpUserDataService::shareDestination(CLtpWGS84Coordinates& destination)
{
  // TODO Implementation will be shown later. Destination will be sent to SP and Cloud server.
  
}

void CLtpUserDataService::addDestinationListener(ILtpUserDataServiceListener* listener)
{
  destinationListener = listener;
}

void CLtpUserDataService::GetLtpLocations(std::vector<TLocationType>& locationTypes)
{
  LogInterface( iLtpLogId, " iSessionRequestId="<<iSessionRequestId);
  // SP site should start a sendLocationsRequest() action!
  iSessionRequestId++;// = 123; // This shall of course change per call (use requestId-Listener map)
  // Send a request for the Destination, reply is received by the listener.
  userDataConnector->sendLocationsRequest(iSessionRequestId, locationTypes, this);
}

bool CLtpUserDataService::enterListeningLoop()
{
  // HU site should entering a waiting loop(passive mode), to wait for client connection.!
  LogInterface( iLtpLogId, "");
  
  // HU should entering its waiting loop for message R/W from socket!
  return userDataConnector->startUserDataConnectorServerLoop();
}

void CLtpUserDataService::addLocationProvider(ILtpLocationProvider* aLocationProvider)
{
  iLocationProvider = aLocationProvider;
}

void CLtpUserDataService::connectionStatus(TConnectionStatus connectionStatus)
{
  // Show the connection status.
}

void CLtpUserDataService::getLocationsRequest(int requestId, const std::vector<TLocationType>& locationTypes)
{
  LogInterface( iLtpLogId, " requestId="<<requestId);

  std::vector<CLtpTypedLocation> typedLocations;// = new ArrayList<>();
  CLtpTypedLocation typedLocation;

  std::string timeStamp = CLtpTimestampedLocation::GetLocalTimeStamp();

  // Make sure we have iLocationProvider instance.
  assert(NULL != iLocationProvider);

  for (std::vector<TLocationType>::const_iterator it = locationTypes.begin(); it != locationTypes.end(); ++it) 
  {
    switch (*it) 
    {
    case ELocationType_DESTINATION:
      {
        // FIX ME:
        // We should get location info from UIApplication or LocationProvider!
        //
        typedLocation.setNavDataType(ELocationType_DESTINATION);
        CLtpWGS84Coordinates destination = iLocationProvider->getDestination();
        CLtpTimestampedLocation timeStampedDestination;// = new CLtpTimestampedLocation();
        timeStampedDestination.setCoordinates(destination);
        timeStampedDestination.setTimestamp(timeStamp);
        typedLocation.setNavData(timeStampedDestination);
        typedLocations.push_back(typedLocation);
      }
      break;

    case ELocationType_LAST_KNOWN_CAR_POSITION:
      {
        // FIX ME:
        // We should get location info from UIApplication or LocationProvider!
        //
        typedLocation.setNavDataType(ELocationType_LAST_KNOWN_CAR_POSITION);
        CLtpWGS84Coordinates lastKnownCarPosition = iLocationProvider->getCurrentCarPosition();
        CLtpTimestampedLocation timeStampedLastKnownCarPosition;// = new CLtpTimestampedLocation();
        timeStampedLastKnownCarPosition.setCoordinates(lastKnownCarPosition);
        timeStampedLastKnownCarPosition.setTimestamp(timeStamp);
        typedLocation.setNavData(timeStampedLastKnownCarPosition);
        typedLocations.push_back(typedLocation);
      } 
      break;
      
    default:
      LogError( iLtpLogId, " Unknow TLocationType="<<(*it));
      break;
    }
  }

  userDataConnector->sendLocationsResponse(requestId, typedLocations);
}

void CLtpUserDataService::getLocationsReply(int requestId, int respCode, const std::vector<CLtpTypedLocation>& typedLocations)
{

  // Normally, match the requestId with the corresponding request.
  // For now just check whether the response has the same id as the request.
  if (requestId != iSessionRequestId) 
  {
    LogError( iLtpLogId, "ERROR: Wrong RequestId!!! requestId:"<<requestId<<", iSessionRequestId:"<<iSessionRequestId);
    //System.exit(-1);
    return;
  }
  
  if (respCode != 0) 
  {
    LogError( iLtpLogId, "ERROR: Request could not be handled because a RESP_ERROR received!");
    //System.exit(-1);
    return;
  }
  
  // Get the Destination from navDataItems.
  // As we only asked for the ELocationType_DESTINATION:
  // - there is either 1 item in the list; the ELocationType_DESTINATION
  // - or the list is empty; meaning that the ELocationType_DESTINATION wasn't available
  if (typedLocations.size() == 0) 
  {
    LogInfo( iLtpLogId, "CLtpUserDataService::getLocationsReply Done: No LCKP or Destination available!");
  } 
  else 
  {
    // Loop through all the std::vector<CLtpTypedLocation> elements, and extract their contents.
    //std::vector<CLtpTypedLocation>::const_iterator it = typedLocations.begin();
    for (std::vector<CLtpTypedLocation>::const_iterator it = typedLocations.begin(); it != typedLocations.end(); ++it)
    {
      if (it->getNavDataType() == ELocationType_LAST_KNOWN_CAR_POSITION) 
      {
        // Extracting ELocationType_LAST_KNOWN_CAR_POSITION data.......
        const CLtpTimestampedLocation& navData = it->getNavData();
        //if (navData instanceof CLtpTimestampedLocation) 
        {
          const CLtpTimestampedLocation& timestampedLocation = navData;//(CLtpTimestampedLocation) navData;

          // here you could do something smart with the timestamp.
          // for now just show it.
          LogInfo( iLtpLogId, "CLtpUserDataService::getLocationsReply: Timestamp = " << timestampedLocation.getTimestamp().c_str());
          
          // Get the Destination and set it in the Navigation application
          const CLtpWGS84Coordinates& destination = timestampedLocation.getCoordinates();
          LogInfo( iLtpLogId, "CLtpUserDataService::getLocationsReply: Guiding you to LKCP: " << destination.getLatitude() << ", " << destination.getLongitude());
          
          // we're done.
          LogInfo( iLtpLogId, "CLtpUserDataService::getLocationsReply LKCP Done!");

          // Notify the listener.
          if (destinationListener)
            destinationListener->destinationReceived(timestampedLocation);
        }
      } 
      else if (it->getNavDataType() == ELocationType_DESTINATION) 
      {
        // Extracting ELocationType_DESTINATION data.......
        const CLtpTimestampedLocation& navData = it->getNavData();
        //if (navData instanceof CLtpTimestampedLocation) 
        {
          const CLtpTimestampedLocation& timestampedLocation = navData;//(CLtpTimestampedLocation) navData;

          // here you could do something smart with the timestamp.
          // for now just show it.
          LogInfo( iLtpLogId, "CLtpUserDataService::getLocationsReply: Timestamp = " << timestampedLocation.getTimestamp().c_str());
          // Get the Destination and set it in the Navigation application
          const CLtpWGS84Coordinates& destination = timestampedLocation.getCoordinates();
          LogInfo( iLtpLogId, "CLtpUserDataService::getLocationsReply: Guiding you to DEST: " << destination.getLatitude() << ", " << destination.getLongitude());
          
          // we're done.
          LogInfo( iLtpLogId, "CLtpUserDataService::getLocationsReply DEST Done!");
          
          // Notify the listener.
          if (destinationListener)
            destinationListener->destinationReceived(timestampedLocation);
        }
      }
      else
      {
        LogError( iLtpLogId, "LtpDemo: Wrong navDataItem received!");
        continue;
      }
    }
  }

}


