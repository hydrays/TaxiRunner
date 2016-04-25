1;3409;0c#include "runner.hpp"
#include "collector.hpp"
#include "convector.hpp"

int main()
{
  Collector collector;
  collector.init();
  Runner runner;
  runner.init(collector);
  Convector convector;
  convector.init(collector);

  runner.run(collector);

  //collector.print_trajctList();
  collector.make_weight_matrix();
  collector.output_weight_matrix();

  convector.make_road_network(collector);
  convector.output_road_network();
  return 0;
}
