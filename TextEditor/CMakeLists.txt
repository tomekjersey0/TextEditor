cmake_minimum_required(VERSION 3.25)
project(TextEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# vcpkg toolchain (if not already set globally)
# -----------------------------
# You can skip this if you set CMAKE_TOOLCHAIN_FILE globally
# set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# -----------------------------
# Include directories
# -----------------------------
include_directories("C:/vcpkg/installed/x64-windows/include")

# -----------------------------
# Link directories
# -----------------------------
link_directories("C:/vcpkg/installed/x64-windows/lib")

# -----------------------------
# Create an imported target for PDCurses
# -----------------------------
add_library(PDCurses STATIC IMPORTED)
set_target_properties(PDCurses PROPERTIES
    IMPORTED_LOCATION "C:/vcpkg/installed/x64-windows/lib/pdcurses.lib"
    INTERFACE_INCLUDE_DIRECTORIES "C:/vcpkg/installed/x64-windows/include"
)

# -----------------------------
# Add your executable
# -----------------------------
file(GLOB SOURCES "*.cpp")   # automatically add all cpp files
add_executable(TextEditor ${SOURCES})

# -----------------------------
# Link PDCurses
# -----------------------------
target_link_libraries(TextEditor PRIVATE PDCurses)

# Optional: add extra compiler flags if needed
# target_compile_options(TextEditor PRIVATE /W4 /MD)
