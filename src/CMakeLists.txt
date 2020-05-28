file(GLOB SOURCES CONFIGURE_DEPENDS *.h *.cpp)

add_library(src ${SOURCES})
target_include_directories(src PUBLIC ./ ../imgui-sfml)
target_link_libraries(src tmxlite imgui-sfml pugixml sfml-graphics sfml-audio sfml-system box2d)
