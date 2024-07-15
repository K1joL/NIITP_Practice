add_library(auxiliary 
            ${CMAKE_SOURCE_DIR}/src/aux.cpp
            ${CMAKE_SOURCE_DIR}/include/aux.hpp
            )

target_include_directories(auxiliary PUBLIC ${CMAKE_SOURCE_DIR}/include)
