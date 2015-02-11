#ifndef RJDOC_H_
#define RJDOC_H_

// I need to manage the case of missing field. 
// For instance here I am trying to access a field of a non-existing element in the array. 
// The default RapidJson behavior is letting an assertion fail. 
// I want instead a standard exception to be thrown, 
// so that I can catch it and perform some sort of fall back operation.
// we can see RAPIDJSON_ASSERT in rapidjson.h, 
// So what I have done is defining the symbol RAPIDJSON_ASSERT before any RapidJson include in my code.
#include <stdexcept>
#define RAPIDJSON_ASSERT(x) if(!(x)) throw std::logic_error("rapidjson exception");
#include <rapidjson/document.h>

class RJDoc
{
private:
  rapidjson::Document doc_;

public:
  RJDoc(const std::string& json);
  bool checkStatus();
  int getTotal();
  int getNumber();
  const rapidjson::Value& getDs();
};

#endif  // RJDOC_H_