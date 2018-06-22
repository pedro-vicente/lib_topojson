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

int is_topojson(const char* file_name);

//A topology must have an �arcs� member whose value is an array of arrays of positions. 
//Each arc must be an array of two or more positions.
class WT_API arc_t
{
public:
  arc_t() {}
  std::vector<std::vector<double>> vec;
};

//A geometry is a TopoJSON object where the type member�s value is one of the following strings: 
//�Point�, �MultiPoint�, �LineString�, �MultiLineString�, �Polygon�, �MultiPolygon�, or �GeometryCollection�.
class WT_API Point
{
public:
  Point() {}
  std::vector<double> coordinates;
};

class WT_API LineString
{
public:
  LineString() {}
  std::vector<int> arcs; //indices into arc_t array
};

//For type �Polygon�, the �arcs� member must be an array of LinearRing arc indexes. 
class WT_API Polygon_topojson_t
{
public:
  Polygon_topojson_t() {}
  //indices into arc_t array
  std::vector<int> arcs;
  //converted x and y coordinates
  std::vector<double> m_x;
  std::vector<double> m_y;
};

class WT_API Geometry_t
{
public:
  Geometry_t() {}
  //A TopoJSON object must have a member with the name �type�. 
  //This member�s value is a string that determines the type of the TopoJSON object.
  std::string type;
  std::vector<Polygon_topojson_t> m_polygon;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//topojson_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

class WT_API topojson_t
{
public:
  topojson_t() :
    idx_geom(0)
  {
    scale[0] = scale[1] = translate[0] = translate[1] = 0;
  }
  int convert(const char* file_name);
  std::vector<Geometry_t> m_geom;
  std::vector<arc_t> m_arcs;
  std::vector<double> transform_point(const int position_quant[2]);
  std::vector<double> get_first();

private:
  int parse_root(JsonValue value);
  int parse_topology(JsonValue value);
  int parse_transform(JsonValue value);
  int parse_geometry_object(JsonValue value);
  int parse_arcs(JsonValue value);
  void make_coordinates();
  double scale[2];
  double translate[2];
  size_t idx_geom; //helper
};

#endif




