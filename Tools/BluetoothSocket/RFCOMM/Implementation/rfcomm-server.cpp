//! --------------------------------------------------------------------------------
//! @file rfcomm-server.c
//!
//! @brief
//! Bluetooth Socket Server with serial port profile(RFCOMM) using bluez APIs.
//!
//! --------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <errno.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

#define BT_SOCKET_MIN_PORT (1)
#define BT_SOCKET_MAX_PORT (30)
//#define BT_SOCKET_FIXED_PORT (11)

#ifdef __cplusplus
  // *** BDADDR_ANY Constructs a temporary object and uses its address as below. This isn't allowed in C++.   ***
  // *** bluetooth.h:                                                                                         *** 
  // *** #define BDADDR_ANY   (&(bdaddr_t) {{0, 0, 0, 0, 0, 0}})                                              *** 
  // *** #define BDADDR_ALL   (&(bdaddr_t) {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}})                            *** 
  // *** #define BDADDR_LOCAL (&(bdaddr_t) {{0, 0, 0, 0xff, 0xff, 0xff}})                                     *** 
  // *** We don't want to modify bluetooth.h and don't want to add C++ compiler flag [-fpermissive] to fix it,***
  // *** so just replace BDADDR_ANY with my_BDADDR_ANY global variable.                                       ***
  // *** Ref: http://stackoverflow.com/questions/9751710/c-c-warning-address-of-temporary-with-bdaddr-any-bluetooth-library
  // ***      http://stackoverflow.com/questions/8843818/what-does-the-fpermissive-flag-do
  bdaddr_t my_BDADDR_ANY = {{0}};
  bdaddr_t my_BDADDR_LOCAL = {{0, 0, 0, 0xff, 0xff, 0xff}};
#endif //__cplusplus

