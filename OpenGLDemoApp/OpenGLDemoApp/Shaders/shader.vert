#version 330                                                                  

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 u_model;                                                 
uniform mat4 u_projection;                                                          
uniform mat4 u_view;                                                          
                                                                              
void main()                                                                  
{                                                                             
    gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0);									  
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0);

    TexCoord = tex;								  
    
    Normal = mat3(transpose(inverse(u_model))) * norm;

    FragPos = (u_model * vec4(pos, 1.0)).xyz;

}
