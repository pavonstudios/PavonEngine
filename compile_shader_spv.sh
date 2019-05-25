cd ./Game/Assets/shaders/vk
glslangValidator -V shader.frag -o frag.spv
glslangValidator -V shader.vert -o vert.spv
glslangValidator -V skinshader.vert -o skin_shader.spv 

