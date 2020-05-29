cmake_minimum_required(VERSION 3.15)
cmake_policy(VERSION 3.15)

include(FetchContent)
project(MicroProjetJIN4 VERSION 1.0.0 LANGUAGES CXX)

set(ARCH "x64")
set (BUILD_SHARED_LIBS FALSE)

# pugixml
FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
        GIT_TAG v1.10
)
FetchContent_MakeAvailable(pugixml)

# SFML
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(sfml)


# Box2D
FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d
        GIT_TAG 1025f9a10949b963d6311995910bdd04f72dae6c
)
FetchContent_MakeAvailable(box2d)

# Permet d'avoir une macro WSL
# Vraiment pas nécessaire pour faire fonctionner dans des conditions normales
if (CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_HOST_SYSTEM_VERSION MATCHES "Microsoft")
    add_definitions(-DWSL)
endif()

add_subdirectory(mainLauncher)
add_subdirectory(src)
add_subdirectory(unitTests)
add_subdirectory(imgui-sfml)

set(TMXLITE_STATIC_LIB true)
add_subdirectory(tmxlite)
