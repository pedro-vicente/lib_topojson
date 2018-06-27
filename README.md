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

# C++ class

```c++
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
  int parse_arcs(JsonValue value);
  std::vector<double> transform_point(const int position_quant[2]);
  double scale[2];
  double translate[2];
  std::vector<arc_t> m_vec_arcs;
};

```

# C++ get coordinates

```c++
size_t size_geom = topojson.m_geom.size();
  for (size_t idx_geom = 0; idx_geom < size_geom; idx_geom++)
  {
    Geometry_t geometry = topojson.m_geom.at(idx_geom);
    if (geometry.type.compare("Polygon") == 0)
    {
      size_t size_pol = geometry.m_polygon.size();
      for (size_t idx_pol = 0; idx_pol < size_pol; idx_pol++)
      {
        Polygon polygon = geometry.m_polygon.at(idx_pol);
        size_t size_arcs = polygon.arcs.size();
        for (size_t idx_arc = 0; idx_arc < size_arcs; idx_arc++)
        {
          int index = polygon.arcs.at(idx_arc);
          arc_t arc = topojson.m_arcs.at(index);
          size_t size_vec_arcs = arc.vec.size();
          for (size_t idx_vec = 0; idx_vec < size_vec_arcs; idx_vec++)
          {
            std::vector<double> coord_quant = arc.vec.at(idx_vec);
            int position_quantized[2] = { (int)coord_quant[0], (int)coord_quant[1] };
            std::vector<double> coord = topojson.transform_point(position_quantized);
            std::cout << coord[0] << " " << coord[1] << "\t";

          }//size_vec_arcs
        }//size_arcs
      }//size_pol
    }//"Polygon"
  }//size_geom
```

# rendered file example.quantized.topojson

Render by Wt Leaflet branch

https://github.com/pedro-vicente/wt

![image](https://user-images.githubusercontent.com/6119070/41950762-ae7f33aa-7995-11e8-85a7-661c3630d5c8.png)


