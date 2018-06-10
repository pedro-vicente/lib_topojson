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
  for (size_t idx_geom = 0; idx_geom < size_geom; idx_geom++)
  {
    Geometry_t geometry = topojson.m_geom.at(idx_geom);
    if (geometry.type.compare("Polygon") == 0)
    {
      std::cout << "Polygon" << std::endl;
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
            std::vector<double> coord = arc.vec.at(idx_vec);
            std::cout << coord[0] << " " << coord[1] << "\t";
          }//size_vec_arcs
        }//size_arcs
      }//size_pol
    }//"Polygon"
  }//size_geom

  return 0;
}
