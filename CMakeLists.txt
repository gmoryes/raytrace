cmake_minimum_required(VERSION 3.9)
project(mashgraph3)

set(CMAKE_CXX_STANDARD 11)

include_directories(lib/EasyBMP/include)

SET(EASY_BMP lib/EasyBMP/src/EasyBMP.cpp
    lib/EasyBMP/include/EasyBMP.h
    lib/EasyBMP/include/EasyBMP_BMP.h
    lib/EasyBMP/include/EasyBMP_DataStructures.h
    lib/EasyBMP/include/EasyBMP_VariousBMPutilities.h)

set(SOURCE
    main.cpp

    include/ArgumentsParser.h
    src/ArgumentsParser.cpp

    include/BaseStructures.h
    src/BaseStructures.cpp

    include/Figures.h
    src/Figures.cpp

    include/Scene.h
    src/Scene.cpp)


set(OpenMP_CXX_FLAGS "-fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

add_executable(mashgraph3 ${SOURCE} ${EASY_BMP})

target_include_directories(mashgraph3 PRIVATE include)