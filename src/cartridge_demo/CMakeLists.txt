cmake_minimum_required(VERSION 3.10)

project(cartridge_demo)

add_executable(
        ${PROJECT_NAME}
        main.cpp #
	ball.cpp #
)

target_include_directories(${PROJECT_NAME} PUBLIC ../../include)

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC project_options #
               project_warnings #
               avr_boy_core #
               CONAN_PKG::spdlog #
)
