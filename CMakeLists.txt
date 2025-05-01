cmake_minimum_required(VERSION 4.0)
project(HexMe)

find_package(Curses REQUIRED)

file (GLOB_RECURSE HexMe_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE HexMe_HEADERS CONFIGURE_DEPENDS "src/*.h")

set (HexMe_INCLUDE_DIRS "")
foreach (_headerFile ${HexMe_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND HexMe_INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES HexMe_INCLUDE_DIRS)

add_executable(hexme ${HexMe_SOURCES})
target_include_directories(hexme PRIVATE ${HexMe_INCLUDE_DIRS})
target_link_libraries(hexme ${CURSES_LIBRARIES})
