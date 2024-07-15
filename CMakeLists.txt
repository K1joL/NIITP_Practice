cmake_minimum_required( VERSION 3.14 )
project( TransferSys )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )
include( ODB )

set(headers
	src/dbtypes/Audio.hpp
	src/dbtypes/Chat.hpp
	src/dbtypes/Contact.hpp
	src/dbtypes/Document.hpp
	src/dbtypes/File.hpp
	src/dbtypes/Location.hpp
    src/dbtypes/User.hpp
    src/dbtypes/Video.hpp
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
)

set(sources
	src/dbtypes/Audio.cpp
	src/dbtypes/Chat.cpp
	src/dbtypes/Contact.cpp
	src/dbtypes/Document.cpp
	src/dbtypes/File.cpp
	src/dbtypes/Location.cpp
	src/dbtypes/User.cpp
	src/dbtypes/Video.cpp
	${ODB_GENERATED_SOURCES}
)

add_subdirectory(src)

find_package(Threads REQUIRED)

set(libraries
    odb 
    odb-pgsql
	auxiliary
	torrent-rasterbar
	Threads::Threads
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14 -Wno-unknown-pragmas" )
include_directories(src/dbtypes
					include/)

add_executable(TransferSys src/main.cpp ${sources})
target_link_libraries(TransferSys ${libraries})
