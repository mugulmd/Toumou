add_executable(
lava_example
    main_example.cpp
)

target_include_directories(
lava_example 
PUBLIC 
    ${LAVA_INCLUDE_DIR}
)

target_link_libraries(
lava_example 
PUBLIC 
    lava_engine
    spdlog::spdlog
)

install(
TARGETS 
    lava_example 
DESTINATION 
    Lava-${CMAKE_PROJECT_VERSION}
)
