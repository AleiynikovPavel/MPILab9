cmake_minimum_required(VERSION 3.18)
project(lab9)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab9 main.cpp)

find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
target_link_libraries(lab9 PUBLIC MPI::MPI_CXX)