cmake_minimum_required( VERSION 3.14 )
project( TransferSys )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )
include( ODB )

set(headers
	include/dbtypes/Document.hpp
	include/dbtypes/Message.hpp
	include/dbtypes/MessageEntityTextUrl.hpp
	include/dbtypes/MessageReplies.hpp
	include/dbtypes/PeerChanel.hpp
	include/dbtypes/Photo.hpp
    include/dbtypes/ReplyMarkup.hpp
    include/dbtypes/Poll.hpp
    include/dbtypes/Sizes.hpp
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
	src/dbtypes/Document.cpp
	src/dbtypes/Message.cpp
	src/dbtypes/MessageEntityTextUrl.cpp
	src/dbtypes/MessageReplies.cpp
	src/dbtypes/PeerChanel.cpp
	src/dbtypes/Photo.cpp
	src/dbtypes/ReplyMarkup.cpp
	src/dbtypes/Poll.cpp
	src/dbtypes/Sizes.cpp
	${ODB_GENERATED_SOURCES}
)

add_subdirectory(src)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

set(libraries
    odb 
    odb-pgsql
	odb-boost
	torrent-rasterbar
	Threads::Threads
	Boost::boost

	auxiliary
	xmlHandlers
	torHandlers
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14 -Wno-unknown-pragmas" )
include_directories(src/dbtypes
					include/dbtypes
					include/
					)

add_executable(TransferSys src/main.cpp ${sources})
target_link_libraries(TransferSys ${libraries})
