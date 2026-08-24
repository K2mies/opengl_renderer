#version 330 core

//-------------------------------------------------- uniforms
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

//---------------------------------------------------- in/out
in  vec3 Normal;
in  vec3 fragmentPosition;
out vec4 FragColor;

void main()
{ 
    float ambientStrength = 0.1;

    vec3  ambient         = ambientStrength * lightColor;

    vec3  normal          = normalize(Normal);

    vec3  lightDirection;
          lightDirection  = lightPosition - fragmentPosition;
          lightDirection  = normalize(lightDirection);
    
    float diff;
          diff            = dot(normal, lightDirection);
          diff            = max(diff,   0.0);

    vec3  diffuse         = diff * lightColor;

    vec3  result;
          result          = ambient + diffuse;
          result          = result  * objectColor;

          FragColor       = vec4(result, 1.0);
}
