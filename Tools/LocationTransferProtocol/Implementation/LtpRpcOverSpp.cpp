//
// This file contain implementation of NLtp::ILtpRpcOverSpp for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpRpcOverSpp.h"
#include "ILogging.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <stdio.h>

using namespace NLtp;

extern logging::TLogComponent iLtpLogId;

std::string RequestMessage::ToJson()  // REQ
{
  LogInterface( iLtpLogId, "");
  using namespace rapidjson;

  // The RequestMessage JSON document is as following :
  //{
  //    "ptl_ver": {
  //        "major": 1
  //        "minor": 0,
  //    },
  //    "msg_type": "REQ",
  //    "msg_id": 0,
  //    "method": "getUserData",
  //    "parameters": [
  //        [
  //            "DESTINATION",
  //            "LAST_KNOWN_CAR_POSITION"
  //        ]
  //    ]
  //}

#if 1
  // Use GenericValue::AddMember() to create the whole string.
  rapidjson::Document& ResultDocument = (*getJsonDocument());
  rapidjson::Value& ResultParameter = (*getParameters());
  LogInterface( iLtpLogId, "ResultDocument : " << CLtpJsonTools::JsonValueToString(ResultDocument).c_str());
  LogInterface( iLtpLogId, "ResultParameter : " << CLtpJsonTools::JsonValueToString(ResultParameter).c_str());
  //printf("ResultDocument : %s\n", CLtpJsonTools::JsonValueToString(ResultDocument).c_str());
  //printf("ResultParameter : %s\n", CLtpJsonTools::JsonValueToString(ResultParameter).c_str());

  rapidjson::Value JObjPtlVer(rapidjson::kObjectType);
  JObjPtlVer.AddMember(Ltp_major, getPtlVer()->getMajor(), ResultDocument.GetAllocator());
  JObjPtlVer.AddMember(Ltp_minor, getPtlVer()->getMinor(), ResultDocument.GetAllocator());
  ResultDocument.AddMember(Ltp_ptl_ver, JObjPtlVer, ResultDocument.GetAllocator());
  
  ResultDocument.AddMember(Ltp_msg_type, getMsgType().c_str(), ResultDocument.GetAllocator());
  ResultDocument.AddMember(Ltp_msg_id, getMsgId(), ResultDocument.GetAllocator());
  ResultDocument.AddMember(Ltp_method, getMethod().c_str(), ResultDocument.GetAllocator());
  ResultDocument.AddMember(Ltp_parameters, (*getParameters()), ResultDocument.GetAllocator());

  //CLtpJsonTools::QueringObjectMembersAndTypes(ResultDocument);
  
  std::string FinalJsonString = CLtpJsonTools::JsonValueToString(ResultDocument);
  LogInterface( iLtpLogId, "FinalJsonString : " << FinalJsonString.c_str());
  //printf("FinalJsonString : %s\n", FinalJsonString.c_str());
  
  return FinalJsonString;
#else
  // Use GenericValue::StartObject() to create the whole string.
  StringBuffer strbuf;
  Writer<StringBuffer> writer(strbuf);
  
  // Create REQ json string to this Writer.
  writer.StartObject();
    writer.String(Ltp_ptl_ver);
      writer.StartObject();
        writer.String(Ltp_major);   writer.Uint(getPtlVer()->getMajor());
        writer.String(Ltp_minor);   writer.Uint(getPtlVer()->getMinor());
      writer.EndObject();
    writer.String(Ltp_msg_type);    writer.String(getMsgType().c_str());
    writer.String(Ltp_msg_id);      writer.Uint(getMsgId());
    writer.String(Ltp_method);      writer.String(getMethod().c_str());

  // Start to fill all the "parameters".
  //-----------------------------------------------
  writer.String(Ltp_parameters);
    writer.StartArray();
      // string content will be fill later...
    writer.EndArray();
  //-----------------------------------------------
  writer.EndObject();

  // Start to merge the parameter string.
  std::string tmpStr = strbuf.GetString();
  std::string tmpPar = CLtpJsonTools::JsonValueToString(*getParameters());
  LogInterface( iLtpLogId, "tmpStr:" << tmpStr.c_str());
  LogInterface( iLtpLogId, "tmpPar:" << tmpPar.c_str());
  LogInterface( iLtpLogId, "tmpStr.length():" << tmpStr.length());
  LogInterface( iLtpLogId, "tmpPar.length():" << tmpPar.length());
  
  tmpStr.replace(tmpStr.length()-3, tmpPar.length(), tmpPar);
  tmpStr += "}";
  LogInterface( iLtpLogId, "Final tmpStr:" << tmpStr.c_str());
  
  return tmpStr;
#endif
}

