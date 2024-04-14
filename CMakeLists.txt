cmake_minimum_required(VERSION 3.26)
project(WaterManagementDA)

set(CMAKE_CXX_STANDARD 17)


# Define the common output directory
set(output_dir "${CMAKE_SOURCE_DIR}/output")

# Create the output directory if it doesn't exist
file(MAKE_DIRECTORY ${output_dir})

# Add a preprocessor definition for the output directory
add_compile_definitions(OUTPUT_DIR="${output_dir}")

# Define the common output directory
set(dataset_dir "${CMAKE_SOURCE_DIR}/Datasets")

# Create the output directory if it doesn't exist
file(MAKE_DIRECTORY ${dataset_dir})

# Add a preprocessor definition for the output directory
add_compile_definitions(DATASET_DIR="${dataset_dir}")


add_executable(WaterManagementDA main.cpp
        src/data_structures/MutablePriorityQueue.h
        src/data_structures/Graph.h
        src/Menu.cpp
        src/Menu.h
        src/ManagementSystem.cpp
        src/ManagementSystem.h
        src/terminalColors.h)







