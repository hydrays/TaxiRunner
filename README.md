# TaxiRunner
Simulate city-wide taxi movement.

# Pre-request
GDAL
OSRM installed as lib
A set of working .osrm map data files (generated using osrm-extract, osrm-prepare). You may need a profile.lua file to generate these files from a OSM map file. Also copy SampleTiff.tif into the current folder.

# Install
```
mkdir build
cd build
cmake ..
make
```

# Run
change the file names in the code to locate the OD file and OSRM file (currently in the collector.hpp files).

# Contact:
Yucheng Hu
Zhou Pei-yuan Center for Applied Mathematics, Tsinghua University, Beijing, China, 100084.
huyc@tsinghua.edu.cn
