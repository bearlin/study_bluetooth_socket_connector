//! --------------------------------------------------------------------------------
//! @file simplescan.c
//!
//! @brief
//! Scan nearby Bluetooth devices using bluez HCI APIs.
//!
//! --------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <errno.h>

//#define SIMPLESCAN_TEST_JSON_PARSER_
#ifdef SIMPLESCAN_TEST_JSON_PARSER_
#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output

int test_json1() {
  const char json[] = "{ \"hello\" : \"world\" }";
  rapidjson::Document d;
  
  printf("\n===JSON Parser test_json1() START!===\n");
  
  d.Parse<0>(json);

  printf("Test JSON String: %s\n", json);
  printf("d[\"hello\"].GetString() = %s\n", d["hello"].GetString());
  printf("\n===JSON Parser test_json1() END!===\n");
  return 0;
}

// Hello World example
// This example shows basic usage of DOM-style API.
int test_json2() {
  using namespace rapidjson;
	////////////////////////////////////////////////////////////////////////////
	// 1. Parse a JSON text string to a document.

	const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
  printf("\n===JSON Parser test_json2() START!===\n");
	printf("Original JSON:\n %s\n", json);

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

#if 0
	// "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
	if (document.Parse<0>(json).HasParseError())
		return 1;
#else
	// In-situ parsing, decode strings directly in the source string. Source must be string.
	char buffer[sizeof(json)];
	memcpy(buffer, json, sizeof(json));
	if (document.ParseInsitu<0>(buffer).HasParseError())
		return 1;
#endif

	printf("\nParsing to document succeeded.\n");

	////////////////////////////////////////////////////////////////////////////
	// 2. Access values in document. 

	printf("\nAccess values in document:\n");
	assert(document.IsObject());	// Document is a JSON value represents the root of DOM. Root can be either an object or array.

	assert(document.HasMember("hello"));
	assert(document["hello"].IsString());
	printf("hello = %s\n", document["hello"].GetString());

	assert(document["t"].IsBool());		// JSON true/false are bool. Can also uses more specific function IsTrue().
	printf("t = %s\n", document["t"].GetBool() ? "true" : "false");

	assert(document["f"].IsBool());
	printf("f = %s\n", document["f"].GetBool() ? "true" : "false");

	printf("n = %s\n", document["n"].IsNull() ? "null" : "?");

	assert(document["i"].IsNumber());	// Number is a JSON type, but C++ needs more specific type.
	assert(document["i"].IsInt());		// In this case, IsUint()/IsInt64()/IsUInt64() also return true.
	printf("i = %d\n", document["i"].GetInt());	// Alternative (int)document["i"]

	assert(document["pi"].IsNumber());
	assert(document["pi"].IsDouble());
	printf("pi = %g\n", document["pi"].GetDouble());

	{
		const Value& a = document["a"];	// Using a reference for consecutive access is handy and faster.
		assert(a.IsArray());
		for (SizeType i = 0; i < a.Size(); i++)	// rapidjson uses SizeType instead of size_t.
			printf("a[%d] = %d\n", i, a[i].GetInt());
		
		// Note:
		//int x = a[0].GetInt();					// Error: operator[ is ambiguous, as 0 also mean a null pointer of const char* type.
		int y = a[SizeType(0)].GetInt();			// Cast to SizeType will work.
		int z = a[0u].GetInt();						// This works too.
		(void)y;
		(void)z;
	}

	// 3. Modify values in document.

	// Change i to a bigger number
	{
		uint64_t f20 = 1;	// compute factorial of 20
		for (uint64_t j = 1; j <= 20; j++)
			f20 *= j;
		document["i"] = f20;	// Alternate form: document["i"].SetUint64(f20)
		assert(!document["i"].IsInt());	// No longer can be cast as int or uint.
	}

	// Adding values to array.
	{
		Value& a = document["a"];	// This time we uses non-const reference.
		Document::AllocatorType& allocator = document.GetAllocator();
		for (int i = 5; i <= 10; i++)
			a.PushBack(i, allocator);	// May look a bit strange, allocator is needed for potentially realloc. We normally uses the document's.

		// Fluent API
		a.PushBack("Lua", allocator).PushBack("Mio", allocator);
	}

	// Making string values.

	// This version of SetString() just store the pointer to the string.
	// So it is for literal and string that exists within value's life-cycle.
	{
		document["hello"] = "rapidjson";	// This will invoke strlen()
		// Faster version:
		// document["hello"].SetString("rapidjson", 9);
	}

	// This version of SetString() needs an allocator, which means it will allocate a new buffer and copy the the string into the buffer.
	Value author;
	{
		char buffer[10];
		int len = sprintf(buffer, "%s %s", "Milo", "Yip");	// synthetic example of dynamically created string.

		author.SetString(buffer, len, document.GetAllocator());
		// Shorter but slower version:
		// document["hello"].SetString(buffer, document.GetAllocator());

		// Constructor version: 
		// Value author(buffer, len, document.GetAllocator());
		// Value author(buffer, document.GetAllocator());
		memset(buffer, 0, sizeof(buffer)); // For demonstration purpose.
	}
	// Variable 'buffer' is unusable now but 'author' has already made a copy.
	document.AddMember("author", author, document.GetAllocator());

	assert(author.IsNull());		// Move semantic for assignment. After this variable is assigned as a member, the variable becomes null.

	////////////////////////////////////////////////////////////////////////////
	// 4. Stringify JSON

	printf("\nModified JSON with reformatting:\n");
	FileStream f(stdout);
	PrettyWriter<FileStream> writer(f);
	document.Accept(writer);	// Accept() traverses the DOM and generates Handler events.
  printf("\n===JSON Parser test_json2() END!===\n");
	return 0;
}
#endif //SIMPLESCAN_TEST_JSON_PARSER_

