#version 330 core
out vec3 color;
in vec2 f_uv;
void main() {
    color = vec3(f_uv, 0);
}