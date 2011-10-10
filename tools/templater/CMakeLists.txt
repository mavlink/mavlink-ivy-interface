# Create a library called "Hello" which includes the source file "hello.cxx". 
# The extension is already found. Any number of sources could be listed here. 


project(templater)
cmake_minimum_required(VERSION 2.6)

add_executable (templater "templater.cpp" )

