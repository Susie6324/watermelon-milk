#version 400 core
layout(location = 0) in vec3  position;
layout(location = 1) in vec3  velocity;
layout(location = 2) in float size;
layout(location = 3) in float life;
layout(location = 4) in vec4  color;

out Particle{
    vec3  position;
    vec3  velocity;
    float size;
    float life;
    vec4  color;
}particle;

uniform float duration;
uniform vec3 back;
void main(){
    particle.position = position + velocity;
    particle.velocity = velocity + back;

    particle.size     = size;
    particle.life     = life + duration;
    particle.color    = color;
}
