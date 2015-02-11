#include <stdio.h>
#include "RJDoc.h"

int main(int argc, char **argv) {
  printf("Running main() from RJDoc.cpp\n");
  
  RJDoc reader("/home/ws/denso/daito/ProjComp/BTServices/Tools/RJDoc/Implementation/minimal.json");
  printf("reader.checkStatus():%d\n", reader.checkStatus());
  printf("reader.getTotal():%d\n", reader.getTotal());
  printf("reader.getNumber():%d\n", reader.getNumber());
  
  const rapidjson::Value& ds = reader.getDs();
  printf("ds.Size():%d\n", ds.Size());
  printf("ds[0U][\"a\"].GetString():%s\n", ds[0U]["a"].GetString());
  printf("ds[0U][\"b\"].GetString():%s\n", ds[0U]["b"].GetString());
  printf("ds[1U][\"a\"].GetString():%s\n", ds[1U]["a"].GetString());
  printf("ds[1U][\"b\"].GetString():%s\n", ds[1U]["b"].GetString());
  
  //printf("ds[3U][\"b\"].GetString():%s\n", ds[3U]["b"].GetString());
  
  return 0;
}