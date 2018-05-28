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
    scale[0] = scale[1] = translate[0] = translate[1] = 0;
  }
  int convert(const char* file_name);

private:
  int parse_root(JsonValue value);
  int parse_topology(JsonValue value);
  int parse_transform(JsonValue value);
  int parse_geometry_object(JsonValue value);
  std::vector<double> transform_point(const int position_quant[2]);
  double scale[2];
  double translate[2];

};

#endif




