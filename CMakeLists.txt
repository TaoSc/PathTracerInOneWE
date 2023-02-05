cmake_minimum_required(VERSION 3.20)
project(PathTracing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main main.cpp)

# add_library(utilities
# 	"utilities/vec3.h"
# )

include_directories(
	"${PROJECT_SOURCE_DIR}/externals"
	"${PROJECT_SOURCE_DIR}/hittables"
	"${PROJECT_SOURCE_DIR}/materials"
	"${PROJECT_SOURCE_DIR}/scenes"
	"${PROJECT_SOURCE_DIR}/textures"
	"${PROJECT_SOURCE_DIR}/utilities"
)
