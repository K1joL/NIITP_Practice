set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )
include( ODB )
# XML HANDLERS LIBRARY
add_library(xmlHandlers 
            ${CMAKE_SOURCE_DIR}/src/xmlHandlers.cpp
            ${CMAKE_SOURCE_DIR}/include/xmlHandlers.hpp
            )

target_include_directories(xmlHandlers PUBLIC ${CMAKE_SOURCE_DIR}/include)
# TORRENT HANDLERS LIBRARY
find_package(Threads REQUIRED)
add_library(torHandlers 
            ${CMAKE_SOURCE_DIR}/src/torrentHandlers.cpp
            ${CMAKE_SOURCE_DIR}/include/torrentHandlers.hpp
            )
set(libraries
	torrent-rasterbar
	Threads::Threads
)
target_link_libraries(torHandlers PRIVATE ${libraries})
target_include_directories(torHandlers PUBLIC ${CMAKE_SOURCE_DIR}/include)

# AUXILIARY LIBRARY
add_library(auxiliary 
            ${CMAKE_SOURCE_DIR}/src/aux.cpp
            ${CMAKE_SOURCE_DIR}/include/aux.hpp
            )

target_include_directories(auxiliary PUBLIC ${CMAKE_SOURCE_DIR}/include)

# DATABASE HANDLER LIBRARY
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_autodetect(settings)
conan_cmake_run(REQUIRES
                    jinja2cpp/1.1.0
                    gtest/1.14.0
                BASIC_SETUP
                ${CONAN_SETTINGS}
                OPTIONS
                    jinja2cpp/*:shared=False
                    gtest/*:shared=False
                BUILD missing)


set(headers
    ${CMAKE_SOURCE_DIR}/include/databaseHandler/dbtypes/Addressees.hpp
    ${CMAKE_SOURCE_DIR}/include/databaseHandler/dbtypes/Contacts.hpp
    ${CMAKE_SOURCE_DIR}/include/databaseHandler/dbtypes/DocInfos.hpp
    ${CMAKE_SOURCE_DIR}/include/databaseHandler/dbtypes/Document.hpp
    ${CMAKE_SOURCE_DIR}/include/databaseHandler/dbtypes/Fields.hpp
    ${CMAKE_SOURCE_DIR}/include/databaseHandler/dbtypes/Parts.hpp
)

odb_compile(
    HEADERS ${headers}
	DB pgsql
	STANDARD c++14
	GENERATE_QUERY GENERATE_SESSION GENERATE_SCHEMA
	HEADER_SUFFIX ".hpp"
	INLINE_SUFFIX ".ipp"
	SOURCE_SUFFIX ".cpp"
	FILE_SUFFIX ".odb"
	DEFAULT_POINTER "std::shared_ptr"
	PROFILE "boost/date-time"
)

set(sources
    ${CMAKE_SOURCE_DIR}/src/databaseHandler/dbtypes/Addressees.cpp
    ${CMAKE_SOURCE_DIR}/src/databaseHandler/dbtypes/Contacts.cpp
    ${CMAKE_SOURCE_DIR}/src/databaseHandler/dbtypes/DocInfos.cpp
    ${CMAKE_SOURCE_DIR}/src/databaseHandler/dbtypes/Document.cpp
    ${CMAKE_SOURCE_DIR}/src/databaseHandler/dbtypes/Fields.cpp
    ${CMAKE_SOURCE_DIR}/src/databaseHandler/dbtypes/Parts.cpp
	${ODB_GENERATED_SOURCES}
)

set(libraries
    odb 
    odb-pgsql
    odb-boost
    ${CONAN_LIBS}
)
add_library(databaseHandler
            ${CMAKE_SOURCE_DIR}/src/databaseHandler/databaseHandler.cpp
            ${CMAKE_SOURCE_DIR}/include/databaseHandler/databaseHandler.hpp  
            ${CMAKE_SOURCE_DIR}/include/databaseHandler/dbConstants.hpp
            ${CMAKE_SOURCE_DIR}/include/databaseHandler/dbTypes.hpp
            ${sources}
            ${headers}
)
target_link_libraries(databaseHandler PRIVATE ${libraries})
target_include_directories(databaseHandler PUBLIC 
                            ${CMAKE_SOURCE_DIR}/include/databaseHandler
                            ${CMAKE_SOURCE_DIR}/include/databaseHandler/dbtypes
                            ${CMAKE_SOURCE_DIR}/build/src/odb
)