std::string RequestMessage::BuildJson(CVersion* ptl_ver, std::string msg_type, int msg_id, std::string method, rapidjson::Document& document, rapidjson::Value& params) // REQ
{
  LogInterface( iLtpLogId, "");
  setPtlVer(ptl_ver);
  setMsgType(msg_type);
  setMsgId(msg_id);
  setMethod(method);
  setParameters(&params);
  setJsonDocument(&document);
  return ToJson();
}

std::string ResponseMessage::ToJson() // RESP 
{
  LogInterface( iLtpLogId, "");
  using namespace rapidjson;

// The ResponseMessage JSON document is as following :
//{
//    "ptl_ver": {
//        "major": 1,
//        "minor": 0
//    },
//    "msg_type": "RESP",
//    "msg_id": 0,
//    "method": "getUserData",
//    "resp_code": 0,
//    "parameters": [
//        {
//            "DESTINATION": {
//                "timestamp": "2014-05-30T13:25:08.000+0800",
//                "coordinates": {
//                    "latitude": 51505541,
//                    "longitude": -118420
//                }
//            },
//            "LAST_KNOWN_CAR_POSITION": {
//                "timestamp": "2014-05-30T13:25:08.000+0800",
//                "coordinates": {
//                    "latitude": 0,
//                    "longitude": 0
//                }
//            }
//        }
//    ]
//}

#if 1
  // Use GenericValue::AddMember() to create the whole string.
  rapidjson::Document& ResultDocument = (*getJsonDocument());
  rapidjson::Value& ResultParameter = (*getParameters());
  LogInterface( iLtpLogId, "ResultDocument : " << CLtpJsonTools::JsonValueToString(ResultDocument).c_str());
  LogInterface( iLtpLogId, "ResultParameter : " << CLtpJsonTools::JsonValueToString(ResultParameter).c_str());
  //printf("ResultDocument : %s\n", CLtpJsonTools::JsonValueToString(ResultDocument).c_str());
  //printf("ResultParameter : %s\n", CLtpJsonTools::JsonValueToString(ResultParameter).c_str());
  
  rapidjson::Value JObjPtlVer(rapidjson::kObjectType);
  JObjPtlVer.AddMember(Ltp_major, getPtlVer()->getMajor(), ResultDocument.GetAllocator());
  JObjPtlVer.AddMember(Ltp_minor, getPtlVer()->getMinor(), ResultDocument.GetAllocator());
  ResultDocument.AddMember(Ltp_ptl_ver, JObjPtlVer, ResultDocument.GetAllocator());

  ResultDocument.AddMember(Ltp_msg_type, getMsgType().c_str(), ResultDocument.GetAllocator());
  ResultDocument.AddMember(Ltp_msg_id, getMsgId(), ResultDocument.GetAllocator());
  ResultDocument.AddMember(Ltp_method, getMethod().c_str(), ResultDocument.GetAllocator());
  ResultDocument.AddMember(Ltp_resp_code, getRespCode(), ResultDocument.GetAllocator());
  ResultDocument.AddMember(Ltp_parameters, ResultParameter, ResultDocument.GetAllocator());

  //CLtpJsonTools::QueringObjectMembersAndTypes(ResultDocument);

  std::string FinalJsonString = CLtpJsonTools::JsonValueToString(ResultDocument);
  LogInterface( iLtpLogId, "FinalJsonString : " << FinalJsonString.c_str());
  //printf("FinalJsonString : %s\n", FinalJsonString.c_str());

  return FinalJsonString;
#else
  // Use GenericValue::StartObject() to create the whole string.
  StringBuffer strbuf;
  Writer<StringBuffer> writer(strbuf);
  
  // Create RESP json string to this Writer.
  writer.StartObject();
    writer.String(Ltp_ptl_ver);
      writer.StartObject();
        writer.String(Ltp_major);   writer.Uint(getPtlVer()->getMajor());
        writer.String(Ltp_minor);   writer.Uint(getPtlVer()->getMinor());
      writer.EndObject();
    writer.String(Ltp_msg_type);    writer.String(getMsgType().c_str());
    writer.String(Ltp_msg_id);      writer.Uint(getMsgId());
    writer.String(Ltp_method);      writer.String(getMethod().c_str());
    writer.String(Ltp_resp_code);   writer.Uint(getRespCode());

  // Start to fill all the "parameters".
  //-----------------------------------------------
  writer.String(Ltp_parameters);
    writer.StartArray();
      // string content will be fill later...
    writer.EndArray();
  //-----------------------------------------------
  writer.EndObject();
  
  // Start to merge the parameter string.
  std::string tmpStr = strbuf.GetString();
  std::string tmpPar = CLtpJsonTools::JsonValueToString(*getParameters());
  LogInterface( iLtpLogId, "tmpStr:" << tmpStr.c_str());
  LogInterface( iLtpLogId, "tmpPar:" << tmpPar.c_str());
  LogInterface( iLtpLogId, "tmpStr.length():" << tmpStr.length());
  LogInterface( iLtpLogId, "tmpPar.length():" << tmpPar.length());

  tmpStr.replace(tmpStr.length()-3, tmpPar.length(), tmpPar);
  tmpStr += "}";
  LogInterface( iLtpLogId, "Final tmpStr:" << tmpStr.c_str());
  
  return tmpStr;
#endif
}

