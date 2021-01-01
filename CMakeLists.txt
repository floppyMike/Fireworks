cmake_minimum_required(VERSION 3.0.0)
project(Fireworks VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory("CustomLibrary")

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE includes)
target_link_libraries(${PROJECT_NAME} PRIVATE CustomLibrary ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_MIXER_LIBRARY})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