int main(int argc, char **argv)
{
  inquiry_info *devices = NULL;
  int max_rsp, num_rsp, name_rsp;
  int adapter_id, sock, len, flags;
  int i;
  char addr[19] = { 0 };
  char name[248] = { 0 };

#ifdef SIMPLESCAN_TEST_JSON_PARSER_
  // Test if JSON parser works.
  if (test_json1())
  {
    fprintf(stderr, "JSON parser doesn't work\n");
    exit(EXIT_FAILURE);
  }
  
  if (test_json2())
  {
    fprintf(stderr, "JSON parser doesn't work\n");
    exit(EXIT_FAILURE);
  }
#endif //SIMPLESCAN_TEST_JSON_PARSER_

  // A program must specify which adapter to use when allocating system resources.
  adapter_id = hci_get_route(NULL);
  if (adapter_id < 0) 
  {
    fprintf(stderr, "hci_get_route error code %d: %s\n", errno, strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Opens a Bluetooth socket with the specified resource number.
  sock = hci_open_dev( adapter_id );
  if (adapter_id < 0 || sock < 0) 
  {
    fprintf(stderr, "hci_open_dev error code %d: %s\n", errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("local adapter adapter_id:%d sock:%d\n", adapter_id, sock);

  len  = 8;
  max_rsp = 255;
  flags = IREQ_CACHE_FLUSH; //the cache of previously detected devices is flushed before performing the current inquiry
  devices = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

  // Bluetooth device discovery
  num_rsp = hci_inquiry(adapter_id, len, max_rsp, NULL, &devices, flags);
  if( num_rsp < 0 ) 
  {
    // Cleanup and exit.
    fprintf(stderr, "hci_inquiry error code %d: %s\n", errno, strerror(errno));
    free( devices );
    close( sock );
    exit(EXIT_FAILURE);
  }
  printf("num_rsp:%d\n", num_rsp);

  // Get the user-friendly names associated with those addresses, and presents them to the user.
  for (i = 0; i < num_rsp; i++) 
  {
    ba2str(&(devices+i)->bdaddr, addr);
    memset(name, 0, sizeof(name));

    name_rsp = hci_read_remote_name(sock, &(devices+i)->bdaddr, sizeof(name), name, 0);
    if (0 != name_rsp) 
    {
      strcpy(name, "[unknown]");
      fprintf(stderr, "hci_read_remote_name error code %d: %s\n", errno, strerror(errno));
    }
    printf("%s  %s\n", addr, name);
  }

  // Cleanup and return.
  free( devices );
  close( sock );
  return 0;
}