std::string ResponseMessage::BuildJson(CVersion* ptl_ver, std::string msg_type, int msg_id, std::string method, int resp_code, rapidjson::Document& document, rapidjson::Value& params) // RESP
{
  LogInterface( iLtpLogId, "");
  setPtlVer(ptl_ver);
  setMsgType(msg_type);
  setMsgId(msg_id);
  setMethod(method);
  setRespCode(resp_code);
  setParameters(&params);
  setJsonDocument(&document);
  return ToJson();
}

CLtpRpcOverSpp::CLtpRpcOverSpp(std::string& aInstance, TSocketType aSocketType) : 
  tconnectionStatus(EConnectionStatusNOT_CONNECTED)
  , connection(NULL)
  , iInstance(NULL)
  , connectionStatusListener(NULL)
  , requestListener(NULL)
  , responseListener(NULL)
  , protocolVersion(NULL)
  , iLtpTimerREQ(NULL)
  , iLtpTimerRESP(NULL)
{
  LogInterface( iLtpLogId, "");
  
  // The instance will be "HU: " or "SP: "
  iInstance = &aInstance;

  // Create a lower layer CLtpConnection, and register myself(CLtpRpcOverSpp) as its Listener.
  connection = new CLtpConnection(*iInstance, aSocketType);
  connection->registerListener(*iInstance, *this);

  // Dummy, fixed status.
  // FIX ME: 
  // Underlying Bluetooth/Wifi connection status should be check first.
  tconnectionStatus = EConnectionStatusCONNECTED;  // Dummy, fixed status.

  // Create protocol version.
  protocolVersion = new CVersion(1, 0);

  // Create timer objects.
  iLtpTimerREQ = new CLtpTimer();
  iLtpTimerRESP = new CLtpTimer();
  iLtpTimerREQ->TimerCreate(CLtpRpcOverSpp::fn_timer_expire_handler_REQ, SIG_TIMER1);
  iLtpTimerRESP->TimerCreate(CLtpRpcOverSpp::fn_timer_expire_handler_RESP, SIG_TIMER2);
  iLtpTimerREQ->TimerStop();
  iLtpTimerRESP->TimerStop();
  
}

