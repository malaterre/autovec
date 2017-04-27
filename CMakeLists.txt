cmake_minimum_required(VERSION 3.2)
project(autov C)
set(CMAKE_C_STANDARD 11)

if(OPT)
  if(WIN32)
	set_source_files_properties(vec.c
			PROPERTIES
			COMPILE_FLAGS "/O2 /Qpar /Qpar-report:2"
			)

  else()
	set_source_files_properties(vec.c
			PROPERTIES
			COMPILE_FLAGS "-msse2  -ftree-vectorize -fopt-info-vec-optimized"
			)
  endif()
endif()

add_executable(vec vec.c)
