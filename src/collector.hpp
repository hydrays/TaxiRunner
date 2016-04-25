#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

#include <string>
#include <stdio.h>
#include <vector>
#include "utils.hpp"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "cpl_string.h"
#include "ogr_srs_api.h"
#include "ogr_spatialref.h"

class Collector
{
 public:
  int Ndim, Mdim;
  double * WeightMatrix1;
  double * WeightMatrix2;
  std::string mapfile = "Map.osrm";
  std::string ODfileName = "od_sample.txt";
  std::vector<Trajct> trajctList;
  Collector();
  int init();
  int print_trajctList();
  int make_weight_matrix();
  int output_weight_matrix();
};

#endif //COLLECTOR_HPP
