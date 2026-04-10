cmake_minimum_required(VERSION 3.21)

project(labo_fk)

# Setup language requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#--------------------------------------------------
# Sous MAC, OpenGL est Deprecated, mais toujours
# fonctionnel, on veut éviter tous les warnings
# durant la compilation.
#--------------------------------------------------
if (APPLE)
  add_definitions( -DGL_SILENCE_DEPRECATION )
endif()

add_definitions(${NANOGUI_EXTRA_DEFS})
include_directories(${nanogui_SRC_DIR}/include)
include_directories(${NANOGUI_EXTRA_INCS})

find_package(OpenGL REQUIRED)

# Add .cpp and .h files
file(GLOB_RECURSE TESTS_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE MAIN_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "src/*.h")
file(GLOB_RECURSE MAIN_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "src/*.cpp")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${COMMON_INCLUDES})

add_executable(labo_fk main.cpp ${MAIN_SOURCES} ${MAIN_HEADERS} ${TESTS_SOURCES})
target_link_libraries(labo_fk nanogui gtest ${NANOGUI_EXTRA_LIBS})

if(MSVC) 
	set_property(TARGET labo_fk PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/labo_fk)
endif()