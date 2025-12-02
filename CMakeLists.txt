cmake_minimum_required(VERSION 3.17)

project(project-gltf)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)







add_subdirectory(external/the-engine/the-engine/ecs)






add_executable(opengl-rendering-engine

 src/main.cpp
 "glad/glad.c"
 
 
 "src/common_header_imp.cpp"
 "src/camera.cpp"





  "src/model-loading/gltf_intermediate_mapper.cpp"

  "src/low-level/vertex_format_manager.cpp"
  "src/model-loading/data_convertor.cpp"
# "src/model-loading/gltf_animation_extractor.cpp"

 "src/engine/engine_core.cpp"
 "src/engine/engine_loader.cpp"
 "src/low-level/vertex_format_manager.cpp"
 "src/low-level/gpu_buffer_manager.cpp"
 "src/low-level/gpu_buffer_sub_bump_allocator.cpp"
 "src/low-level/gpu_texture_manager.cpp"
 "src/low-level/gpu_material_system.cpp"
 "src/low-level/shader_code_generator.cpp"
 "src/low-level/shader_manager.cpp"
 "src/low-level/vertex_format_helper.cpp" 
 "src/low-level/world_vertex_buffer_management_system.cpp"

 "src/model-loading/vertex_attributes_repacker.cpp"
 "src/engine/engine_loader.cpp"



 "src/engine/engine_core.cpp"
 "src/rendering-system/render_system.cpp"
 "src/rendering-system/world_renderer.cpp"

 "src/model-loading/gltf_attribute_extractor.cpp"



 # UI SOURCE FILES


 "src/ui/src/builder/ui_element.cpp"

  "src/ui/src/systems/ui_vertex_buffer_management_system.cpp"

  "src/ui/src/utils/geometry_generator.cpp"
  



 "src/ui/src/systems/ui_mesh_system.cpp" 
 



 "src/ui/src/systems/ui_transformation_system.cpp"

 "src/ui/src/systems/performance_monitor_system.cpp"


 "src/ui/src/builder/ui_text_label_builder.cpp"
   "src/ui/src/builder/ui_graph_builder.cpp"
  "src/ui/src/builder/ui_builder.cpp"
  "src/ui/src/builder/ui_base_builder.cpp" 
  "src/ui/src/builder/ui_panel_builder.cpp"


 "src/ui/src/core/ui_shader_manager.cpp"
  "src/ui/src/core/ui_core_system.cpp"
   "src/ui/src/ui_renderer.cpp"
)


 set_target_properties(opengl-rendering-engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")



target_include_directories(opengl-rendering-engine PRIVATE

${CMAKE_SOURCE_CURRENT_DIR}/include

${CMAKE_SOURCE_CURRENT_DIR}/glad
${CMAKE_SOURCE_CURRENT_DIR}/external/the-engine/the-engine/ecs/include

${CMAKE_SOURCE_CURRENT_DIR}/ui/include

)

#target_include_directories(opengl-rendering-engine PUBLIC ${glm_INCLUDE_DIR})

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


target_link_libraries(opengl-rendering-engine PRIVATE

  SDL3::SDL3
  glm::glm
  TheEngine_ECS
)


  add_custom_command(TARGET opengl-rendering-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:SDL3::SDL3>
   $<TARGET_FILE_DIR:opengl-rendering-engine>
    COMMENT "Copying SDL3 DLL to the executable directory"
  )
