# Create a library called "Hello" which includes the source file "hello.cxx". 
# The extension is already found. Any number of sources could be listed here. 


project(uav_math)
cmake_minimum_required(VERSION 2.6)

add_library (uav_math "Conversions.cpp" "gps_time.cpp" "./llh_utm/llh_utm.cpp" )

