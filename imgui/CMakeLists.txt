
file(GLOB SOURCES CONFIGURE_DEPENDS *.h *.cpp)

add_library(imgui ${SOURCES})
target_include_directories(imgui PUBLIC ./)
target_link_libraries(imgui sfml-graphics)
