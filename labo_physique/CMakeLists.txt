cmake_minimum_required(VERSION 3.21)

project(labo_physique)

# Setup language requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
    set(OpenMP_C_LIB_NAMES "omp")
    set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
    set(OpenMP_CXX_LIB_NAMES "omp")
    set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")
endif()
find_package(OpenMP REQUIRED)

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
include_directories(${PROJECT_SOURCE_DIR}/../labo_fk/src)
include_directories(${PROJECT_SOURCE_DIR}/../labo01/src ${COMMON_INCLUDES})

find_package(OpenGL REQUIRED)

# Add .cpp and .h files
set(HEADERS ParticleSimGLCanvas.h ParticleSimApplication.h ParticleSystem.h ParticleSystemFactory.h Vector2d.h Solvers.h GraphColoring.h)
set(SOURCE main.cpp ParticleSimApplication.cpp ParticleSimGLCanvas.cpp ParticleSystem.cpp ParticleSystemFactory.cpp GraphColoring.cpp)
add_executable(labo_physique ${SOURCE} ${HEADERS})
target_link_libraries(labo_physique nanogui ${NANOGUI_EXTRA_LIBS} OpenMP::OpenMP_CXX "-framework OpenGL")

if(MSVC) 
	set_property(TARGET labo_physique PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/labo_physique)
endif()