# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(FetchWeatherData)

# Enable OpenMP
find_package(OpenMP REQUIRED)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include directories for JSONCPP
include_directories(/usr/include/jsoncpp)

# Add the executable
add_executable(fetch main.cpp fetch.cpp)

# Link the required libraries
target_link_libraries(fetch
    OpenMP::OpenMP_CXX
    curl
    jsoncpp
)

# Link the libraries based on your requirements
# You can add other libraries if needed.

