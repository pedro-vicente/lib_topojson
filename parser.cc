#include <iostream>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//main
/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "usage : ./parser <TOPOJSON file>" << std::endl;
    return 1;
  }

  std::cout << argv[1] << std::endl;
  return 0;
}
