#include <iostream>
#include <stdio.h>
#include "topojson.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//main
/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "usage : ./parser <GEOJSON file>" << std::endl;
    return 1;
  }

  topojson_t topojson;
  if (topojson.convert(argv[1]) < 0)
  {
    return 1;
  }

  size_t size_geom = topojson.m_geom.size();
  for (size_t idx = 0; idx < size_geom; idx++)
  {
    std::string str = topojson.m_geom.at(idx).type;
    std::cout << str << "\n";
  }

  return 0;
}
