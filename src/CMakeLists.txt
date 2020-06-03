file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.h *.cpp *.hpp)

add_library(src ${SOURCES})
target_include_directories(src PUBLIC ./)
target_include_directories(src PUBLIC ../imgui-sfml ../tmxlite/include/)
target_link_libraries(src tmxlite imgui-sfml pugixml sfml-graphics sfml-audio sfml-system box2d)
