cmake_minimum_required(VERSION 3.12)

project(ProjetoJogoOpenGL)

add_executable(ProjetoJogoOpenGL main.cpp  )

# Diret�rios de inclus�o para as depend�ncias
target_include_directories(ProjetoJogoOpenGL PRIVATE deps/glm 
                                              deps/glfw/include
                                              deps/glew/include)

# Diret�rios onde as bibliotecas est�o localizadas
target_link_directories(ProjetoJogoOpenGL PRIVATE deps/glfw/lib-vc2019
                                           deps/glew/lib/Release/x64)

# Bibliotecas a serem vinculadas
target_link_libraries(ProjetoJogoOpenGL PRIVATE glfw3.lib glew32.lib opengl32.lib)

# Copiar glew32.dll ap�s a constru��o do execut�vel
add_custom_command(TARGET ProjetoJogoOpenGL POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/glew/bin/Release/x64/glew32.dll" "${CMAKE_BINARY_DIR}/glew32.dll")