sdp_session_t* register_service(int port)
{
  /*
  //fa87c0d0-afac-11de-8a39-0800200c9a66    // Linux <-> Linux.
  uint8_t svc_uuid_int[] = 
  { 
  0xfa, 0x87, 0xc0, 0xd0, 
  0xaf, 0xac, 0x11, 0xde, 
  0x8a, 0x39, 0x08, 0x00, 
  0x20, 0x0c, 0x9a, 0x66
  };
  */
  
  //00001101-0000-1000-8000-00805F9B34FB    // Linux <-> Linux/Android, use this one for Android phone.
  uint8_t svc_uuid_int[] = 
  { 
  0x00, 0x00, 0x11, 0x01, 
  0x00, 0x00, 0x10, 0x00, 
  0x80, 0x00, 0x00, 0x80, 
  0x5F, 0x9B, 0x34, 0xFB
  };
  
  /*
  //a3dd6d39-518c-4032-b226-f61d91052ca2    // For testing.
  uint8_t svc_uuid_int[] = 
  { 
  0xa3, 0xdd, 0x6d, 0x39, 
  0x51, 0x8c, 0x40, 0x32, 
  0xb2, 0x26, 0xf6, 0x1d, 
  0x91, 0x05, 0x2c, 0xa2 
  };
  */

  /*
  //00000000-0000-0000-0000-00000000abcd    // Linux <-> Linux testing.
  uint8_t svc_uuid_int[] = 
  { 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xab, 0xcd 
  };
  */

  uint8_t rfcomm_channel = port;//11;

  const char *service_name = "BearLin_BTService";
  const char *svc_dsc = "Mobile_Connector";
  const char *service_prov = "TT";

  uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid, svc_class_uuid;

  sdp_list_t *l2cap_list = 0, 
             *rfcomm_list = 0,
             *root_list = 0,
             *proto_list = 0, 
             *access_proto_list = 0,
             *svc_class_list = 0,
             *profile_list = 0;
  sdp_data_t *channel = 0;
  sdp_profile_desc_t profile;
  sdp_record_t record = { 0 };
  sdp_session_t *session = 0;

  int status = 0;
  printf("------------ register_service start ------------\n");
  // Set the general service ID
  sdp_uuid128_create( &svc_uuid, &svc_uuid_int );
  sdp_set_service_id( &record, svc_uuid );
  printf("sdp_set_service_id:0x%X%X\n", svc_uuid_int[14], svc_uuid_int[15]);

  // Set the service class
  sdp_uuid16_create(&svc_class_uuid, SERIAL_PORT_SVCLASS_ID);
  svc_class_list = sdp_list_append(0, &svc_class_uuid); //appen to linked list.
  sdp_set_service_classes(&record, svc_class_list);
  printf("sdp_set_service_classes:0x%X\n", SERIAL_PORT_SVCLASS_ID);

  // Set the Bluetooth profile information
  sdp_uuid16_create(&profile.uuid, SERIAL_PORT_PROFILE_ID);
  profile.version = 0x0100;
  profile_list = sdp_list_append(0, &profile);
  sdp_set_profile_descs(&record, profile_list);
  printf("sdp_set_profile_descs:0x%X\n", SERIAL_PORT_PROFILE_ID);

  // Make the service record publicly browsable
  sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
  root_list = sdp_list_append(0, &root_uuid);
  sdp_set_browse_groups( &record, root_list );
  printf("sdp_set_browse_groups:0x%X\n", PUBLIC_BROWSE_GROUP);

  // Set l2cap information
  sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
  l2cap_list = sdp_list_append( 0, &l2cap_uuid );
  proto_list = sdp_list_append( 0, l2cap_list );

  // Register the RFCOMM channel for RFCOMM sockets
  sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
  channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
  rfcomm_list = sdp_list_append( 0, &rfcomm_uuid );
  sdp_list_append( rfcomm_list, channel );
  sdp_list_append( proto_list, rfcomm_list );

  access_proto_list = sdp_list_append( 0, proto_list );
  sdp_set_access_protos( &record, access_proto_list );
  printf("sdp_set_access_protos:\n");
  printf("\tL2CAP_UUID:0x%X\n", L2CAP_UUID);
  printf("\tRFCOMM_UUID:0x%X\n", RFCOMM_UUID);
  printf("\trfcomm_channel:%d\n", rfcomm_channel);

  // Set the name, provider, and description
  sdp_set_info_attr(&record, service_name, service_prov, svc_dsc);
  printf("sdp_set_info_attr:\n");
  printf("\tservice_name:%s\n", service_name);
  printf("\tservice_prov:%s\n", service_prov);
  printf("\tsvc_dsc:%s\n", svc_dsc);

  // Connect to the local SDP server
#ifdef __cplusplus
  session = sdp_connect(&my_BDADDR_ANY, &my_BDADDR_LOCAL, SDP_RETRY_IF_BUSY);
#else //__cplusplus
  session = sdp_connect(BDADDR_ANY, BDADDR_LOCAL, SDP_RETRY_IF_BUSY);
#endif //__cplusplus
  if (!session) 
  {
    // Cleanup and exit.
		printf("Failed to connect to the local SDP server. %s(%d)", strerror(errno), errno);
    sdp_data_free( channel );
    sdp_list_free( l2cap_list, 0 );
    sdp_list_free( rfcomm_list, 0 );
    sdp_list_free( root_list, 0 );
    sdp_list_free( access_proto_list, 0 );
    sdp_list_free( svc_class_list, 0 );
    sdp_list_free( profile_list, 0 );
		exit(EXIT_FAILURE);
	}
  printf("sdp_connect OK!\n");
  
  // Register the service record
  status = sdp_record_register(session, &record, 0);
  if (status < 0)
  {
    // Cleanup and exit.
    perror("sdp_record_register error");
    sdp_data_free( channel );
    sdp_list_free( l2cap_list, 0 );
    sdp_list_free( rfcomm_list, 0 );
    sdp_list_free( root_list, 0 );
    sdp_list_free( access_proto_list, 0 );
    sdp_list_free( svc_class_list, 0 );
    sdp_list_free( profile_list, 0 );
    exit(EXIT_FAILURE);
  } 
  printf("sdp_record_register OK!\n");

  // Cleanup and return service session.
  sdp_data_free( channel );
  sdp_list_free( l2cap_list, 0 );
  sdp_list_free( rfcomm_list, 0 );
  sdp_list_free( root_list, 0 );
  sdp_list_free( access_proto_list, 0 );
  sdp_list_free( svc_class_list, 0 );
  sdp_list_free( profile_list, 0 );
  printf("------------ register_service done ------------\n");
  return session;
}

