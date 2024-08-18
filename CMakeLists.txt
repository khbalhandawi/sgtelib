#-------------------------------------------------------------------------------
#	CMAKE SETTINGS
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
set(SGTELIB_VERSION_MAJOR 0)
set(SGTELIB_VERSION_MINOR 1)
set(SGTELIB_VERSION_PATCH 0)
set(SGTELIB_VERSION ${SGTELIB_VERSION_MAJOR}.${SGTELIB_VERSION_MINOR}.${SGTELIB_VERSION_PATCH})

# name of the project
project (sgtelib LANGUAGES CXX VERSION ${SGTELIB_VERSION})

# require c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "FATAL: In-source builds are not allowed.
    You should create a separate directory for build files.")
endif()

# check compiler version
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4)
        message(FATAL_ERROR "GCC version < 4 has not been tested for SGTELIB")
    endif()
elseif (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    # require at least clang 5
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5)
        message(FATAL_ERROR "Clang version has not been tested for SGTELIB")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # require at least 15.0 (MSVC 2017) for C++17 support
    if (MSVC_TOOLSET_VERSION VERSION_LESS 143)
        message(FATAL_ERROR "MSVC version ${CMAKE_CXX_COMPILER_VERSION} has not been tested for SGTELIB")
    endif()
else()
    message(WARNING "You are using an unsupported compiler. Compilation has only been tested with Clang, GCC, or MSVC.")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#-------------------------------------------------------------------------------
#	SOURCE CODE AND USER SETTINGS
#-------------------------------------------------------------------------------

set(SOURCES_LIB
    src/TrainingSet.cpp
    src/Surrogate_Parameters.cpp
    src/Surrogate_KS.cpp
    src/Surrogate_RBF.cpp
    src/Surrogate_PRS.cpp
    src/Surrogate_PRS_EDGE.cpp
    src/Surrogate_LOWESS.cpp
    src/Surrogate_Kriging.cpp
    src/Surrogate_PRS_CAT.cpp
    src/Surrogate_Ensemble.cpp
    src/Surrogate_CN.cpp
    src/Surrogate.cpp
    src/Matrix.cpp
    src/Kernel.cpp
    src/Surrogate_Utils.cpp
    src/Surrogate_Factory.cpp
    src/sgtelib_help.cpp
    src/Metrics.cpp
)

set(SOURCES_MAIN
    src/main.cpp
)

#-------------------------------------------------------------------------------
#	PROJECT SETTINGS
#-------------------------------------------------------------------------------

set(PROJECT_NAME "sgtelib")
cmake_policy(SET CMP0048 NEW)
project(${PROJECT_NAME} VERSION 0.1.0)

set(TARGET_NAME "sgtelib")


#-------------------------------------------------------------------------------
#	COMPILE FLAGS
#-------------------------------------------------------------------------------

# Common compiler flags for all build types
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# Debug build flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
# Release build flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	set(GUI_TYPE WIN32)
	add_compile_options(/MP)
elseif(CMAKE_COMPILER_IS_GNUCXX)
	add_compile_options(-std=c++17)
endif(MSVC)

#-------------------------------------------------------------------------------
#	OPTIONAL EXECUTABLE BUILD
#-------------------------------------------------------------------------------

# Option to build the main executable
option(BUILD_MAIN_EXECUTABLE "Build the main executable from main.cpp" OFF)

if(BUILD_MAIN_EXECUTABLE)
    add_executable(sgtelib.exe ${SOURCES_MAIN})
    target_link_libraries(sgtelib.exe PRIVATE sgtelib)
    message(STATUS "Building main executable: sgtelib.exe")
endif()

#-------------------------------------------------------------------------------
#	GENERATE LIBRARY
#-------------------------------------------------------------------------------

# Targets for compilation (SGTELIB library (sgtelib))
# add_library(sgtelib ${SOURCES_LIB})
# target_include_directories(sgtelib PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# build shared library sgtelib
add_library (sgtelib SHARED ${SOURCES_LIB})
target_include_directories(sgtelib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# export symbols on Windows
if (WIN32)
    target_compile_definitions(sgtelib PRIVATE DLL_EXPORTS)
endif()


#-------------------------------------------------------------------------------
#	UNIT TESTS
#-------------------------------------------------------------------------------

# Option to build the unit tests
option(BUILD_UNIT_TESTS "Build the unit tests" OFF)

if(BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

#-------------------------------------------------------------------------------
#	INSTALLATION
#-------------------------------------------------------------------------------

# installing executables and libraries
install(TARGETS sgtelib
    RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin
    ARCHIVE DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib
    LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include
    INCLUDES DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include
)

if(MSVC)
    install(FILES $<TARGET_PDB_FILE:sgtelib> DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR} OPTIONAL)
    target_compile_options(sgtelib PRIVATE /wd4002)
    target_compile_options(sgtelib PRIVATE /wd4267)
endif()

# Install the main executable
if(BUILD_MAIN_EXECUTABLE)
    install(TARGETS sgtelib.exe
        RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin
    )
    install(TARGETS sgtelib
        LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin
    )

    install(TARGETS sgtelib.exe
        RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../test_data
    )
    install(TARGETS sgtelib
        LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../test_data
    )
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
