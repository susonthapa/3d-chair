#version 330 core
out vec4 FragColor;

in vec3 ourNormal;

in vec3 fragmentPosition;

in vec2 ourTexture;

//Ambient Light Reflection coefficient
uniform float ambLightStrength;

uniform sampler2D texture1;


void main()
{
    //Specular Light Reflection Coefficient
    float specularStrength=0.3;

    //Object Texture Intensity
    vec3 textureIntensity=vec3(0.5,0.5,0.5);

    //Color of the Light Source
    vec3 lightColor=vec3(1.0,1.0,1.0);

    //Position of the Light Source
    vec3 lightPosition=vec3(0.0,1.0,1.0);

    //Calculate the ambient Reflection
    vec3 ambient=ambLightStrength*lightColor;

    //Calculate the unit Vector of the normals and light Directions
    vec3 normal=normalize(ourNormal);
    vec3 lightDirection=normalize(lightPosition-fragmentPosition);

    //Use max(),the dot product may be negative,and we don't want that
    float difference=max(dot(normal,lightDirection),0.0);

    //Calculate the Diffuse Reflection
    vec3 diffuse=difference*lightColor;
    vec3 viewDirection=(lightPosition-fragmentPosition);

    //Calculate the Reflection Vector R
    vec3 reflectionDirection=reflect(-lightDirection,normal);

    //32=intensity of specular Reflection i.e (cos)pow nS
    float spec=pow(max(dot(viewDirection,reflectionDirection),0.0),8);

    //Get the specular Strength for this light Color
    vec3 specular=specularStrength*spec*lightColor;

    //Calculate the total intensity of all the reflections
    vec3 totalLight=(ambient+diffuse+specular)*textureIntensity;

    //Pass the final Color to Test And Blending Component
	FragColor=texture(texture1,ourTexture)*vec4(totalLight,1.0);
}