int main(int argc, char **argv)
{
  struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
  char buf[1024] = { 0 };
  int s, client, bytes_read, status;
  socklen_t opt = sizeof(rem_addr);
  int port = 0;

  // Allocate socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if(s < 0)
  {
    perror("create socket error");
    exit(EXIT_FAILURE);
  } 
  printf("socket create ok!\n");

  // Bind socket to port 1 of the first available bluetooth adapter
  loc_addr.rc_family = AF_BLUETOOTH;
#ifdef __cplusplus
  loc_addr.rc_bdaddr = my_BDADDR_ANY;
#else //__cplusplus
  loc_addr.rc_bdaddr = *BDADDR_ANY;
#endif //__cplusplus
#ifndef BT_SOCKET_FIXED_PORT
  for (port = BT_SOCKET_MIN_PORT; port <= BT_SOCKET_MAX_PORT; port++)
  {
    loc_addr.rc_channel = port;
    status = bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    if(status < 0)
    {
      perror("bind error");
    }
    if(status == 0)
      break;
  }
  if (port <= BT_SOCKET_MAX_PORT)
  {
    printf("Dynamic bind to port#%d!\n", loc_addr.rc_channel);
  }
  else
  {
    printf("Dynamic bind port failed!\n");
    exit(EXIT_FAILURE);
  }
#else //BT_SOCKET_FIXED_PORT
  port = BT_SOCKET_FIXED_PORT;
  loc_addr.rc_channel = port;
  status = bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
  if(status < 0)
  {
    perror("bind error");
  }
  printf("bind to fixed port#%d!\n", loc_addr.rc_channel);
#endif //BT_SOCKET_FIXED_PORT

  // Register the "TT_BTService" service.
  sdp_session_t* session = register_service(port);
  printf("Advertising new services START ...\n");

  // Put socket into listening mode
  status = listen(s, 1);
  if(status < 0)
  {
    // Cleanup and exit.
    perror("socket listen error");
    close(s);
    sdp_close(session);
    exit(EXIT_FAILURE);
  } 
  printf("socket listening...\n");

  // Accept one connection
  client = accept(s, (struct sockaddr *)&rem_addr, &opt);
  if(client < 0)
  {
    // Cleanup and exit.
    perror("socket client accept error");
    close(s);
    sdp_close(session);
    exit(EXIT_FAILURE);
  } 
  ba2str( &rem_addr.rc_bdaddr, buf );
  fprintf(stderr, "accepted connection from %s\n", buf);
  memset(buf, 0, sizeof(buf));

  // Read data from the client
  bytes_read = read(client, buf, sizeof(buf));
  if (bytes_read < 0)
  {
    // Cleanup and exit.
    perror("socket read error");
    close(client);
    close(s);
    sdp_close(session);
    exit(EXIT_FAILURE);
  } 
  if( bytes_read > 0 ) 
  {
    printf("Received (%d) bytes: %s\n", bytes_read, buf);
  }
  
#if 1 // Infinite loop for sending fix string to client.
  int length=0;
  while (1)
  {
    printf("sleep(2)...\n");
      sleep(2);
    
    //char aBuffer[] = "{\"ptl_ver\":{\"major\":1,\"minor\":0},\"msg_type\":\"RESP\",\"msg_id\":123,\"method\":\"getUserData\",\"resp_code\":0,\"parameters\":[{\"DESTINATION\":{\"timestamp\":\"2014-03-01T20:10:23.000+0800\",\"coordinates\":{\"latitude\":51514271,\"longitude\":-139346}},\"LAST_KNOWN_CAR_POSITION\":{\"timestamp\":\"2014-03-01T20:10:23.000+0800\",\"coordinates\":{\"latitude\":51514271,\"longitude\":-139346}}}]}";
    const char* aBuffer = "aaaaaa";
    
    length = send(client, aBuffer, strlen(aBuffer), 0);
    if (length < 0)
    {
      perror("Socket write error");
      //close(client);
      return 0;//exit(EXIT_FAILURE);
    } 
    printf("Write (%d) bytes: %s\n", length, aBuffer);
  }
#endif //#if 1

  // Close connections.
  status = close(client);
  if (status < 0)
  {
    perror("close client socket error");
    exit(EXIT_FAILURE);
  }
  
  status = close(s);
  if (status < 0)
  {
    perror("close socket error");
    exit(EXIT_FAILURE);
  } 
  
  // The service record will stay registered and advertised until the program exits or 
  // closes the connection to the local SDP server by calling sdp close.
  sdp_close(session);
  printf("Advertising new services STOP!\n");
  
  return 0;
}

