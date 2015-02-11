//! --------------------------------------------------------------------------------
//! @file rfcomm-client.c
//!
//! @brief
//! Bluetooth Socket Client with serial port profile(RFCOMM) using bluez APIs.
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
#endif //__cplusplus

int search_service(char *bt_addr)
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

  int status, status2;
  bdaddr_t target;
  uuid_t svc_uuid;
  sdp_list_t *response_list, *search_list, *attrid_list;
  sdp_session_t *session = 0;
  uint32_t range = 0x0000ffff;
  uint8_t port = 0;
  
  char string[128] = {0};

  printf("------------ search_service start ------------\n");
  
  if(bt_addr == NULL)
  {
    fprintf(stderr, "Missing <bt_addr>\n");
    exit(EXIT_FAILURE);
  }
  str2ba( bt_addr, &target );

  // Connect to the SDP server running on the remote machine
#ifdef __cplusplus
  session = sdp_connect( &my_BDADDR_ANY, &target, 0 );
#else //__cplusplus
  session = sdp_connect( BDADDR_ANY, &target, 0 );
#endif //__cplusplus
  if (!session) 
  {
    printf("Failed to connect to the local SDP server. %s(%d)", strerror(errno), errno);
    exit(EXIT_FAILURE);
  }
  printf("sdp_connect OK!\n");

  sdp_uuid128_create( &svc_uuid, &svc_uuid_int );
  search_list = sdp_list_append( 0, &svc_uuid );
  attrid_list = sdp_list_append( 0, &range );

  // Get a list of service records that have UUID 0xabcd
  response_list = NULL;
  status = sdp_service_search_attr_req( session, search_list, SDP_ATTR_REQ_RANGE, attrid_list, &response_list);

  if (response_list == NULL)
  {
    printf("response_list not found!\n");
  }
    
  if (status == 0)
  {
    sdp_list_t *proto_list = NULL;
    sdp_list_t *r = response_list;

    // Go through each of the service records
    for (; r; r = r->next ) 
    {
      // Get record.
      sdp_record_t *rec = (sdp_record_t*) r->data;

      // Get service name.
      status2 = sdp_get_service_name( rec, string, sizeof(string) );
      if (status2 < 0)
      {
        perror("sdp_get_service_name error");
      }
      else
        printf("service_name:%s\n", string);
      
      // Get service description.
      status2 = sdp_get_service_desc( rec, string, sizeof(string) );
      if (status2 < 0)
      {
        perror("sdp_get_service_desc error");
      }
      else
        printf("service_desc:%s\n", string);
      
      // Get service provider name.
      status2 = sdp_get_provider_name( rec, string, sizeof(string) );
      if (status2 < 0)
      {
        perror("sdp_get_provider_name error");
      }
      else
        printf("provider_name:%s\n", string);
      
      // Get a list of the protocol sequences
      status2 = sdp_get_access_protos( rec, &proto_list );
      if (status2 < 0)
      {
        // Cleanup and exit.
        perror("sdp_get_access_protos error");
        sdp_list_free( response_list, 0 );
        sdp_list_free( search_list, 0 );
        sdp_list_free( attrid_list, 0 );
        sdp_close( session );
        exit(EXIT_FAILURE);
      }
      
      // Get RFCOMM port number
      port = sdp_get_proto_port( proto_list, RFCOMM_UUID ); 
      if (port <= 0)
      {
        // Cleanup and exit.
        perror("sdp_get_proto_port RFCOMM_UUID error");
        sdp_list_free( proto_list, 0 );
        sdp_list_free( response_list, 0 );
        sdp_list_free( search_list, 0 );
        sdp_list_free( attrid_list, 0 );
        sdp_close( session );
        exit(EXIT_FAILURE);
      } 
      else 
      {
        printf("found service running on RFCOMM port %d\n", port);
      }
      
      // Free resources.
      sdp_list_free( proto_list, 0 );
      sdp_record_free( rec );
    }
  }
  else
  {
    printf("The request completed unsuccessfully due to a timeout!\n");
  }
  
  // Cleanup and return RFCOMM port number.
  sdp_list_free( response_list, 0 );
  sdp_list_free( search_list, 0 );
  sdp_list_free( attrid_list, 0 );
  sdp_close( session );

  printf("------------ search_service end ------------\n");
  return port;
}

int main(int argc, char **argv)
{
  struct sockaddr_rc addr = { 0 };
  int s, status;
  char dest[18] = "1C:4B:D6:0F:F1:56"; //BuildServerTest-linstNB
  char message[] = " @@@ RFCOMM bluetooth socket @@@";

  if(argc < 2) 
  {
    fprintf(stderr, "usage: %s <bt_addr>\n", argv[0]);
    return 1;
  }
  strncpy(dest, argv[1], 18);
  printf("<bt_addr>:%s\n", argv[1]);
  
  // Allocate a socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if (s < 0)
  {
    perror("create socket error");
    exit(EXIT_FAILURE);
  } 

  // Set the connection parameters (who to connect to)
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_channel = search_service(argv[1]);
  str2ba( dest, &addr.rc_bdaddr );

  // Connect to server
  status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
  if (status < 0)
  {
    // Cleanup and exit.
    perror("connect error");
    close(s);
    exit(EXIT_FAILURE);
  } 

  // Send a message
  status = write(s, message, sizeof(message));
  if (status < 0)
  {
    // Cleanup and exit.
    perror("write error");
    close(s);
    exit(EXIT_FAILURE);
  } 
  else
    printf("Write (%d) bytes: %s\n", status, message);

#if 1 // Infinite loop for receiving fix string from server.
  int length=0;
  char iBuffer[1024];
  while (1)
  {
    length = recv(s, iBuffer, 1024-1, 0);
    if (length < 0)
    {
      perror("Socket read error");
      //close(fd_socket);
      exit(EXIT_FAILURE);
    }
    iBuffer[1024-1] = '\0'; /* null character manually added at buffer end */
    printf("Received (%d) bytes: %s\n", length, iBuffer);
  }
#endif //#if 1

  printf("Press any key to close connection...\n");
  getchar();
  
  // Close a socket
  status = close(s);
  if (status < 0)
  {
    perror("close error");
    exit(EXIT_FAILURE);
  } 

  return 0;
}

