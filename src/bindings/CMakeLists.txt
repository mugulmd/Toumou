pybind11_add_module(
toumou 
    toumou_python.cpp
)

target_include_directories(
toumou 
PUBLIC 
    ${TOUMOU_INCLUDE_DIR}
)

target_link_libraries(
toumou 
PUBLIC 
    toumou_engine
)

install(
TARGETS 
    toumou 
DESTINATION 
    Toumou-${CMAKE_PROJECT_VERSION}
)
