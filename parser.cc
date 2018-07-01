#include <iostream>
#include <stdio.h>
#include "topojson.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//is_polygon
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_polygon(const Geometry_t &g)
{
  if (g.type.compare("Polygon") == 0 || g.type.compare("MultiPolygon") == 0)
  {
    return true;
  }
  return false;
}

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

  //no objects
  if (!topojson.m_topology.size())
  {
    return 0;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //must make the coordinates for the topology first; assume the first topology
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  size_t topology_index = 0;
  topojson.make_coordinates(topology_index);
  topology_object_t topology = topojson.m_topology.at(topology_index);

  size_t size_geom = topology.m_geom.size();
  for (size_t idx_geom = 0; idx_geom < size_geom; idx_geom++)
  {
    Geometry_t geometry = topology.m_geom.at(idx_geom);
    if (is_polygon(geometry))
    {
      size_t size_pol = geometry.m_polygon.size();
      for (size_t idx_pol = 0; idx_pol < size_pol; idx_pol++)
      {
        Polygon_topojson_t polygon = geometry.m_polygon.at(idx_pol);

        //render

        size_t size_points = geometry.m_polygon.at(idx_pol).m_y.size();
        for (size_t idx_crd = 0; idx_crd < size_points; idx_crd++)
        {
          double lat = geometry.m_polygon.at(idx_pol).m_y.at(idx_crd);
          double lon = geometry.m_polygon.at(idx_pol).m_x.at(idx_crd);
        }

      }
    }
  }

  return 0;
}
