project(${PROJECT_NAME}_lib)

add_library(
        ${PROJECT_NAME} #
        fifo_pipe.cpp #
        communicator_pipe.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ../include)

target_link_libraries(
        ${PROJECT_NAME} #
        PRIVATE project_options #
                project_warnings #
                avr_boy_core #
)
