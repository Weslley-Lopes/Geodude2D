project(runPFEM)

cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")

find_package(MPI)
find_package(PETSc REQUIRED)

include_directories(include ${MPI_INCLUDE_PATH} ${PETSC_INCLUDES})
add_definitions(${PETSC_DEFINITIONS})

add_subdirectory(external_libraries)
add_subdirectory(src)

link_directories(${PETSC_DIR}/${PETSC_ARCH}/lib)

add_executable(${PROJECT_NAME} Main.cpp ${CXX_SOURCE_FILES} ${GMSH_SOURCE_FILES})

target_link_libraries(runPFEM triangle tetgen lapacke metis ${MPI_LIBRARIES} ${PETSC_LIBRARIES})