cmake_minimum_required(VERSION 2.6)
project(velocity)
set(VELOCITY_PLUGIN_NAME velocity)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_BUILD_TYPE RELEASE)

## Check for correct compiler
## Using C compiler GNUCXX for c++ compiler
#if(CMAKE_COMPILER_IS_GNUCC)
#    set(CMAKE_CXX_FLAGS "-ggdb -D_DEBUG -O0")
#endif(CMAKE_COMPILER_IS_GNUCC)

# Maya SDK
set(MAYA_ROOT "/usr/autodesk/maya2016")
set(MAYA_INCLUDE ${MAYA_ROOT}/include)
set(MAYA_LIB ${MAYA_ROOT}/lib)

set(UTILITIES_HEADER_FILES
        include/utilities/debugUtils.h
        )

set(VELOCITY_SOURCE_FILES
        ${UTILITIES_HEADER_FILES}
        include/velocityNode.h
        src/velocityNode.cpp
        src/velocityMain.cpp
        )

include_directories(
        include
        ${UTILS_INCLUDE}
        ${MAYA_INCLUDE}
)

link_directories(
        ${LIBRARY_OUTPUT_PATH}
        ${MAYA_LIB}
)

# maya plugin library
add_library(${VELOCITY_PLUGIN_NAME} SHARED ${VELOCITY_SOURCE_FILES})
target_link_libraries(${VELOCITY_PLUGIN_NAME}
        OpenMaya
        Foundation)
set_target_properties(${VELOCITY_PLUGIN_NAME} PROPERTIES
        PREFIX "" # no 'lib' prefix to .so files
        )

