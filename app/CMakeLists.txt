# cmake_minimum_required(VERSION 3.10)

# project("avr_boy_emulator")

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
# add_library(project_warnings INTERFACE)

# standard compiler warnings include(cmake/compiler-warnings.cmake)
# set_project_warnings(project_warnings)

add_executable(nconv main.cpp graphx_loop.cpp)

target_link_libraries(nconv PRIVATE project_options project_warnings
                                    avr_boy_core CONAN_PKG::imgui-sfml)
