#include "convector.hpp"

Convector::Convector()
{
}

int Convector::init(const Collector &collector)
{
  mapfile = collector.mapfile;

  boost::property_tree::ptree pTree;
  try {
    read_xml("config.xml", pTree);
    std::cout << "reading config file: " << "config.xml" << std::endl;
  }
  catch (boost::property_tree::xml_parser_error e) {
    std::cout << "error" << std::endl;
  }

  try {
    gridinfo[0] = pTree.get<double>("main.lonmin");
    std::cout << "lonmin: " << gridinfo[0] << std::endl;
    gridinfo[1] = pTree.get<double>("main.lonmax");
    std::cout << "lonmax: " << gridinfo[1] << std::endl;
    gridinfo[2] = pTree.get<double>("main.latmin");
    std::cout << "latmin: " << gridinfo[2] << std::endl;
    gridinfo[3] = pTree.get<double>("main.latmax");
    std::cout << "latmax: " << gridinfo[3] << std::endl;
  }
  catch(boost::property_tree::ptree_bad_path e) {
    std::cout << "error" << std::endl;
  }
  gridinfo[6] = collector.Ndim;
  gridinfo[7] = collector.Mdim;
  gridinfo[4] = (gridinfo[1] - gridinfo[0])/gridinfo[7]; //xres
  gridinfo[5] = (gridinfo[3] - gridinfo[2])/gridinfo[6]; //yres
  return 0;
}

int Convector::make_road_network(const Collector &collector)
{
  int Ndim = collector.Ndim;
  int Mdim = collector.Mdim;
  int i,j;
  double coord[2];
  for (i=0; i<Ndim; i++)
    {
      for (j=0; j<Mdim; j++)
	{
	  Node newnode;
	  newnode.id = i*Ndim+j+1;
	  newnode.name = boost::lexical_cast<std::string>(i*Ndim+j+1);
	  // cell number start with 1 (not 0)
	  xy_from_cell(i*Ndim+j+1, coord);
	  newnode.lat = coord[1];
	  newnode.lon = coord[0];
	  road_network.node.push_back(newnode);

	  Way newway;
	  newway.id = i*Ndim+j+1;
	  newway.name = boost::lexical_cast<std::string>(i*Ndim+j+1);
	  newway.speed = 1.0/double(i*Ndim+j+1);
	  newway.nd.push_back(1);
	  newway.nd.push_back(2);
	  road_network.way.push_back(newway);
	}
    }
  return 0;

  /*
    ## require(XML)
## require(raster)

## ## read config file
## d <- xmlTreeParse("config.xml")
## r <- xmlRoot(d)
## Ndim <- as.numeric(xmlValue(r[["Ndim"]]))
## Mdim <- as.numeric(xmlValue(r[["Mdim"]]))
## lonmin <- as.numeric(xmlValue(r[["lonmin"]]))
## lonmax <- as.numeric(xmlValue(r[["lonmax"]]))
## latmin <- as.numeric(xmlValue(r[["latmin"]]))
## latmax <- as.numeric(xmlValue(r[["latmax"]]))

## ## generate raster
## rast <- raster(ncols=Ndim, nrows=Mdim)
## extent(rast) <- c(lonmin, lonmax, latmin, latmax)
## projection(rast) <- CRS("+proj=longlat +datum=WGS84")
## ##extent(rast) <- c(116.2847, 116.36, 39.95, 40.00766)

## ## compute node
## cat("compute node...\n")
## nf <- lapply(seq_along(rast),function(x){dum.nf <- c(x, xFromCell(rast,x), yFromCell(rast,x), 1)})

## ## compute way
## cat("compute way...\n")
## wf <- matrix(0, nrow=4096*4, ncol=5)
## wid <- 0
## Direction <- "Left"
## WM1 <- brick(paste("WM/WM", Direction, "All.grd", sep=""))
## WM1[is.na(WM1[])] <- 0.0
## WM1 <- mean(WM1, na.rm=TRUE)

## Direction <- "Right"
## WM2 <- brick(paste("WM/WM", Direction, "All.grd", sep=""))
## WM2[is.na(WM2[])] <- 0.0
## WM2 <- mean(WM2, na.rm=TRUE)

## Direction <- "Down"
## WM3 <- brick(paste("WM/WM", Direction, "All.grd", sep=""))
## WM3[is.na(WM3[])] <- 0.0
## WM3 <- mean(WM3, na.rm=TRUE)

## Direction <- "Up"
## WM4 <- brick(paste("WM/WM", Direction, "All.grd", sep=""))
## WM4[is.na(WM4[])] <- 0.0
## WM4 <- mean(WM4, na.rm=TRUE)

## for ( i in seq(Ndim) ){
##     cat(i, wid, "\n")
##     for ( j in seq(Mdim) ){
##         if ( i>1 ){ ## add way to the left
##             wid <- wid + 1
##             wf[wid,] <- c(wid, 1, cellFromRowCol(rast,j,i), cellFromRowCol(rast,j,i-1), max(0.001,WM1[j,i]))
##         }
##         if ( i<Ndim ){ ## add way to the right
##             wid <- wid + 1
##             wf[wid,] <- c(wid, 1, cellFromRowCol(rast,j,i), cellFromRowCol(rast,j,i+1), max(0.001,WM2[j,i]))
##         }
##         if ( j>1 ){ ## add way to the down
##             wid <- wid + 1
##             wf[wid,] <- c(wid, 1, cellFromRowCol(rast,j,i), cellFromRowCol(rast,j-1,i), max(0.001,WM3[j,i]))
##         }
##         if ( j<Mdim ){ ## add way to the up
##             wid <- wid + 1
##             wf[wid,] <- c(wid, 1, cellFromRowCol(rast,j,i), cellFromRowCol(rast,j+1,i), max(0.001,WM4[j,i]))
##         }            
##     }
## }
## wf <- wf[1:wid,]
## wf[,5] <- 100*wf[,5]
   */
}

