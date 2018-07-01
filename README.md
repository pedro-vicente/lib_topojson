# lib_topojson
a C++ topoJSON parser. TopoJSON is a format for encoding a variety of geographic data structures.

[![Build status](https://ci.appveyor.com/api/projects/status/pluqviybu1v2xh7a?svg=true)](https://ci.appveyor.com/project/pedro-vicente/lib-topojson)


# topoJSON specification

https://github.com/topojson/topojson-specification

# JSON

http://json.org/

JSON parsing is done with gason

https://github.com/vivkin/gason


# usage

```
./parser example.quantized.topojson
```

# sample file example.quantized.topojson

```json
{
  "type": "Topology",
  "transform": {
    "scale": [0.0005000500050005, 0.00010001000100010001],
    "translate": [100, 0]
  },
  "objects": {
    "example": {
      "type": "GeometryCollection",
      "geometries": [
        {
          "type": "Point",
          "properties": {
            "prop0": "value0"
          },
          "coordinates": [4000, 5000]
        },
        {
          "type": "LineString",
          "properties": {
            "prop0": "value0",
            "prop1": 0
          },
          "arcs": [0]
        },
        {
          "type": "Polygon",
          "properties": {
            "prop0": "value0",
            "prop1": {
              "this": "that"
            }
          },
          "arcs": [[1]]
        }
      ]
    }
  },
  "arcs": [
    [[4000, 0], [1999, 9999], [2000, -9999], [2000, 9999]],
    [[0, 0], [0, 9999], [2000, 0], [0, -9999], [-2000, 0]]
  ]
}
```

# output

```
type
transform
        object name:    scale
        scale:  0.00050005,0.00010001
        object name:    translate
        translate:      100,0
objects
        object name:    example
        geometry type:  GeometryCollection
                geometry type:  Point
                geometry type:  LineString
                geometry type:  Polygon
arcs
arcs size: 2
        arc size: 4
        arc size: 5
```

# C++ data structures

```c++
//A topology must have an “arcs” member whose value is an array of arrays of positions. 
//Each arc must be an array of two or more positions.
class WT_API arc_t
{
public:
  arc_t() {}
  std::vector<std::vector<double>> vec;
};

//A geometry is a TopoJSON object where the type member’s value is one of the following strings: 
//“Point”, “MultiPoint”, “LineString”, “MultiLineString”, “Polygon”, “MultiPolygon”, or “GeometryCollection”.
class WT_API Point_topojson_t
{
public:
  Point_topojson_t() {}
  std::vector<double> coordinates;
};

class WT_API LineString_topojson_t
{
public:
  LineString_topojson_t() {}
  std::vector<int> arcs; //indices into arc_t array
};

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
  //A TopoJSON object must have a member with the name “type”. 
  //This member’s value is a string that determines the type of the TopoJSON object.
  std::string type;
  std::vector<Polygon_topojson_t> m_polygon;
};

```

# C++ class

```c++
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

```

# C++ get coordinates

```c++
void topojson_t::make_coordinates()
{
  size_t size_geom = m_geom.size();
  for (idx_geom = 0; idx_geom < size_geom; idx_geom++)
  {
    Geometry_t geometry = m_geom.at(idx_geom);
    if (geometry.type.compare("Polygon") == 0)
    {
      size_t size_pol = geometry.m_polygon.size();
      for (size_t idx_pol = 0; idx_pol < size_pol; idx_pol++)
      {
        Polygon_topojson_t polygon = geometry.m_polygon.at(idx_pol);
        size_t size_arcs = polygon.arcs.size();

        for (size_t idx_arc = 0; idx_arc < size_arcs; idx_arc++)
        {
          int index = polygon.arcs.at(idx_arc);
          int index_q = index < 0 ? ~index : index;
          arc_t arc = m_arcs.at(index_q);
          size_t size_vec_arcs = arc.vec.size();
          //if a topology is quantized, the positions of each arc in the topology which are quantized 
          //must be delta-encoded. The first position of the arc is a normal position [x1, y1]. 
          //The second position [x2, y2] is encoded as [dx2, dy2], where 
          //x2 = x1 + dx2 and 
          //y2 = y1 + dx2.
          //The third position [x3, y3] is encoded as [dx3, dy3], where 
          //x3 = x2 + dx3 = x1 + dx2 + dx3 and
          //y3 = y2 + dy3 = y1 + dy2 + dy3 and so on.
          double x = 0;
          double y = 0;
          //temporary positions
          std::vector<double> xp;
          std::vector<double> yp;
          for (size_t idx = 0; idx < size_vec_arcs; idx++)
          {
            double position[2];
            position[0] = arc.vec.at(idx).at(0);
            position[1] = arc.vec.at(idx).at(1);
            position[0] = (x += position[0]) * scale[0] + translate[0];
            position[1] = (y += position[1]) * scale[1] + translate[1];
            xp.push_back(position[0]);
            yp.push_back(position[1]);
          }//size_vec_arcs

          //@mbostock
          //TopoJSON allows you to reference a reversed arc in a geometry so that when geometries share an arc, 
          //but some geometries need the arc in the opposite direction, the geometries can reference the same arc.
          //This occurs very commonly when you have neighboring geometries.For example, California and Nevada 
          //share a border, but given that both would typically have the same winding order, 
          //the shared border must be reversed between the two polygons if you want to share the arc.
          //A reversed arc means that rather than the arc’s points going p_0, p_1, … p_n, 
          //the points go p_n, p_{ n - 1 }, … p_0.

           //reverse the subsequences of points represented by the negative arc indexes
          if (index < 0)
          {
            for (size_t idx = 0; idx < size_vec_arcs; idx++)
            {
              size_t jdx = size_vec_arcs - idx - 1;
              m_geom.at(idx_geom).m_polygon.at(idx_pol).m_x.push_back(xp[jdx]);
              m_geom.at(idx_geom).m_polygon.at(idx_pol).m_y.push_back(yp[jdx]);
            }
          }
          //do not reverse
          else
          {
            for (size_t idx = 0; idx < size_vec_arcs; idx++)
            {
              size_t jdx = idx;
              m_geom.at(idx_geom).m_polygon.at(idx_pol).m_x.push_back(xp[jdx]);
              m_geom.at(idx_geom).m_polygon.at(idx_pol).m_y.push_back(yp[jdx]);
            }
          }
          assert(xp.size() == size_vec_arcs);

        }//size_arcs
      }//size_pol
    }//"Polygon"
  }//size_geom
}
```

# Rendering

Render by WxWidgets GUI library

https://github.com/pedro-vicente/render_geojson

![image](https://user-images.githubusercontent.com/6119070/41935976-a4eb5530-7959-11e8-9f51-5a7e0be79197.png)

Render by Wt web framewwork, Leaflet branch

https://github.com/pedro-vicente/wt

![image](https://user-images.githubusercontent.com/6119070/41950762-ae7f33aa-7995-11e8-85a7-661c3630d5c8.png)


