cmake_minimum_required(VERSION 3.0)
project(autov C)

if(WIN32)
else()
  set(CMAKE_C_STANDARD 99)
  set (CMAKE_C_FLAGS "-O2 -std=c99 ${CMAKE_C_FLAGS}")
endif()

if(OPT)
  if(WIN32)
    set_source_files_properties(vec.c
      PROPERTIES
      COMPILE_FLAGS "/O2 /Qpar /Qpar-report:2"
      )

  else()
    set(OPT_VERBOSITY "-ftree-vectorizer-verbose=1 -fopt-info-vec-optimized")
    set_source_files_properties(vec.c
      PROPERTIES
      COMPILE_FLAGS "-msse2 -ftree-vectorize ${OPT_VERBOSITY}"
      )

    set_source_files_properties(copy.c
      PROPERTIES
      #COMPILE_FLAGS "-msse2  -ftree-vectorize -fopt-info-vec-optimized"
      COMPILE_FLAGS "-msse2  -ftree-vectorize ${OPT_VERBOSITY}"
      )
  endif()
endif()

add_executable(vec vec.c)
#set_property(TARGET vec PROPERTY C_STANDARD 99)
add_executable(copy copy.c)
