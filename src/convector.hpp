#ifndef CONVECTOR_HPP
#define CONVECTOR_HPP

#include "collector.hpp"
#include <string>
#include <stdio.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <vector>

struct Node
{
  int id;
  std::string name;
  double lat;
  double lon;
};

struct Way
{
  int id;
  std::string name;
  double speed;
  std::vector<int> nd;
};

struct RoadNetwork
{
  std::vector<Node> node;
  std::vector<Way> way;
};

class Convector
{
 public:
  RoadNetwork road_network;
  std::string mapfile; 
  double gridinfo[6];
  Convector();
  int init(const Collector &);
  int make_road_network(const Collector &);
  int output_road_network();
  int xy_from_cell(long int, double *);
};

#endif //CONVECTOR_HPP
