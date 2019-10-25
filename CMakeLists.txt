cmake_minimum_required(VERSION 2.8)
set(project_name xorm)
project(${project_name})

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -pthread -std=c++14")
endif ()

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_INSTALL_RPATH "/usr/local/lib:")

set(SOURCE_FILES main.cpp)

add_executable(${project_name} ${SOURCE_FILES})

if (MSVC)
target_link_libraries(${project_name} libmysql)
else()
target_link_libraries(${project_name} mysqlclient)
endif()

install(TARGETS ${project_name} DESTINATION include)
