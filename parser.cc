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

  ///////////////////////////////////////////////////////////////////////////////////////
  //render topojson
  ///////////////////////////////////////////////////////////////////////////////////////

  size_t idx_color = 0;
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

        ///////////////////////////////////////////////////////////////////////////////////////
        //render each polygon as a vector of vertices passed to Polygon
        ///////////////////////////////////////////////////////////////////////////////////////

        std::vector<double> lat;
        std::vector<double> lon;

        for (size_t idx_arc = 0; idx_arc < size_arcs; idx_arc++)
        {
          int index = polygon.arcs.at(idx_arc);
          int idx = index < 0 ? ~index : index;
          arc_t arc = topojson.m_arcs.at(idx);
          size_t size_vec_arcs = arc.vec.size();
          //if a topology is quantized, the positions of each arc in the topology which are quantized 
          //must be delta-encoded. The first position of the arc is a normal position [x1, y1]. 
          //The second position [x2, y2] is encoded as [dx2, dy2], where 
          //x2 = x1 + dx2 and 
          //y2 = y1 + dx2.
          //The third position [x3, y3] is encoded as [dx3, dy3], where 
          //x3 = x2 + dx3 = x1 + dx2 + dx3 and
          //y3 = y2 + dy3 = y1 + dy2 + dy3 and so on.
          int x0 = arc.vec.at(0).at(0);
          int y0 = arc.vec.at(0).at(1);
          std::vector<int> x;
          std::vector<int> y;
          x.push_back(x0);
          y.push_back(y0);
          for (size_t idx = 1; idx < size_vec_arcs; idx++)
          {
            int xn = x[idx - 1] + arc.vec.at(idx).at(0);
            int yn = y[idx - 1] + arc.vec.at(idx).at(1);
            x.push_back(xn);
            y.push_back(yn);
          }

          for (size_t idx = 0; idx < size_vec_arcs; idx++)
          {
            int pos_quant[2];
            pos_quant[0] = x[idx];
            pos_quant[1] = y[idx];
            std::vector<double> coord = topojson.transform_point(pos_quant);
            std::cout << coord[0] << " " << coord[1] << "\t";
            lat.push_back(coord[1]);
            lon.push_back(coord[0]);

          }//size_vec_arcs
        }//size_arcs

       //render lat, lon

      }//size_pol
    }//"Polygon"
  }//size_geom

  return 0;
}
