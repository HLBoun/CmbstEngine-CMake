cmake_minimum_required(VERSION 4.2.1)

#set(CMAKE_GENERATOR Ninja)
find_package(Vulkan 1.4.335)
find_package(Doxygen)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_COMPILER "c:/msys64/ucrt64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "c:/msys64/ucrt64/bin/gcc.exe")

set(SRC
      src/main.cpp
      src/first_app.cpp
      src/cmbst_window.cpp
      src/cmbst_pipeline.cpp
      src/cmbst_device.cpp
      src/cmbst_model.cpp
      src/cmbst_swap_chain.cpp
)


project(CmbstEngine)

add_executable(${PROJECT_NAME} ${SRC})

add_subdirectory(ext/glfw-3.4)
#add_library(myInclude include)
#add_subdirectory(ext/VulkanSDK)

target_include_directories(${PROJECT_NAME}
      PUBLIC
      ${PROJECT_SOURCE_DIR}/ext/glfw-3.4/include
      ${PROJECT_SOURCE_DIR}/ext/VulkanSDK/1.4.335.0/Include
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/shaders
)

target_link_libraries(${PROJECT_NAME}
      PRIVATE
      Vulkan::Vulkan
      ${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw-3.4/build/src/Debug/glfw3.lib
)
