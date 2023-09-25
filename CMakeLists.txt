#-------------------------------------------------------------------------------
#	CMAKE SETTINGS
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8.12)

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
    src/Tests.cpp
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

set(TARGET_NAME "sgtelib_test.exe")

#-------------------------------------------------------------------------------
#	COMPILE FLAGS
#-------------------------------------------------------------------------------

# Common compiler flags for all build types
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# Debug build flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
# Release build flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	set(GUI_TYPE WIN32)
	add_compile_options(/MP)
elseif(CMAKE_COMPILER_IS_GNUCXX)
	add_compile_options(-std=c++17)
endif(MSVC)

#-------------------------------------------------------------------------------
#	GENERATE EXECUTABLE
#-------------------------------------------------------------------------------

# Set additional include directories
include_directories(${INCLUDE_DIRS})

# Targets for compilation (SGTELIB library (sgtelib))
add_library(sgtelib ${SOURCES_LIB})
target_include_directories(sgtelib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Generate the executable
add_executable(${TARGET_NAME} ${GUI_TYPE} ${SOURCES_MAIN})
target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# link libraries
target_link_libraries(${TARGET_NAME} PRIVATE sgtelib ${LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)