CLtpRpcOverSpp::~CLtpRpcOverSpp()
{
  LogInterface( iLtpLogId, "");

  if (connection)
  {
    connection->unregisterListener(*iInstance);
    delete connection;
    connection = NULL;
  }

  if (protocolVersion)
  {
    delete protocolVersion;
    protocolVersion = NULL;
  }

  // Delete timer objects.
  if (iLtpTimerREQ)
  {
    iLtpTimerREQ->TimerDelete();
    delete iLtpTimerREQ;
    iLtpTimerREQ = NULL;
  }
  if (iLtpTimerRESP)
  {
    iLtpTimerRESP->TimerDelete();
    delete iLtpTimerRESP;
    iLtpTimerRESP = NULL;
  }

  // Dummy, fixed status.
  // FIX ME: 
  // Underlying Bluetooth/Wifi connection status should be check first.
  tconnectionStatus = EConnectionStatusNOT_CONNECTED;  // Dummy, fixed status.

  iInstance = NULL;
  connectionStatusListener = NULL;
  requestListener = NULL;
  responseListener = NULL;
}

void CLtpRpcOverSpp::registerConnectionListener(ILtpRpcOverSppListener* listener)
{
  LogInterface( iLtpLogId, "");

  connectionStatusListener = listener;   // registered UserDataConnector layer as listener of this layer, but in this implementation it will never receive updates.
  connectionStatusListener->connectionStatus(tconnectionStatus); //let UserDataConnector layer know the connection status of RpcOverSpp layer.
}

void CLtpRpcOverSpp::unregisterConnectionListener(ILtpRpcOverSppListener* listener)
{
  LogInterface( iLtpLogId, "");
  connectionStatusListener = NULL;
}

void CLtpRpcOverSpp::registerRequestListener(ILtpRpcOverSppListener* listener)
{
  LogInterface( iLtpLogId, "");
  requestListener = listener;
}

void CLtpRpcOverSpp::registerResponseListener(ILtpRpcOverSppListener* listener)
{
  LogInterface( iLtpLogId, "");
  responseListener = listener;
}

void CLtpRpcOverSpp::sendRequest(int requestId, ILtpRpcOverSppListener* listener, std::string methodName, rapidjson::Document& document, rapidjson::Value& parameters)
{
  LogInterface( iLtpLogId, "");

  if (listener)
    registerResponseListener(listener);

  // Convert rapidjson::Value to string.
  //###############################################################################
  std::string actual_msg;
  RequestMessage* requestMessage = new RequestMessage();
  actual_msg = requestMessage->BuildJson(protocolVersion, Ltp_msg_types_req, requestId, methodName, document, parameters);
  delete requestMessage;
  //printf("sendRequest actual_msg: %s\n", actual_msg.c_str());
  //###############################################################################

  // TODO Start time-out timer.
  //iLtpTimerREQ->TimerStart(1, 500000000UL, 0, 0);

  try
  {
    connection->blockSocketSendMessage(*iInstance, actual_msg);
  }
  catch (CLtpBaseException& e)
  {
    LogError( iLtpLogId, ""<<e.what());
  }
}

void CLtpRpcOverSpp::sendResponse(int requestId, std::string methodName, rapidjson::Document& document, rapidjson::Value& parameters)
{
  LogInterface( iLtpLogId, "");
  // Convert rapidjson::Value to string.
  //###############################################################################
  std::string actual_msg;
  ResponseMessage* responseMessage = new ResponseMessage();
  actual_msg = responseMessage->BuildJson(protocolVersion, Ltp_msg_types_resp, requestId, methodName, ResponseMessage::EResponseCode_MESSAGE_OK, document, parameters);
  delete responseMessage;
  //###############################################################################

  // TODO Start time-out timer.
  //iLtpTimerRESP->TimerStart(1, 500000000UL, 0, 0);
  
  try
  {
    connection->blockSocketSendMessage(*iInstance, actual_msg);
  }
  catch (CLtpBaseException& e)
  {
    LogError( iLtpLogId, ""<<e.what());
  }
}

