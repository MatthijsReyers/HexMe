cmake_minimum_required(VERSION 4.0)
project(HexMe)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)

file (GLOB_RECURSE HexMe_SOURCES CONFIGURE_DEPENDS "source/*.cpp")
file (GLOB_RECURSE HexMe_HEADERS CONFIGURE_DEPENDS "source/*.h")
file (GLOB_RECURSE HexMe_HEADERS CONFIGURE_DEPENDS "source/*.hpp")

set (HexMe_INCLUDE_DIRS "")
foreach (_headerFile ${HexMe_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND HexMe_INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES HexMe_INCLUDE_DIRS)

add_executable(hexme ${HexMe_SOURCES})
target_include_directories(hexme PRIVATE ${HexMe_INCLUDE_DIRS})
target_link_libraries(hexme ${CURSES_LIBRARIES})
