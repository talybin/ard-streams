cmake_minimum_required(VERSION 3.5)
project(ard-streams)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features (${PROJECT_NAME} INTERFACE cxx_std_11)

target_include_directories(${PROJECT_NAME}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ DESTINATION include/${PROJECT_NAME})

