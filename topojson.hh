#ifndef LIB_TOPO_JSON_HH
#define LIB_TOPO_JSON_HH

#ifdef WT_BUILDING
#include "Wt/WDllDefs.h"
#else
#ifndef WT_API
#define WT_API
#endif
#endif

#include <string>
#include <vector>
#include "gason.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//topojson_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

class WT_API topojson_t
{
public:
  topojson_t()
  {
  }
  int convert(const char* file_name);

private:
  int parse_root(JsonValue value);
  int parse_topology(JsonValue value);
  int parse_geometry_object(JsonValue value);

};

#endif




