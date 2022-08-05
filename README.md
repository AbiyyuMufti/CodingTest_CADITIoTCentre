# How to Run It

## Requirement

This project is created in *c++11 compiler* using *cmake* as tools in linux environment.

But nevertheless this still can be run in other platform, but please make sure that that your has the following requirement:

1. c++ compiler
2. cmake tools
3. qt5
4. qcustomplot

Assuming that you also are working in linux, then you can do the following to add the dependancy:

1. Start by updating the p
```
sudo apt-get update
```
2. Install the build-essential package by typing:
```
sudo apt-get install build-essential
```
You may also want to install the manual pages about using GNU/Linux for development:
```
sudo apt-get install manpages-dev
```
3. Install cmake tools:
```
sudo apt-get -y install cmake
```
4. Install qt5 and add qcustomplot
```
sudo apt install qt5-default
sudo apt-get install libqcustomplot-dev
```


## Build from this source and run the programm

To build this project you can open terminal in this directory, and then run the following command line:

```
mkdir build
cd build
cmake ..
cmake --build .
```

After that you can run the programs.
To run the first program "salary_conversion":
```
./salary_conversion
```
To run the second program "sensor_aggregation":
```
./sensor_aggregation
```
To run the thrid program "sensor_simulation":
```
./sensor_simulation/sensor_simulation
```

Lastly, to execute the unittest
```
./test/UtilityTest
```
## Run from installed binary