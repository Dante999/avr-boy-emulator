project(${PROJECT_NAME}_tests)

add_executable(
        ${PROJECT_NAME}
        fifo_pipe_test.cpp
)

# target_include_directories(${PROJECT_NAME} PUBLIC ../include)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE project_options #
                project_warnings #
                CONAN_PKG::gtest #
                avr_boy_core #
                avr_boy_emulator_lib
)
