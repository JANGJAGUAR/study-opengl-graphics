#version 330 core
in vec4 toColor;
out vec4 FragColor;
uniform vec3 objectColor;

void main()
{
    vec3 result = objectColor;
    FragColor = vec4(result, 1.0);
}
