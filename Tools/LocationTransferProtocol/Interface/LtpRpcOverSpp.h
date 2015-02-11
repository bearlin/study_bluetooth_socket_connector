//
// This file contain RpcOverSpp interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPRPCOVERSPP_H_
#define _LTPRPCOVERSPP_H_

#include "LtpCommon.h"
#include "LtpExceptions.h"

#include "LtpConnection.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

namespace NLtp //Namespace is LocationTransferProtocol
{
  class ILtpRpcOverSppListener 
  {
  public: 
    //static int SYSTEM_ERROR = 1;   // Something went wrong in the communication. Details are logged at protocol level.
    //static int TIME_OUT_OCCURRED = 2;

    //! Caller:        RpcOverSpp layer (to notify UserDataConnector layer listener about current connection status).
    //! Implement: UserDataConnector layer.
    virtual void connectionStatus(TConnectionStatus connectionStatus) = 0;

    //! Caller:        RpcOverSpp layer (to notify UserDataConnector layer listener that a request message was received).
    //! Implement: UserDataConnector layer.
    //virtual void requestReceived(int requestId, std::string methodName, const std::vector<TLocationType> parameter) = 0;
    virtual void requestReceived(int requestId, std::string methodName, rapidjson::Value& parameters) = 0;

    //! Caller:        RpcOverSpp layer (to notify UserDataConnector layer listener that a response message was received).
    //! Implement: UserDataConnector layer.
    //virtual void responseReceived(int requestId, std::string methodName, const std::vector<CLtpTypedLocation> parameter) = 0;
    virtual void responseReceived(int requestId, std::string methodName, rapidjson::Value& parameters) = 0;

    //! Caller:        RpcOverSpp layer (to notify UserDataConnector layer listener that a response error message was received).
    //! Implement: UserDataConnector layer.
    virtual void responseErrorReceived(int requestId, std::string methodName, int respCode) = 0;
  };
  
  /**
   * Interface for the layer that implements the RCP protocol.
   * This interface is unaware of any real functionality like getting a destination.
   *
   * This interface could support multiple clients, but as this is not needed, and
   * it would require a more complicated mechanism for the 'request listeners',
   * only a single client is supported.
   */
  class ILtpRpcOverSpp 
  {
  public: 
    //! Caller:        UserDataConnector layer (to register him-self as a connection status listener).
    //! Implement: RpcOverSpp layer.
    virtual void registerConnectionListener(ILtpRpcOverSppListener* listener) = 0;

    //! Caller:        UserDataConnector layer (to un-register him-self as a connection status listener).
    //! Implement: RpcOverSpp layer.
    virtual void unregisterConnectionListener(ILtpRpcOverSppListener* listener) = 0;

    //! Caller:        UserDataConnector layer (to register him-self as a requests listener).
    //! Implement: RpcOverSpp layer.
    virtual void registerRequestListener(ILtpRpcOverSppListener* listener) = 0;

    //! Caller:        UserDataConnector layer (to register him-self as a response listener).
    //! Implement: RpcOverSpp layer.
    virtual void registerResponseListener(ILtpRpcOverSppListener* listener) = 0;
    
    //! Caller:        UserDataConnector layer (to send a request messages to the other side).
    //! Implement: RpcOverSpp layer.
    //virtual void sendRequest(int requestId, ILtpRpcOverSppListener* listener, std::string methodName, std::vector<TLocationType>& parameters) = 0;
    virtual void sendRequest(int requestId, ILtpRpcOverSppListener* listener, std::string methodName, rapidjson::Document& document, rapidjson::Value& parameters) = 0;

    //! Caller:        UserDataConnector layer (to send a response messages to the other side).
    //! Implement: RpcOverSpp layer.
    virtual void sendResponse(int requestId, std::string methodName, rapidjson::Document& document, rapidjson::Value& parameters) = 0;

    //! Caller:        UserDataConnector layer (Entering Server loop).
    //! Implement: RpcOverSpp layer.
    virtual bool startRcpOverSppServerLoop() = 0;
  };
  
  class CVersion 
  {
  private:
    int major;
    int minor;
      
  public: 
    CVersion() 
    {
      this->major = 0;
      this->minor = 0;
    };
      
    CVersion(int major, int minor) {
      this->major = major;
      this->minor = minor;
    }

    int getMajor() {
      return major;
    }

    void setMajor(int major) {
      this->major = major;
    }

    int getMinor() {
      return minor;
    }

    void setMinor(int minor) {
      this->minor = minor;
    }
  };
  
  class RequestHeader 
  {
  private: 
    CVersion* ptl_ver;
    std::string msg_type;
    int msg_id;
    std::string method;
    //std::vector<TLocationType>* parametersReq;// = new ArrayList<>();
    //rapidjson::Value* parametersReq;
    //std::vector<CLtpTypedLocation>* parametersResp;
    rapidjson::Value* parameters;
    rapidjson::Document* iDocument;
    
  public: 
    RequestHeader() 
      : ptl_ver(NULL)
      , msg_type("")
      , msg_id(0)
      , method("")
      , parameters(NULL)
      , iDocument(NULL)
    {
    }
    
