add_executable(
toumou_example
    main_example.cpp
)

target_include_directories(
toumou_example 
PUBLIC 
    ${TOUMOU_INCLUDE_DIR}
)

target_link_libraries(
toumou_example 
PUBLIC 
    toumou_engine
    spdlog::spdlog
)

install(
TARGETS 
    toumou_example 
DESTINATION 
    Toumou-${CMAKE_PROJECT_VERSION}
)
