pybind11_add_module(
lava_python 
    lava_python.cpp
)

target_include_directories(
lava_python 
PUBLIC 
    ${LAVA_INCLUDE_DIR}
)

target_link_libraries(
lava_python 
PUBLIC 
    lava_engine
)

install(
TARGETS 
    lava_python 
DESTINATION 
    Lava-${CMAKE_PROJECT_VERSION}
)
