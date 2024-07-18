add_library(xmlHandlers 
            ${CMAKE_SOURCE_DIR}/src/xmlHandlers.cpp
            ${CMAKE_SOURCE_DIR}/include/xmlHandlers.hpp
            )

target_include_directories(xmlHandlers PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(torHandlers 
            ${CMAKE_SOURCE_DIR}/src/torrentHandlers.cpp
            ${CMAKE_SOURCE_DIR}/include/torrentHandlers.hpp
            )

target_include_directories(torHandlers PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(auxiliary 
            ${CMAKE_SOURCE_DIR}/src/aux.cpp
            ${CMAKE_SOURCE_DIR}/include/aux.hpp
            )

target_include_directories(auxiliary PUBLIC ${CMAKE_SOURCE_DIR}/include)