int Convector::output_road_network()
{
/*
z <- xmlTree("osm", attrs=c(version="0.6", generator="R"))
z$addNode("bounds", attrs=c(minlat=latmin, minlon=lonmin, maxlat=latmax, maxlon=lonmax))

## generate node
cat("generate node...\n")
nodefill <- function(x){
##for (x in seq(3)){
    cat("node ", x, "\n")
    z$addNode("node", attrs = c(id=nf[[x]][1],
                                lon=nf[[x]][2],
                                lat=nf[[x]][3],
                                version=nf[[x]][4]), close=FALSE)
    z$addTag("tag", attrs =c(k="highway",v="motorway_junction"))
    z$closeTag()
}

## generate way
cat("generate way...\n")
wayfill <- function(x){
##for (x in seq(3)){
    cat("way ", x, "\n")
    z$addNode("way", attrs = c(id=wf[x,1], version=wf[x,2]), close=FALSE)
    z$addTag("nd", attrs =c(ref=wf[x,3]))
    z$addTag("nd", attrs =c(ref=wf[x,4]))
    z$addTag("tag", attrs =c(k="highway",v="residential"))
    z$addTag("tag", attrs =c(k="maxspeed",v=wf[x,5]))
    z$addTag("tag", attrs =c(k="junction",v="traffic_signals"))
    z$addTag("tag", attrs =c(k="name",v=wf[x,1]))
    z$closeTag()
}

system.time(lapply(seq_along(nf), nodefill))
system.time(lapply(seq_along(wf[,1]), wayfill))
cat("output xml file...\n")
saveXML(z, "gridMap.osm")
*/

  using namespace boost::property_tree;
  ptree pt;
  ptree osm;
  ptree bound;
  ptree node;
  ptree way;
  ptree tag;
  ptree nd;

  bound.put("<xmlattr>.minlat", gridinfo[2]);
  bound.put("<xmlattr>.minlon", gridinfo[0]);
  bound.put("<xmlattr>.maxlat", gridinfo[3]);
  bound.put("<xmlattr>.matlon", gridinfo[1]);
  osm.add_child("bounds", bound);
  bound.clear();

  for (int i=0; i<road_network.node.size(); i++)
    {
      //generate xml entries for node
      tag.put("<xmlattr>.k", "highway");  
      tag.put("<xmlattr>.v", "motorway_junction");  
      node.put("<xmlattr>.id", road_network.node[i].id);  
      node.put("<xmlattr>.lat", road_network.node[i].lat);  
      node.put("<xmlattr>.lon", road_network.node[i].lon);  
      node.put("<xmlattr>.version", 1);  
      node.add_child("tag", tag);
      tag.clear();
      osm.add_child("node", node);  
      node.clear();  
    }
  for (int i=0; i<road_network.way.size(); i++)
    {
      //generate xml entries for node      
      way.put("<xmlattr>.id", road_network.way[i].id);  
      way.put("<xmlattr>.version", 1);  
      way.put("<xmlattr>.close", "FALSE");  

      for (int j=0; j<road_network.way[i].nd.size(); j++)
	{
	  nd.put("<xmlattr>.ref", road_network.way[i].nd[j]);  
	  way.add_child("nd", nd);
	  nd.clear();
	}
      tag.put("<xmlattr>.k", "highway");  
      tag.put("<xmlattr>.v", "residential");  
      way.add_child("tag", tag);
      tag.clear();
      tag.put("<xmlattr>.k", "maxspeed");  
      tag.put("<xmlattr>.v", 60.0);  
      way.add_child("tag", tag);
      tag.clear();
      tag.put("<xmlattr>.k", "junction");  
      tag.put("<xmlattr>.v", "traffic_signals");  
      way.add_child("tag", tag);
      tag.clear();
      tag.put("<xmlattr>.k", "name");  
      tag.put("<xmlattr>.v", 1);  
      way.add_child("tag", tag);
      tag.clear();

      osm.add_child("way", way);  
      way.clear();  
    }

  osm.put("<xmlattr>.version", "0.6");
  osm.put("<xmlattr>.generator", "C++");
  pt.add_child("osm", osm);
  boost::property_tree::xml_writer_settings<char> settings('\t', 1);   
  write_xml("test.osm", pt, std::locale(), settings);  
  return 0;
}

int Convector::xy_from_cell(long int i, double * coord)
{
  double xmin = gridinfo[0];
  double xmax = gridinfo[1];
  double ymin = gridinfo[2];
  double ymax = gridinfo[3];
  double xres = gridinfo[4];
  double yres = gridinfo[5];
  int Ndim = gridinfo[6];
  int Mdim = gridinfo[7];
  int xi, yi;
  if ( (i < 1) | (i > Ndim*Mdim) )
    {
      printf(" error: i = %ld \n", i);
      //error("[C] y out of bound in rowFromY \n");
      getchar();
      return(-777);
    }
  xi = (i-1)%Mdim;
  yi = floor((i-1)/Ndim);
  coord[0] = xmin + (xi+0.5)*xres;
  coord[1] = ymax - (yi+0.5)*xres;
  return(0);
}

