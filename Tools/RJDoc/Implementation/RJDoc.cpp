#include <stdio.h>
#include <sstream>
#include <fstream>
#include "RJDoc.h"

RJDoc::RJDoc(const std::string& json)
{
  std::stringstream ss;
  std::ifstream ifs;
  ifs.open(json.c_str(), std::ios::binary);
  ss << ifs.rdbuf(); // 1
  ifs.close();
 
  if(doc_.Parse<0>(ss.str().c_str()).HasParseError()) // 2
    throw std::invalid_argument("json parse error"); // 3
}
  
bool RJDoc::checkStatus()
{
  rapidjson::Value& status = doc_["res"];
  if(!status.IsString())
    return false;
 
  return std::strcmp(doc_["res"].GetString(), "OK") == 0; // 1
}
 
int RJDoc::getTotal()
{
  return doc_["tot"].GetInt(); // 2
}
 
int RJDoc::getNumber()
{
  return doc_["nr"].GetInt();
}

const rapidjson::Value& RJDoc::getDs()
{
  rapidjson::Value& value = doc_["ds"];
  if(!value.IsArray())
    throw std::logic_error("bad ds"); // 3
 
  return value;
}
