#version 330 core
layout(location = 0) out vec4 output;

in vec3 v_FragPos;

uniform samplerCube skybox;

void main()
{    
    output = texture(skybox, v_FragPos);
    //output = vec4(1, 0, 0, 1);
}

