#version 330 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertColor;
layout (location = 2) in vec2 vertTexPos;

out vec3 daColor;
out vec2 texCoords;

uniform mat4 transform;

void main()
{
   gl_Position = transform * vec4(vertPos.x, vertPos.y, vertPos.z , 1.0);
   daColor = vertColor;
   texCoords = vertTexPos;
}