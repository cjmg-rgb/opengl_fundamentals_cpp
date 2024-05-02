#version 440

in vec3 fs_position;
in vec3 fs_color;
in vec3 fs_normal;
in vec2 fs_texcoord;

out vec4 FragmentColor;

uniform sampler2D texture0;
uniform vec3 lightPos0;
uniform vec3 camPos;

void main()
{
    // Ambient Light
    vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);

    // Diffuse Reflector
    vec3 posToLightDirVec = normalize(lightPos0 - fs_position);
    float diffuseConstant = clamp(dot(posToLightDirVec, fs_normal),0,1);
    vec3 diffuseFinal = diffuseConstant * vec3(1.f, 1.f, 1.f);

    // Specular Reflector
    vec3 lightToPosDirVec = normalize(fs_position - lightPos0);
    vec3 posToViewDirVec = normalize(camPos - fs_position);
    vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(fs_normal)));
    float specularConstant = pow(max(dot(reflectDirVec, posToViewDirVec), 0.0), 30);
    vec3 specularFinal = vec3(1.0f, 1.0f, 1.0f) * specularConstant;
    
    FragmentColor = texture(texture0, fs_texcoord) * vec4(fs_color, 1.0)
        * (vec4(ambientLight, 1.0) + vec4(diffuseFinal, 1.0) + vec4(specularFinal, 1.0));
}
