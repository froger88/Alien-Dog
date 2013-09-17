cmake_minimum_required (VERSION 2.8)
project (alien_dog)

SET(CMAKE_COLOR_MAKEFILE TRUE)
SET(CMAKE_BUILD_TYPE Release)

# use c++11
ADD_DEFINITIONS(
    -std=c++11
)

add_executable (alien_dog
	main.cpp
)

target_link_libraries (alien_dog
	pthread
	ssl
)
