cmake_minimum_required(VERSION 3.12)
project(auth-service CXX)

macro(SET_OPT optname value)
  set(${optname} ${value} CACHE INTERNAL "" FORCE)
endmacro()

SET_OPT(USERVER_NO_WERROR ON)
SET_OPT(USERVER_FEATURE_POSTGRESQL ON)
SET_OPT(USERVER_FEATURE_CRYPTOPP_BLAKE OFF)
SET_OPT(USERVER_CHECK_PACKAGE_VERSIONS OFF)
SET_OPT(USERVER_FEATURE_MONGODB OFF)
SET_OPT(USERVER_FEATURE_REDIS OFF)
SET_OPT(USERVER_FEATURE_CLICKHOUSE OFF)
SET_OPT(USERVER_FEATURE_GRPC OFF)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(third_party/userver/cmake/SetupEnvironment.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(third_party/userver)
add_subdirectory(third_party/Bcrypt.cpp)
add_subdirectory(src)

