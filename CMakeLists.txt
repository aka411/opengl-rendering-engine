cmake_minimum_required(VERSION 3.17)

project(project-gltf)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

add_executable(gltf-test-app

src/main.cpp
src/gpu_texture_manager.cpp
src/gpu_buffer_manager.cpp
 "glad/glad.c"      "src/buffer_management_system.cpp" "src/common_header_imp.cpp" "src/camera.cpp"   "src/skin_animation_system.cpp"

 "src/renderer.cpp"  "src/shader_manager.cpp"  "src/transformation_system.cpp"     "src/gltf_flat_parser.cpp"  "src/hierarchy_traverser_flat.cpp"    "src/bone_animation_system.cpp"  "src/vertex_format_manager.cpp")

target_include_directories(gltf-test-app PRIVATE

${CMAKE_SOURCE_CURRENT_DIR}/include
${CMAKE_SOURCE_CURRENT_DIR}/glad

)



FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.2.20
)

FetchContent_MakeAvailable(SDL3)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	1.0.1 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)


target_link_libraries(gltf-test-app PRIVATE

  SDL3::SDL3
  glm::glm

)


  add_custom_command(TARGET gltf-test-app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:SDL3::SDL3>
    $<TARGET_FILE_DIR:gltf-test-app>
    COMMENT "Copying SDL3 DLL to the executable directory"
  )
