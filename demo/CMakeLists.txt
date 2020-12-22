cmake_minimum_required(VERSION 3.10)

project(avr_boy_emulator_demo)

add_executable(
        ${PROJECT_NAME}
        main.cpp #
)

target_include_directories(${PROJECT_NAME} PUBLIC ../include)

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC project_options #
               project_warnings #
               avr_boy_core #
               avr_boy_emulator_lib #
               CONAN_PKG::spdlog #
)