void CLtpRpcOverSpp::sendErrorReplyMessage(int respCode, int requestId)
{
  rapidjson::Document document;// Set this document as an empty object.
  document.SetObject();
  
  rapidjson::Value parameters;//create an empty json array.
  parameters.SetArray();
  parameters.Clear();
  
  std::string actual_msg;
  ResponseMessage* responseMessage = new ResponseMessage();
  //params.clear();
  actual_msg = responseMessage->BuildJson(protocolVersion, Ltp_msg_types_resp, requestId, "", respCode, document, parameters);
  delete responseMessage;
  
  try
  {
    connection->blockSocketSendMessage(*iInstance, actual_msg);
  }
  catch (CLtpBaseException& e)
  {
    LogError( iLtpLogId, ""<<e.what());
  }
}

int CLtpRpcOverSpp::isJsonObjectErrReply(const rapidjson::Value& json, ResponseMessage::TResponseCode status)
{
 if (!json.IsObject())
 {
   // Sending back a error response.
   sendErrorReplyMessage(status, 0);
   return 1;
 }
 return 0;
}

int CLtpRpcOverSpp::isJsonArrayErrReply(const rapidjson::Value& json, ResponseMessage::TResponseCode status)
{
 if (!json.IsArray())
 {
   // Sending back a error response.
   sendErrorReplyMessage(status, 0);
   return 1;
 }
 return 0;
}

void CLtpRpcOverSpp::isJsonObjectException(const rapidjson::Value& json)
{
 if (!json.IsObject())
 {
   // The message is not well-formed.
   // We don't know whether it was a request or response.
   // So nothing is sent back, only an error is logged.
   throw CLtpRuntimeException("Illegal JSON Value, must start from Object {...}");
 }
}

void CLtpRpcOverSpp::messageReceived(const std::string& message)
{
  try 
  {
    // Try to parse this message, we assume that it should be a valid JSON document.
    if (iJsonParser.Parse<0>(message.c_str()).HasParseError())// "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
    {
      // The message is not a valid JSON document.
      // We don't know whether it was a request or response.
      // So nothing is sent back, only an error is logged.
      throw CLtpJsonParseException("Not a valid JSON document");
    }

    // FIX ME: We assume that all messages in this protocol should start with JSON Object mark {...}.
    isJsonObjectException(iJsonParser);

    // Check for request of response
    //String msgType = (String) jsonObject.get(Ltp_msg_type);
    if (!iJsonParser.HasMember(Ltp_msg_type) || //(msgType == null) 
        (!iJsonParser[Ltp_msg_type].IsString()) )
    {
      // The message is not well-formed.
      // We don't know whether it was a request or response.
      // So nothing is sent back, only an error is logged.
      throw CLtpRuntimeException("Missing property msg_type");
    }
    
    std::string msgType = iJsonParser[Ltp_msg_type].GetString();//iJsonParser[Ltp_msg_type].GetString()
    //printf("msgType:%s\n", msgType.c_str());

    if (0 == msgType.compare(Ltp_msg_types_req)) 
    {
      handleRequest(iJsonParser);
    } 
    else if (0 == msgType.compare(Ltp_msg_types_resp)) 
    {
      handleResponse(iJsonParser);
    } 
    else 
    {
      // The message is not well-formed.
      // It doesn't seem a request nor a response.
      // So nothing is sent back, only an error is logged.
      throw CLtpRuntimeException("Illegal msg_type: " + msgType);
    }
  } 
  catch (CLtpBaseException& e) 
  {
    // The message is not well-formed.
    // We don't know whether it was a request or response.
    // So nothing is sent back, only an error is logged.
    LogError( iLtpLogId, ""<< e.what());
  }
}

