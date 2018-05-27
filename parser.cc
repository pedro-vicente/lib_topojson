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

  return 0;
}
