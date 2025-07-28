#version 130 // ∂‘”¶opengl∞Ê±æ 3.0
out vec3 FragPos;
out vec3 Normal;
out vec3 objectColor;
out vec3 specular_now;
out vec3 diffuse_now;
out vec2 st;
void main()
{
    FragPos = vec3(gl_ModelViewMatrix * gl_Vertex);
    Normal = vec3(gl_ModelViewMatrix * vec4(gl_Normal,0.0));
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    st = gl_MultiTexCoord0.st;
    objectColor = vec3(gl_Color);
    specular_now = vec3(gl_FrontMaterial.specular);
    diffuse_now = vec3(gl_FrontMaterial.diffuse);
}