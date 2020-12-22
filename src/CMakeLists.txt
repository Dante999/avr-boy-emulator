project(avr_boy_emulator_lib)

add_library(
        ${PROJECT_NAME} #
        fifo_pipe.cpp #
        communicator_pipe.cpp
)

target_link_libraries(
        ${PROJECT_NAME} #
        PRIVATE project_options #
                project_warnings #
                avr_boy_core #
                CONAN_PKG::spdlog #
)

target_include_directories(${PROJECT_NAME} PUBLIC ../include)
