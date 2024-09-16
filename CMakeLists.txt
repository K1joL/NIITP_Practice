cmake_minimum_required( VERSION 3.14 )
project( TransferSys )

add_subdirectory(src)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

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


set(libraries
    odb 
    odb-pgsql
	odb-boost
	torrent-rasterbar
	Threads::Threads
	Boost::boost
	${CONAN_LIBS}

	auxiliary
	xmlHandlers
	torHandlers
	databaseHandler
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14 -Wno-unknown-pragmas" )
include_directories(
    include/
    include/databaseHandler
    )

add_executable(TransferSys src/main.cpp)
target_link_libraries(TransferSys ${libraries})