    virtual ~RequestHeader()
    {
    }
    
    CVersion* getPtlVer() {
      return ptl_ver;
    }
    
    void setPtlVer(CVersion* ptlVer) {
      ptl_ver = ptlVer;
    }
    
    std::string getMsgType() {
      return msg_type;
    }
    
    void setMsgType(std::string msgType) {
      msg_type = msgType;
    }
    
    int getMsgId() {
      return msg_id;
    }
    
    void setMsgId(int msgId) {
      msg_id = msgId;
    }
    
    std::string getMethod() {
      return method;
    }
    
    void setMethod(std::string method) {
      this->method = method;
    }

    void setParameters(rapidjson::Value* params) {
      this->parameters = params;
    }
    
    rapidjson::Value* getParameters() {
      return this->parameters;
    }

    void setJsonDocument(rapidjson::Document* document) {
      this->iDocument = document;
    }
    
    rapidjson::Document* getJsonDocument() {
      return this->iDocument;
    }
  };
  
  class RequestMessage : public RequestHeader 
  {
  public:
    RequestMessage() 
      : RequestHeader()
    {
    }
    
    virtual ~RequestMessage()
    {
    }
    
    std::string ToJson();  // REQ
    std::string BuildJson(CVersion* ptl_ver, std::string msg_type, int msg_id, std::string method, rapidjson::Document& document, rapidjson::Value& params); // REQ
  };
  
  class ResponseMessage : public RequestHeader 
  {
  public:
    int resp_code;
    enum TResponseCode 
    {
      EResponseCode_MESSAGE_OK = 0, 
      EResponseCode_MESSAGE_NOT_WELL_FORMED    = 1,
      EResponseCode_INCOMPATIBLE_PROTOCOL_VERSION    = 2,
      EResponseCode_UNKNOW_METHOD    = 3
    };
    
    ResponseMessage() 
      : RequestHeader()
      , resp_code(EResponseCode_MESSAGE_OK)
    {
      //MESSAGE_NOT_WELL_FORMED = 1;
      //INCOMPATIBLE_PROTOCOL_VERSION = 2;
    }
    
    virtual ~ResponseMessage()
    {
    }
    
    int getRespCode() {
      return resp_code;
    }
    
    void setRespCode(int respCode) {
      this->resp_code = respCode;
    }

    std::string ToJson(); // RESP 
    std::string BuildJson(CVersion* ptl_ver, std::string msg_type, int msg_id, std::string method, int resp_code, rapidjson::Document& document, rapidjson::Value& params); // RESP
  };

  class CLtpRpcOverSpp : public ILtpRpcOverSpp, public ILtpConnectionListener 
  {
  private:
    //static JSONParser parser = new JSONParser();
    rapidjson::Document iJsonParser;

    // For notifying uppder layer connection status listener about current connection status.
    TConnectionStatus tconnectionStatus; // Dummy, fixed status.
    
    CLtpConnection* connection;
    std::string* iInstance; // Just for showing SP or HU in demo.
    ILtpRpcOverSppListener* connectionStatusListener;
    ILtpRpcOverSppListener* requestListener;
    ILtpRpcOverSppListener* responseListener;
    CVersion* protocolVersion;

    // Timer objects.
    CLtpTimer* iLtpTimerREQ;
    CLtpTimer* iLtpTimerRESP;
    static void fn_timer_expire_handler_REQ(int sig, siginfo_t* si, void* uc);
    static void fn_timer_expire_handler_RESP(int sig, siginfo_t* si, void* uc);
    
    void handleRequest(rapidjson::Document& jsonObject);
    void handleResponse(rapidjson::Document& jsonObject);
    void sendErrorReplyMessage(int respCode, int requestId);    

    void isJsonObjectException(const rapidjson::Value& json);
    int isJsonObjectErrReply(const rapidjson::Value& json, ResponseMessage::TResponseCode status);
    int isJsonArrayErrReply(const rapidjson::Value& json, ResponseMessage::TResponseCode status);
    
  public:
    CLtpRpcOverSpp(std::string& aInstance, TSocketType aSocketType);
    virtual ~CLtpRpcOverSpp();

    //! See base class
    void registerConnectionListener(ILtpRpcOverSppListener* listener);

    //! See base class
    void unregisterConnectionListener(ILtpRpcOverSppListener* listener);
    
    //! See base class
    void registerRequestListener(ILtpRpcOverSppListener* listener);

    //! See base class
    void registerResponseListener(ILtpRpcOverSppListener* listener);

    //! See base class
    //void sendRequest(int requestId, ILtpRpcOverSppListener* listener, std::string methodName, std::vector<TLocationType>& parameters);
    void sendRequest(int requestId, ILtpRpcOverSppListener* listener, std::string methodName, rapidjson::Document& document, rapidjson::Value& parameters);
    
    //! See base class
    void sendResponse(int requestId, std::string methodName, rapidjson::Document& document, rapidjson::Value& parameters);

    //! See base class
    void messageReceived(const std::string& message);

    //! See base class
    bool startRcpOverSppServerLoop();
    
  };
} //namespace NLtp


#endif  // _LTPRPCOVERSPP_H_