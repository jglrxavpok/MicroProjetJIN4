
file(GLOB SOURCES CONFIGURE_DEPENDS *.h *.cpp)

add_library(imgui-sfml ${SOURCES})
target_include_directories(imgui-sfml PUBLIC ./)
target_link_libraries(imgui-sfml sfml-graphics)
