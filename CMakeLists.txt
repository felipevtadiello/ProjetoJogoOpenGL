cmake_minimum_required(VERSION 3.12)

project(ProjetoJogoOpenGL)

add_executable(ProjetoJogoOpenGL main.cpp  )

# Diretórios de inclusão para as dependências
target_include_directories(ProjetoJogoOpenGL PRIVATE deps/glm 
                                              deps/glfw/include
                                              deps/glew/include)

# Diretórios onde as bibliotecas estão localizadas
target_link_directories(ProjetoJogoOpenGL PRIVATE deps/glfw/lib-vc2019
                                           deps/glew/lib/Release/x64)

# Bibliotecas a serem vinculadas
target_link_libraries(ProjetoJogoOpenGL PRIVATE glfw3.lib glew32.lib opengl32.lib)

# Copiar glew32.dll após a construção do executável
add_custom_command(TARGET ProjetoJogoOpenGL POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/glew/bin/Release/x64/glew32.dll" "${CMAKE_BINARY_DIR}/glew32.dll")
