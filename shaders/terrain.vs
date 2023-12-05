#version 330 core // versión de GLSL
const int MAX_BONES = 100;
layout (location = 0) in vec3 in_position;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in vec3 in_normal;

out vec3 our_normal;
out vec3 fragPos;
out vec2 our_uv; // coordenadas de textura

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){

	gl_Position = projection * view * model * vec4(in_position, 1.0);
	fragPos = vec3(model * vec4(in_position, 1.0));
	// usamos el truco de Graham Smith
	our_normal = mat3(transpose(inverse(model))) * in_normal;
	our_uv = in_uv;
}