void CLtpRpcOverSpp::handleRequest(rapidjson::Document& jsonObject)
{
  if (isJsonObjectErrReply(jsonObject, ResponseMessage::EResponseCode_MESSAGE_NOT_WELL_FORMED)) 
    return;

  //CLtpJsonTools::QueringObjectMembersAndTypes(jsonObject);
  
  // Check HasMembers: (Should change to FindMember() after rapidjson 2.x released in the future, please check http://thisthread.blogspot.tw/2013/10/skipping-missing-element-with-rapidjson.html.): 
  // v.HasMember() should only be called when v is an object.
  // If v is an array, you should use IsNull() to skip or detect missing json key string, check the URL above.
  if (!jsonObject.HasMember(Ltp_ptl_ver) || 
      !jsonObject.HasMember(Ltp_msg_type) || //Already checked!
      !jsonObject.HasMember(Ltp_msg_id) || 
      !jsonObject.HasMember(Ltp_method) || 
      !jsonObject.HasMember(Ltp_parameters) ) 
  {
    sendErrorReplyMessage(ResponseMessage::EResponseCode_MESSAGE_NOT_WELL_FORMED, 0);
    return;
  }
  
  // Get protocol version for check
  if (isJsonObjectErrReply(jsonObject[Ltp_ptl_ver], ResponseMessage::EResponseCode_MESSAGE_NOT_WELL_FORMED)) 
    return;
  const rapidjson::Value& ptlVerJson = jsonObject[Ltp_ptl_ver];

  if (!ptlVerJson[Ltp_major].IsNumber() ||  // Number is a JSON type, but C++ needs more specific type.
      !ptlVerJson[Ltp_major].IsInt() ) 
  {
    sendErrorReplyMessage(ResponseMessage::EResponseCode_MESSAGE_NOT_WELL_FORMED, 0);
    return;
  }
  int major = ptlVerJson[Ltp_major].GetInt();

  if (major != 1) 
  {
    sendErrorReplyMessage(ResponseMessage::EResponseCode_INCOMPATIBLE_PROTOCOL_VERSION, 0);
  } 
  else 
  {
    // msg_id
    if (!jsonObject[Ltp_msg_id].IsNumber() || 
        !jsonObject[Ltp_msg_id].IsInt() ) 
    {
      sendErrorReplyMessage(ResponseMessage::EResponseCode_MESSAGE_NOT_WELL_FORMED, 0);
      return;
    }
    int requestId = jsonObject[Ltp_msg_id].GetInt();

    // method
    if (!jsonObject[Ltp_method].IsString())
    {
      sendErrorReplyMessage(ResponseMessage::EResponseCode_MESSAGE_NOT_WELL_FORMED, requestId);
      return;
    }
    std::string method = jsonObject[Ltp_method].GetString();

    // parameters
    if (isJsonArrayErrReply(jsonObject[Ltp_parameters], ResponseMessage::EResponseCode_MESSAGE_NOT_WELL_FORMED)) 
      return;
    rapidjson::Value& parametersArray = jsonObject[Ltp_parameters];

    // Send to request listener.
    requestListener->requestReceived(requestId, method, parametersArray);
  }
}

