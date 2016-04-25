#include "runner.hpp"

Runner::Runner()
{
}

int Runner::init(const Collector &collector)
{
  mapfile = collector.mapfile;
  ODfileName = collector.ODfileName;
  return 0;
}

int Runner::run(Collector &collector)
{
  EngineConfig config;
  config.storage_config = {mapfile};
  config.use_shared_memory = false;
  OSRM osrm{config};
  RouteParameters params;
  params.steps = true;
  json::Object result;
  int OD_counter;

  FILE * ODfile;
  ODfile = fopen (ODfileName.c_str(), "r");
  if ( ODfile == NULL ){
    std::cout << "open ODfile error" << ODfileName << std::endl;
    getchar();
  }
  
  OD_counter = 0;
  double orig_lon, orig_lat, dest_lon, dest_lat;
  while(fscanf(ODfile, "%lf %lf %lf %lf", 
	       &orig_lon, &orig_lat, &dest_lon, &dest_lat)!=EOF)
    {
      params.coordinates.clear();
      params.coordinates.push_back({util::FloatLongitude(orig_lon), util::FloatLatitude(orig_lat)});
      params.coordinates.push_back({util::FloatLongitude(dest_lon), util::FloatLatitude(dest_lat)});

      OD_counter = OD_counter + 1;
      try{
	const auto status = osrm.Route(params, result);
	if (status == Status::Ok)
	  {
	    Trajct trajct;
	    auto &routes = result.values["routes"].get<json::Array>();
	    auto &route = routes.values.at(0).get<json::Object>();
	    const auto distance = route.values["distance"].get<json::Number>().value;
	    const auto duration = route.values["duration"].get<json::Number>().value;
	    auto &legs = route.values["legs"].get<json::Array>();
	    if (legs.values.size() > 1)
	      {
		std::cout << "Multiple legs: there may be more than two viapoints" << "\n";
		return EXIT_FAILURE;

	      }
	    auto &leg = legs.values.at(0).get<json::Object>();
	    auto steps = leg.values["steps"].get<json::Array>();
	    for (int i = 0; i < steps.values.size(); i++ )
	      {	    
		auto step = steps.values.at(i).get<json::Object>();
		auto street_name = step.values["name"].get<json::String>().value;
		//std::cout << "Street Name: " << street_name << "\n";
		trajct.grid.push_back(boost::lexical_cast<int>(street_name));
		trajct.direct.push_back(boost::lexical_cast<int>(street_name));	      
	      }
	    // Warn users if extract does not contain the default Berlin coordinates from above
	    if (distance == 0 or duration == 0)
	      {
		//std::cout << "Note: distance or duration is zero. ";
		//std::cout << "You are probably doing a query outside of the OSM extract.\n";
		//std::cout << "trajectory was not recorded!\n";
		std::cout << OD_counter << ": Distance: " << distance << " meter [trajectory was not recorded!]\n";
	      }
	    else
	      {
		collector.trajctList.push_back(trajct);
		std::cout << OD_counter << ": Distance: " << distance << " meter\n";
	      }

	    //std::cout << "Duration: " << duration << " seconds\n";
	  }
	else if (status == Status::Error)
	  {
	    const auto code = result.values["code"].get<json::String>().value;
	    const auto message = result.values["message"].get<json::String>().value;

	    std::cout << "Code: " << code << "\n";
	    std::cout << "Message: " << code << "\n";
	    return EXIT_FAILURE;
	  }
      }
      catch (const std::exception &e)
	{
	  std::cerr << "Error: " << e.what() << std::endl;
	  return EXIT_FAILURE;
	}
    }
}

