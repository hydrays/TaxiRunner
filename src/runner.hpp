#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <string>
#include <stdio.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <vector>
#include <utility>
#include <iostream>
#include <exception>

#include "osrm/route_parameters.hpp"
#include "osrm/table_parameters.hpp"
#include "osrm/nearest_parameters.hpp"
#include "osrm/trip_parameters.hpp"
#include "osrm/match_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/status.hpp"
#include "osrm/osrm.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "utils.hpp"
#include "collector.hpp"

using namespace osrm;

class Runner
{
 public:
  std::string mapfile;
  std::string ODfileName;

  Runner();
  int init(const Collector &);
  int run(Collector &);
};

#endif //RUNNER_HPP
