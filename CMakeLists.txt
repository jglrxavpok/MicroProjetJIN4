cmake_minimum_required(VERSION 3.15)
cmake_policy(VERSION 3.15)

include(FetchContent)
project(JIN4 VERSION 1.0.0 LANGUAGES CXX)

set (BUILD_SHARED_LIBS FALSE)

FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
        GIT_TAG v1.10
)

FetchContent_MakeAvailable(pugixml)


FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
)

FetchContent_MakeAvailable(sfml)

# Permet d'avoir une macro WSL
# Vraiment pas nécessaire pour faire fonctionner dans des conditions normales
if (CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_HOST_SYSTEM_VERSION MATCHES "Microsoft")
    add_definitions(-DWSL)
endif()

add_subdirectory(mainLauncher)
add_subdirectory(src)
add_subdirectory(unitTests)
add_subdirectory(imgui)
