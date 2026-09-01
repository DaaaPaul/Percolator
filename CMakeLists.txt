cmake_minimum_required(VERSION 3.36)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Percolator)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_TOOLCHAIN_FILE C:/Users/paulp/vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(Vulkan REQUIRED)
find_package(GLFW3 REQUIRED)
if(APPLE)
    find_library(COCOA Cocoa)
    find_library(IOKIT IOKit)
    find_library(CORE_FOUNDATION CoreFoundation)
endif()

add_executable(Percolator)

set(SOURCE_FILES
    Source/main.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

target_sources(Percolator PRIVATE 
    ${SOURCE_FILES}
)
target_link_libraries(Percolator PRIVATE
    ${GLFW3_LIBRARY}
    Vulkan::Vulkan
    $<$<PLATFORM_ID:Darwin>:${COCOA}>
    $<$<PLATFORM_ID:Darwin>:${IOKIT}>
    $<$<PLATFORM_ID:Darwin>:${CORE_FOUNDATION}>
)
target_include_directories(Percolator PRIVATE
    ${GLFW3_INCLUDE_DIR}
	Source
)

set(ADDRESS_SANITIZER OFF)
if(ADDRESS_SANITIZER)
	target_compile_options(Percolator PRIVATE 
		$<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address /Zi>
		$<$<CXX_COMPILER_ID:MSVC>:/fsanitize=fuzzer>
	)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Percolator)