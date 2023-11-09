cmake_minimum_required(VERSION 3.26)
project(VulkanProgramming)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

add_executable(VulkanProgramming main.cpp)
target_include_directories(VulkanProgramming PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanProgramming PRIVATE ${Vulkan_LIBRARIES})