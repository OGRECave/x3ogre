cmake_minimum_required (VERSION 2.8)

project(x3ogre)

include(GenerateExportHeader)
add_compiler_export_flags()

if(WIN32)
    # explanation: needed for M_PI etc, parallel compiling, target win8
    add_definitions(-D_USE_MATH_DEFINES -DMP -D_WIN32_WINNT=0x0602)
    add_definitions(-W2 -wd4305) # tame warnings
else()
    add_definitions(-std=c++11 -Wall -Wextra -Wcast-align -Wno-unused-parameter -Wno-overloaded-virtual)
endif()

# Find OGRE
find_package(OGRE 1.10 QUIET REQUIRED)
link_directories(${OGRE_LIBRARY_DIRS})

# Specify OGRE Include DIRS
include_directories(${OGRE_INCLUDE_DIRS})
 
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif(CMAKE_BUILD_TYPE STREQUAL "")

# reflection db generator
file(GLOB nodes_HDR
    ${PROJECT_SOURCE_DIR}/Animation/*.h
    ${PROJECT_SOURCE_DIR}/Appearance/*.h
    ${PROJECT_SOURCE_DIR}/Geometry/*.h
    ${PROJECT_SOURCE_DIR}/World/*.h)

add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/reflection/db.cpp
    COMMAND python generate.py ${nodes_HDR}
    DEPENDS ${PROJECT_SOURCE_DIR}/reflection/db.cpp.in ${nodes_HDR}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/reflection/)

# X3DCore
file(GLOB core_SRC
    ${PROJECT_SOURCE_DIR}/core/*.cpp
    ${PROJECT_SOURCE_DIR}/GeoShape/*.cpp
    ${PROJECT_SOURCE_DIR}/Parser/*.cpp)
    
add_library(X3OgreCore ${core_SRC} 
    ${PROJECT_SOURCE_DIR}/reflection/db.cpp
    ${PROJECT_SOURCE_DIR}/reflection/TypeInfo.cpp)

generate_export_header(X3OgreCore
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/x3ogre_core_export.h)

target_include_directories(X3OgreCore PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})

# Nodes
file(GLOB nodes_SRC
    ${PROJECT_SOURCE_DIR}/Animation/*.cpp
    ${PROJECT_SOURCE_DIR}/Appearance/*.cpp
    ${PROJECT_SOURCE_DIR}/Geometry/*.cpp
    ${PROJECT_SOURCE_DIR}/World/*.cpp)

add_library(X3DNodes ${nodes_SRC})
target_link_libraries(X3DNodes X3OgreCore ${OGRE_LIBRARIES})

set(MEDIA_DIR ${CMAKE_BINARY_DIR}/x3ogre_media/)
# RTSS data
file(COPY 
    "${OGRE_MEDIA_DIR}/RTShaderLib/GLSL"
    "${OGRE_MEDIA_DIR}/RTShaderLib/GLSLES"
    DESTINATION "${MEDIA_DIR}/RTShaderLib/")

# SDK Tray data
file(COPY
    "${OGRE_MEDIA_DIR}/packs/SdkTrays.zip" 
    DESTINATION ${MEDIA_DIR}/packs/)

# sample shaders
file(COPY 
    "${OGRE_MEDIA_DIR}/materials/programs/GLSLES"
    DESTINATION "${MEDIA_DIR}/materials/programs/")

# some compositors
file(COPY
    "${OGRE_MEDIA_DIR}/materials/scripts/Examples.compositor"
    "${OGRE_MEDIA_DIR}/materials/scripts/Invert.material"
    "${OGRE_MEDIA_DIR}/materials/scripts/RadialBlur.material"
    # "${OGRE_MEDIA_DIR}/materials/scripts/NightVision.material" #local changes
    DESTINATION "${MEDIA_DIR}/materials/scripts/")

# textures
file(COPY
    "${OGRE_MEDIA_DIR}/materials/textures/Random3D.dds"
    DESTINATION "${MEDIA_DIR}/materials/textures/")

# the X3D
file(COPY
    "${PROJECT_SOURCE_DIR}/examples/flipper.x3d"
    "${PROJECT_SOURCE_DIR}/examples/tie.x3d"
    "${PROJECT_SOURCE_DIR}/examples/swarm.x3d"
    DESTINATION "${MEDIA_DIR}/x3d/")
set(OGRE_MEDIA_DIR ${MEDIA_DIR})

configure_file(${PROJECT_SOURCE_DIR}/templates/resources.cfg.in ${CMAKE_BINARY_DIR}/resources.cfg)
file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg DESTINATION ${CMAKE_BINARY_DIR})

if(EMSCRIPTEN)
    add_definitions(-s USE_SDL=2)
    set(CMAKE_EXE_LINKER_FLAGS "-s WASM=1 -s USE_SDL=2 -s USE_WEBGL2=1 --preload-file ${MEDIA_DIR}/@. --bind --shell-file ${PROJECT_SOURCE_DIR}/templates/shell.html")
    # debugging
    # set(CMAKE_EXE_LINKER_FLAGS "-O0 -g -s ASSERTIONS=2 -s DEMANGLE_SUPPORT=1 -s DISABLE_EXCEPTION_CATCHING=0 -s EXCEPTION_DEBUG=1 ${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    list(APPEND OGRE_LIBRARIES freetype zzip z)

    set(OGRE_MEDIA_DIR "")
    configure_file(${PROJECT_SOURCE_DIR}/templates/resources.cfg.in ${MEDIA_DIR}/resources.cfg)
endif()

add_executable(x3ogre apps/viewer.cpp)
target_link_libraries(x3ogre X3OgreCore X3DNodes ${OGRE_LIBRARIES})