void CLtpRpcOverSpp::handleResponse(rapidjson::Document& jsonObject)
{
  if (isJsonObjectErrReply(jsonObject, ResponseMessage::EResponseCode_MESSAGE_NOT_WELL_FORMED)) 
    return;

  //CLtpJsonTools::QueringObjectMembersAndTypes(jsonObject);
  
  // Check HasMembers: (Should change to FindMember() after rapidjson 2.x released in the future, please check http://thisthread.blogspot.tw/2013/10/skipping-missing-element-with-rapidjson.html.): 
  // v.HasMember() should only be called when v is an object.
  // If v is an array, you should use IsNull() to skip or detect missing json key string, check the URL above.
  if (!jsonObject.HasMember(Ltp_ptl_ver) || 
      !jsonObject.HasMember(Ltp_msg_type) || //Already checked!
      !jsonObject.HasMember(Ltp_msg_id) || 
      !jsonObject.HasMember(Ltp_method) || 
      !jsonObject.HasMember(Ltp_resp_code) || 
      !jsonObject.HasMember(Ltp_parameters) ) 
  {
    // Response message is not well-formed. We cannot handle it.
    // Log an error. A timeout will occur, so the client receives a time-out error.
    LogInfo( iLtpLogId, "ERROR: Response message is not well-formed.");
    return;
  }
  
  // Get protocol version for check
  if (!jsonObject[Ltp_ptl_ver].IsObject()) 
  {
    // Response message is not well-formed. We cannot handle it.
    // Log an error. A timeout will occur, so the client receives a time-out error.
    LogInfo( iLtpLogId, "ERROR: reply received without ptl_ver property set.");
    return;
  }
  const rapidjson::Value& ptlVerJson = jsonObject[Ltp_ptl_ver];

  if (!ptlVerJson[Ltp_major].IsNumber() ||  // Number is a JSON type, but C++ needs more specific type.
      !ptlVerJson[Ltp_major].IsInt() ) 
  {
    // Response message is not well-formed. We cannot handle it.
    // Log an error. A timeouut will occur, so the client receives a time-out error.
    LogInfo( iLtpLogId, "ERROR: reply received without ptl_ver.major property set.");
    return;
  }
  int major = ptlVerJson[Ltp_major].GetInt();

  if (major != 1) 
  {
    // Incompatible protocol version. We cannot handle it.
    // Log an error. A timeout will occur, so the client receives a time-out error.
    LogInfo( iLtpLogId, "ERROR: Incompatible protocol version.");
    return;
  } 
  else 
  {
    // msg_id
    if (!jsonObject[Ltp_msg_id].IsNumber() || 
        !jsonObject[Ltp_msg_id].IsInt() ) 
    {
      // Response message is not well-formed. We cannot handle it.
      // Log an error. A timeout will occur, so the client receives a time-out error.
      LogInfo( iLtpLogId, "ERROR: reply received without msg_id.");
      return;
    }
    int requestId = jsonObject[Ltp_msg_id].GetInt();

    // method
    if (!jsonObject[Ltp_method].IsString())
    {
      // Response message is not well-formed. We cannot handle it.
      // Log an error. A timeout will occur, so the client receives a time-out error.
      LogInfo( iLtpLogId, "ERROR: reply received without method.");
      return;
    }
    std::string method = jsonObject[Ltp_method].GetString();

    // resp_code
    if (!jsonObject[Ltp_resp_code].IsNumber() || 
        !jsonObject[Ltp_resp_code].IsInt() ) 
    {
      // Response message is not well-formed. We cannot handle it.
      // Log an error. A timeout will occur, so the client receives a time-out error.
      LogInfo( iLtpLogId, "ERROR: reply received without resp_code.");
      return;
    }
    int resp_code = jsonObject[Ltp_resp_code].GetInt();
    //printf("resp_code:%d\n", resp_code);

    // TODO Stop the time-out timer.
    iLtpTimerREQ->TimerStop();

    if (resp_code != 0) {
      responseListener->responseErrorReceived(requestId, method, resp_code);
    }

    // parameters
    if (!jsonObject[Ltp_parameters].IsArray()) 
    {
      // Response message is not well-formed. We cannot handle it.
      // Log an error. A timeout will occur, so the client receives a time-out error.
      LogInfo( iLtpLogId, "ERROR: reply received without parameter array.");
      return;
    }
    rapidjson::Value& parametersArray = jsonObject[Ltp_parameters];

    // Send to request listener.
    responseListener->responseReceived(requestId, method, parametersArray);
  }
}

void CLtpRpcOverSpp::fn_timer_expire_handler_REQ(int sig, siginfo_t* si, void* uc)
{
  static int countA=0;
  countA++;
  
  /* UNSAFE: This handler uses non-async-signal-safe functions printf() */
  LogInfo( iLtpLogId, "fn_timer_expire_handler_REQ()");
}

void CLtpRpcOverSpp::fn_timer_expire_handler_RESP(int sig, siginfo_t* si, void* uc)
{
  static int countB=0;
  countB++;
  
  /* UNSAFE: This handler uses non-async-signal-safe functions printf() */
  LogInfo( iLtpLogId, "fn_timer_expire_handler_RESP()");
}

bool CLtpRpcOverSpp::startRcpOverSppServerLoop()
{
  return connection->startConnectorSocketServerLoop(0);
}

