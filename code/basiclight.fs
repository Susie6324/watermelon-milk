#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;  
    vec3 direction;
    float constant;
    float linear;
    float quadratic;
};
in vec2 st;
in vec3 Normal;  
in vec3 FragPos;  
in vec3 objectColor;
in vec3 specular_now;
in vec3 diffuse_now;
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform Light light;
float cutOff;
float outerCutOff;
uniform sampler2D tex;
void main()
{
    // ambient
    vec4 color = texture2D(tex, st);
    vec3 tempcolor = objectColor * vec3(color);
    vec3 ambient = 0.5  *  lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuse_now * lightColor;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(viewDir , halfwayDir), 0.0), 0.0);
    vec3 specular = specular_now * vec3(0.3) * spec * lightColor;  
    
    //spotlight
    cutOff = cos(radians(15.5f + (light.position.y / 500.0f * 2.0f)));
    outerCutOff = cos(radians(17.5f));
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (cutOff - outerCutOff);
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    
    vec3 result = (ambient  + diffuse /1.75  + specular) * tempcolor;
    FragColor = vec4(result, 1.0);
} 