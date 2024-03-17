#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    vec3 specular;    
    float shininess;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform SpotLight light;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);

    float costheta = dot(lightDir, normalize(-light.direction));//light dir
    if (costheta > light.cutOff){   
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
        // diffuse 
        vec3 norm = normalize(Normal);
    
        float dotNormLight = dot(norm, lightDir);
        float diff = max(dotNormLight, 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec;
        if (dotNormLight > 0) {
            spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        }
        else spec = 0.0f;
        vec3 specular = light.specular * (spec * material.specular);  
        
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
    else
        FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);

} 